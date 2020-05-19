#include "web.h"

int serverfd;
int unixfd;
struct sockaddr_un cgiAddr;

int main(int argc, char *argv[]) {
    signal(SIGINT, handle_signal);      // Close Server by Interrupt(Control-C);
    signal(SIGTERM, handle_signal);     // Close Server by kill;
    start_server();
    return 0;
}

void start_server() {
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    int clientfd;
    int clientAddrSize = sizeof(clientAddr);
    pthread_t tid;

    if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOG_FAILED("EstablishError.");
        perror("<Master Socket>");
        exit(EXIT_FAILURE);
    }

    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(IP);
    serverAddr.sin_port = htons(PORT);
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

    if (bind(serverfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr))) {
        LOG_FAILED("BindError.");
        perror("<Binding>");
        exit(EXIT_FAILURE);
    }

    if (listen(serverfd, 10)) {
        LOG_FAILED("ListenError.");
        perror("<Listening>");
        exit(EXIT_FAILURE);
    }

    if ((unixfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        LOG_FAILED("EstablishError.");
        perror("<Unix Socket>");
        exit(EXIT_FAILURE);
    }

    bzero(&cgiAddr, sizeof(cgiAddr));
    cgiAddr.sun_family = AF_UNIX;
    strcpy(cgiAddr.sun_path, UNIX_SOCK_PATH);

    if (connect(unixfd, (struct sockaddr *)&cgiAddr, sizeof(cgiAddr)) < 0) {
        LOG_FAILED("ConnectionError. <UnixSocket>");
        perror("<Unix Socket Connection>");
        exit(EXIT_FAILURE);
    }

    LOG_SUCCESS("Establish Success!");
    LOG_INFO("Server Starting Up...");
    LOG_WAIT("Listening...");

    while ((clientfd = accept(serverfd, (struct sockaddr *)&clientAddr, (socklen_t *)&clientAddrSize))) {
		if (pthread_create(&tid, NULL, thread_process, &clientfd)) {
            LOG_FAILED("PthreadCreateError.");
            perror("<Thread>");
            close(clientfd);
            continue;
        }
    }

    if (clientfd < 0) {
        LOG_FAILED("ClientAcceptError.");
        perror("<Accept>");
    }

    close(unixfd);
    close(serverfd);
}

void *thread_process(void *arg) {
    int sockfd = *(int *)arg;
    int receive_size = 0;
    char path_request[MAX_BUFFER_LEN];

    LOG_INFO("Client Connected");

    if ((receive_size = recv(sockfd, path_request, MAX_BUFFER_LEN, 0))) {
        path_request[receive_size] = '\0';
        Request *p_request = parse_request(path_request);
        Response *p_response = process(p_request);
        send(sockfd, p_response->response, strlen(p_response->response), 0);
        free_request(p_request);
        free_response(p_response);
    }
    if (receive_size == 0) {
        LOG_INFO("Client Disconnected");
        fflush(stdout);
    } else if(receive_size == -1) {
        LOG_FAILED("ReceiveError.");
        perror("<Recv>");
    }
    close(sockfd);
    return 0;
}

Response *process(Request *p_request) {
    Response *p_response = NULL;
    int is_static = strcspn(p_request->uri, ".") != strlen(p_request->uri)? 1: 0;

    if (is_static) {
        int file_len = strcspn(p_request->uri, ".");
        int ext_len = strlen(p_request->uri) - file_len - 1;
        char *ext = (char *)malloc(ext_len + 1);
        
        strcpy(ext, p_request->uri + file_len + 1);
        ext[ext_len] = '\0';
        p_response = process_static(p_request, ext);
    } else {
        int received_bytes = 0;
        char buffer[MAX_CGI_LEN];
        char *info = (char *)malloc(strlen(p_request->uri) + strlen(p_request->body) + 2);
        strcpy(info, p_request->uri);
        strcat(info, ";");
        strcat(info, p_request->body);
        info[strlen(p_request->uri) + strlen(p_request->body) + 1] = '\0';

        if (send(unixfd, info, strlen(info), 0) == -1) {
            LOG_FAILED("SendError.");
            perror("Unix Socket Send");
        }

        received_bytes = recv(unixfd, buffer, MAX_CGI_LEN, 0);
        buffer[received_bytes] = '\0';
        if (received_bytes <= 1) {

        }
        p_response = process_dynamic(p_request, buffer);
        free(info);
    }

    return p_response;
}

void handle_signal(int signo) {
    if (signo == SIGINT){
        fprintf(stderr, "Signal: SIGINT(%d)\n", signo);
    } else if (signo == SIGTERM){
        fprintf(stderr, "Signal: SIGTERM(%d)\n", signo);
    }

    close(serverfd);
    exit(EXIT_SUCCESS);
}
