#include "cgi.h"

int serverfd = 0;

int main() {
    signal(SIGINT, handle_signal);      // Close Server by Interrupt(Control-C);
    signal(SIGTERM, handle_signal);     // Close Server by kill;
    start_listen();
    return 0;
}

void start_listen() {
    int clientfd = 0;
    int sockaddr_size = sizeof(struct sockaddr_in);
    struct sockaddr_un servaddr;
    struct sockaddr_in client;
    pthread_t tid;

    if ((serverfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("<Socket>");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, UNIX_SOCK_PATH);
    unlink(UNIX_SOCK_PATH);
    chmod(UNIX_SOCK_PATH, 00640);

    if (bind(serverfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("<Bind>");
        exit(EXIT_FAILURE);
    }

    if (listen(serverfd, SOMAXCONN) < 0) {
        perror("<Listen>");
        exit(EXIT_FAILURE);
    }

    while ((clientfd = accept(serverfd, (struct sockaddr *)&client, (socklen_t *)&sockaddr_size))) {
        if (pthread_create(&tid, NULL, thread_process, (void *)&clientfd)) {
            printf("<Thread>\n");
            close(clientfd);
            continue;
        }
    }

    if (clientfd < 0) {
        perror("<Accept>");
        close(serverfd);
        exit(EXIT_FAILURE);
    }

    unlink(UNIX_SOCK_PATH);
    close(serverfd);
}

void *thread_process(void *arg) {
    int sockfd = *(int *)arg;
    int receive_size;
    char path_request[MAX_LEN];
    char *body = NULL;

    printf("[Client Connected]\n");

    while ((receive_size = recv(sockfd, path_request, MAX_LEN, 0))) {
        path_request[receive_size] = '\0';
        body = router(path_request);
        send(sockfd, body, strlen(body), 0);
        memset(path_request, 0, strlen(path_request));
    }

    if (receive_size == 0) {
        printf("[Client Disconnected]\n");
        fflush(stdout);
    } else if(receive_size == -1) {
        perror("<Recv>");
    }
    
    // close(sockfd);
    return 0;
}

void handle_signal(int signo){
    if (signo == SIGINT) {
        fprintf(stderr, "Signal: SIGINT(%d)\n", signo);
    } else if (signo == SIGTERM){
        printf("SIGTERM\n");
        fprintf(stderr, "received signal: SIGTERM(%d)\n", signo);
    }

    close(serverfd);
    unlink(UNIX_SOCK_PATH);
    exit(EXIT_SUCCESS);
}
