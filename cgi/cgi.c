#include "cgi.h"

int unixfd = 0;

int main() {
    unix_socket();
    return 0;
}

void unix_socket() {
    struct sockaddr_un cgiAddr;
    bzero(&cgiAddr, sizeof(cgiAddr));
    cgiAddr.sun_family = AF_UNIX;
    strcpy(cgiAddr.sun_path, UNIX_SOCK_PATH);

    if ((unixfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        // error
    }

    unlink(UNIX_SOCK_PATH);

    if (bind(unixfd, (struct sockaddr *)&cgiAddr, sizeof(cgiAddr)) < 0){
        // error
    }
    chmod(UNIX_SOCK_PATH, 00640);

    if (listen(unixfd, SOMAXCONN) < 0){
        // error
    }

    int cliendfd = 0;
    int buffer_len = 0;
    char buffer[MAX_REQUEST_LEN];
    while (1) {
        if((cliendfd = accept(unixfd, NULL, NULL)) < 0){
            // error
            continue;
        }
        buffer_len = recv(cliendfd, buffer, MAX_REQUEST_LEN, 0);
        buffer[buffer_len] = '\0';
        printf("MSG: %s\n", buffer);
        send(cliendfd, buffer, buffer_len, 0);

        close(cliendfd);
    }

    close(unixfd);
}
