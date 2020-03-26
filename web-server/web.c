#include "web.h"

int main(int argc, char *argv[]) {
    int sockfd = build_socket();
    start_server(sockfd);
    return 0;
}

int build_socket() {
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    LOG_WAIT("Binding Socket...");
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(IP);
    serverAddr.sin_port = htons(PORT);
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

    if (!bind(serverfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr))) {
        LOG_SUCCESS("Binding Success!");
    } else {
        LOG_FAILED("BindingError! Shutting Down...");
        exit(-1);
    }

    return serverfd;
}

void start_server(int serverfd) {
    char request[MAX_REQUEST_LEN]; // TODO: Use dynamic allocate.

    if (!listen(serverfd, 10)) {
        LOG_INFO("Server Startup!");
    } else {
        LOG_FAILED("ConnectionError. Shutting Down...");
        exit(-1);
    }

    while (1) {
        struct sockaddr_in clientAddr;
        int addrlen = sizeof(clientAddr);
        int clientfd = accept(serverfd, (struct sockaddr *)&clientAddr, (socklen_t *)&addrlen);
        recv(clientfd, request, sizeof(request), 0);
        Request *p_request = parse_request(request);
        Response *p_response = process(p_request);
        send(clientfd, p_response->response, strlen(p_response->response), 0);
        print_request(p_request);
        print_response(p_response);
        free_request(p_request);
        free_response(p_response);
        close(clientfd);
    }

    close(serverfd);
}

Response *process(Request *p_request) {
    int is_static = strcspn(p_request->uri, ".") != strlen(p_request->uri)? 1: 0;
    Response *p_response = NULL;

    if (is_static) {
        int file_len = strcspn(p_request->uri, ".");
        int ext_len = strlen(p_request->uri) - file_len - 1;
        char *ext = (char *)malloc(ext_len + 1);
        
        strcpy(ext, p_request->uri + file_len + 1);
        ext[ext_len] = '\0';
        p_response = process_static(p_request, ext);
    } else {
        // CGI Interface
        // strcpy(info->argument, p_request->uri + 1);
    }

    return p_response;
}

void error_handle(int sockfd) {
    close(sockfd);
}
