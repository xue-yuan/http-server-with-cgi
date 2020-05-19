#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern const char *TEMPLATE_DIR;
extern const char *EXTENSION;

typedef struct Parameter {
    char *key;
    char *value;
    struct Parameter *next_parameter;
} Parameter;

Parameter *argument_parse(char *);
void free_parameter(Parameter *);

char *open_template(char *);

#endif