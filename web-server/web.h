#ifndef WEB_H
#define WEB_H

#include "lib/request.h"
#include "lib/response.h"
#include "lib/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <sys/un.h>
#include <pthread.h>
#include <signal.h>

#define PORT 5278
#define IP "0.0.0.0"
#define MAX_BUFFER_LEN 1024
#define MAX_CGI_LEN 32768
#define UNIX_SOCK_PATH "/tmp/cgi.sock"

typedef struct Info {
    int *sockfd;
} Info;

void start_server();
void *thread_process(void *);
void handle_signal(int);
void client_leave(int);
void client_nonexist(int);
// To determine the path is requesting static file or not.
Response *process(Request *);

#endif
