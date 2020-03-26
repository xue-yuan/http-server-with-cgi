#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const char *BASE_DIR;
extern const char *STATIC_DIR;
extern const char *BACKEND_DIR;

void LOG_INFO(char *);
void LOG_SUCCESS(char *);
void LOG_WAIT(char *);
void LOG_FAILED(char *);
void exception(int, char *, char *);

#endif
