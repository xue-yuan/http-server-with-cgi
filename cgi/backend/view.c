#include "view.h"

const char *TEMPLATE_DIR = "./cgi/backend/templates";
const char *EXTENSION = ".html";

char *self_index() {
    int file_size = 0;
    char *body = NULL;
    char *template = "/index";
    char *path = (char *)malloc(strlen(TEMPLATE_DIR) + strlen(template) + strlen(EXTENSION) + 1);
    FILE *fp;
    
    strcpy(path, TEMPLATE_DIR);
    strcat(path, template);
    strcat(path, EXTENSION);

    printf("Path: %s\n", path);
    fp = fopen(path, "r");
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    body = (char *)malloc(file_size + 1);
    fread(body, 1, file_size, fp);
    body[file_size] = '\0';
    fclose(fp);
    free(path);
    // free(template);
    // printf("Body: %s\n", body);
    return body;
}

char *upload() {
    return NULL;
}

char *view() {
    return NULL;
}
