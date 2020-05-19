#include "view.h"

char *self_index(Parameter *parameter) {
    char *body = open_template("/index");

    return body;
}

char *upload(Parameter *parameter) {
    char *body = open_template("/upload");
    
    return body;
}

char *view(Parameter *parameter) {
    char *buffer = (char *)malloc(200);
    char *student_id = NULL;
    for (Parameter *p=parameter; p!=NULL; p=p->next_parameter) {
        if (!strcmp(p->key, "student_id")) {
            student_id = (char *)malloc(strlen(p->value + 1));
            strcpy(student_id, p->value);
            student_id[strlen(p->value)] = '\0';
        }
    }
    sprintf(buffer, "<html><head><meta charset=\"utf-8\"><title>View</title></head><body><h1>Student_ID: %s</h1></body></html>", student_id);

    return buffer;
}


// char *self_index(Parameter *parameter) {
//     int file_size = 0;
//     char *body = NULL;
//     char *template = "/index";
//     char *path = (char *)malloc(strlen(TEMPLATE_DIR) + strlen(template) + strlen(EXTENSION) + 1);
//     FILE *fp;
    
//     strcpy(path, TEMPLATE_DIR);
//     strcat(path, template);
//     strcat(path, EXTENSION);

//     printf("Path: %s\n", path);
//     fp = fopen(path, "r");
//     fseek(fp, 0L, SEEK_END);
//     file_size = ftell(fp);
//     fseek(fp, 0L, SEEK_SET);
//     body = (char *)malloc(file_size + 1);
//     fread(body, 1, file_size, fp);
//     body[file_size] = '\0';
//     fclose(fp);
//     free(path);

//     return body;
// }