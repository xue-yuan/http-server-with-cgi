#ifndef REQUEST_H
#define REQUEST_H

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S_UNDEF "UNDEFINED"
#define S_GET "GET"
#define S_POST "POST"

typedef enum Method {
    UNDEFINED,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH
} Method;

typedef struct Header {
    char *name;
    char *value;
    struct Header *next_header;
} Header;

typedef struct Request {
    Method method;
    char *uri;
    char *version;
    Header *headers;
    char *body;
} Request;

Request *parse_request(char *);
void free_header(Header *);
void free_request(Request *);

void print_request(Request *);

#endif
