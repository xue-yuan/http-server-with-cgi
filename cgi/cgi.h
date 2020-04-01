#ifndef CGI_H
#define CGI_H

#include "backend/router.h"
#include "../web-server/lib/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>

#define UNIX_SOCK_PATH "/tmp/cgi.sock"
#define MAX_LEN 1000

void start_listen();
void handle_signal(int);
void *thread_process(void *);

#endif