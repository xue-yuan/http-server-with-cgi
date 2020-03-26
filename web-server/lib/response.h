#ifndef RESPONSE_H
#define RESPONSE_H

#include "utils.h"
#include "request.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define ERR404 "./public/errors/404.html"

// For Content type
#define C_HTML "text/html"
#define C_JPEG "image/jpeg"
#define C_PNG "image/png"
#define C_JSON "application/json"
#define C_PDF "application/pdf"
#define C_DEFAULT "text/plain"

// Status Code
extern const char *S_200;
extern const char *S_301;
extern const char *S_404;

typedef struct Response {
    char *header;
    char *content_type;
    char *date;
    char *server;
    char *connection;
    char *body;
    char *response;
} Response;


Response *process_static(Request *, char *);
Response *process_dynamic(Request *, char *);
char *set_body(int, char *);
void build_response(Response *);
void free_response(Response *);

void print_response(Response *);

#endif
