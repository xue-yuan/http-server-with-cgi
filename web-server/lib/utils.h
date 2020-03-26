#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT 5278
#define IP "127.0.0.1"
#define MAX_REQUEST_LEN 1000

extern const char *BASE_DIR;
extern const char *STATIC_DIR;
extern const char *BACKEND_DIR;

void LOG_INFO(char *);
void LOG_SUCCESS(char *);
void LOG_WAIT(char *);
void LOG_FAILED(char *);
void exception(int, char *, char *);

#endif
