#include "utils.h"

const char *TEMPLATE_DIR = "./cgi/backend/templates";
const char *EXTENSION = ".html";

Parameter *argument_parse(char *arg) {
    Parameter *parameters = NULL;
    Parameter *next_parameter = NULL;

    if (!strcmp(arg, "")) {
        return parameters;
    }

    printf("ARG: %s\n", arg);

    while (arg[0] != '\0') {
        parameters = (Parameter *)malloc(sizeof(Parameter));
        if (!parameters) {
            // free
            return NULL;
        }

        size_t key_len = strcspn(arg, "=");
        parameters->key = (char *)malloc(key_len + 1);
        if (!parameters->key) {
            // free
            return NULL;
        }
        memcpy(parameters->key, arg, key_len);
        parameters->key[key_len] = '\0';
        arg += key_len + 1;
        printf("KEY: %s\n", parameters->key);
        printf("SIZE: %zu\n", key_len);

        size_t value_len = strcspn(arg, "\0");
        parameters->value = (char *)malloc(value_len + 1);
        if (!parameters->value) {
            // free
            return NULL;
        }
        memcpy(parameters->value, arg, value_len);
        parameters->value[value_len] = '\0';
        arg += value_len;
        printf("VALUE: %s\n", parameters->value);
        printf("SIZE: %zu\n", value_len);
        printf("ARG: -%s-\n", arg);
        printf("-----\n");
        parameters->next_parameter = next_parameter;
    }


    return parameters;
}

char *open_template(char *template) {
    int file_size = 0;
    char *body = NULL;
    char *path = (char *)malloc(strlen(TEMPLATE_DIR) + strlen(template) + strlen(EXTENSION) + 1);
    FILE *fp;
    
    strcpy(path, TEMPLATE_DIR);
    strcat(path, template);
    strcat(path, EXTENSION);

    // printf("Path: %s\n", path);
    fp = fopen(path, "r");
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    body = (char *)malloc(file_size + 1);
    fread(body, 1, file_size, fp);
    body[file_size] = '\0';
    fclose(fp);
    free(path);

    return body;
}