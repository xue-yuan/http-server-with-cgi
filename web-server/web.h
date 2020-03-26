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

#define PORT 5278
#define IP "127.0.0.1"
#define MAX_REQUEST_LEN 1000
#define UNIX_SOCK_PATH "/tmp/cgi.sock"

int build_socket();
void start_server(int);
void error_handle(int);
// To determine the path is requesting static file or not.
Response *process(Request *);

#endif
