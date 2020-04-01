#include "response.h"

const char *S_200 = "OK";
const char *S_301 = "Moved Permanently";
const char *S_404 = "Not Found";

Response *process_static(Request *p_request, char *ext) {
    int status_code = 0;
    char *version = (char *)malloc(strlen(p_request->version) + 1);
    char *file_path = (char *)malloc(strlen(STATIC_DIR) + strlen(p_request->uri) + 1);
    Response *p_response = (Response *)malloc(sizeof(Response));

    strcpy(version, p_request->version);
    strcpy(file_path, STATIC_DIR);
    strcat(file_path, p_request->uri);
    file_path[strlen(STATIC_DIR) + strlen(p_request->uri)] = '\0';

    if (0) {
        status_code = 301;
        p_response->header = (char *)malloc(strlen(version) + strlen(S_301) + 6);
        sprintf(p_response->header, "%s %d %s", version, status_code, S_301);
    } else if (access(file_path, F_OK) != -1) {
        status_code = 200;
        p_response->header = (char *)malloc(strlen(version) + strlen(S_200) + 6);
        sprintf(p_response->header, "%s %d %s", version, status_code, S_200);
    } else {
        status_code = 404;
        p_response->header = (char *)malloc(strlen(version) + strlen(S_404) + 6);
        sprintf(p_response->header, "%s %d %s", version, status_code, S_404);
    }

    p_response->content_type = set_content_type(ext);
    p_response->date = set_date();
    p_response->server = set_server();
    p_response->connection = set_connection();
    p_response->body = set_body(status_code, file_path);

    build_response(p_response);
    free(version);
    free(file_path);

    return p_response;
}

Response *process_dynamic(Request *p_request, char *template) {
    int status_code = 0;
    char *version = (char *)malloc(strlen(p_request->version) + 1);
    Response *p_response = (Response *)malloc(sizeof(Response));

    strcpy(version, p_request->version);

    if (0) {
        status_code = 301;
        p_response->header = (char *)malloc(strlen(version) + strlen(S_301) + 6);
        sprintf(p_response->header, "%s %d %s", version, status_code, S_301);
    } else if (template != NULL) {
        status_code = 200;
        p_response->header = (char *)malloc(strlen(version) + strlen(S_200) + 6);
        sprintf(p_response->header, "%s %d %s", version, status_code, S_200);
    } else {
        status_code = 404;
        p_response->header = (char *)malloc(strlen(version) + strlen(S_404) + 6);
        sprintf(p_response->header, "%s %d %s", version, status_code, S_404);
    }

    p_response->content_type = set_content_type("html");
    p_response->date = set_date();
    p_response->server = set_server();
    p_response->connection = set_connection();
    p_response->body = template;

    build_response(p_response);
    free(version);

    return p_response;
}

char *set_content_type(char *ext) {
    char *content_type = NULL;
    if (!strcmp(ext, "html")) {
        content_type = (char *)malloc(strlen(C_HTML) + 1);
        strcpy(content_type, C_HTML);
        content_type[strlen(C_HTML)] = '\0';
    } else if (!strcmp(ext, "jpg")) {
        content_type = (char *)malloc(strlen(C_JPEG) + 1);
        strcpy(content_type, C_JPEG);
        content_type[strlen(C_JPEG)] = '\0';
    } else if (!strcmp(ext, "png")) {
        content_type = (char *)malloc(strlen(C_PNG) + 1);
        strcpy(content_type, C_PNG);
        content_type[strlen(C_PNG)] = '\0';
    } else if (!strcmp(ext, "json")) {
        content_type = (char *)malloc(strlen(C_JSON) + 1);
        strcpy(content_type, C_JSON);
        content_type[strlen(C_JSON)] = '\0';
    } else if (!strcmp(ext, "pdf")) {
        content_type = (char *)malloc(strlen(C_PDF) + 1);
        strcpy(content_type, C_PDF);
        content_type[strlen(C_PDF)] = '\0';
    } else {
        content_type = (char *)malloc(strlen(C_DEFAULT) + 1);
        strcpy(content_type, C_DEFAULT);
        content_type[strlen(C_DEFAULT)] = '\0';
    }

    return content_type;
}

char *set_date() {
    char *date = (char *)malloc(30);
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(date, 30, "%a, %d %b %Y %H:%M:%S", &tm);
    date[30] = '\0';

    return date;
}

char *set_server() {
    return "nginx/1.11.8";
}

char *set_connection() {
    return "Close";
}

char *set_body(int status_code, char *file_path) {
    int file_size = 0;
    char *body = NULL;
    FILE *fp;
    switch (status_code) {
        case 200:
            fp = fopen(file_path, "r");
            fseek(fp, 0L, SEEK_END);
            file_size = ftell(fp);
            fseek(fp, 0L, SEEK_SET);
            body = (char *)malloc(file_size + 1);
            fread(body, 1, file_size, fp);
            body[file_size] = '\0';
            fclose(fp);

            return body;
            break;
        case 404:
        default:
            fp = fopen(ERR404, "r");
            fseek(fp, 0L, SEEK_END);
            file_size = ftell(fp);
            fseek(fp, 0L, SEEK_SET);
            body = (char *)malloc(file_size + 1);
            fread(body, 1, file_size, fp);
            fclose(fp);
            
            return body;
            break;
    }
}

void build_response(Response *p_response) {
    p_response->response = (char *)malloc(1000);
    sprintf(
        p_response->response, 
        "%s\r\nContent-Type: %s; charset=utf-8\r\nDate: %s GMT\r\nServer: %s\r\nConnection: %s\r\n\r\n%s",
        p_response->header, 
        p_response->content_type, 
        p_response->date, 
        p_response->server, 
        p_response->connection, 
        p_response->body
    );
}

void free_response(Response *response) {
    if (response) {
        // if (response->body)
        //     free(response->body);
        // if (response->connection)
        //     free(response->connection);
        if (response->content_type)
            free(response->content_type);
        if (response->date)
            free(response->date);
        if (response->header)
            free(response->header);
        // if (response->server)
        //     free(response->server);
        if (response->response)
            free(response->response);
        free(response);
    }
}

void print_response(Response *response) {
    printf("-----\n");
    LOG_INFO("Response Info: ");
    printf("%s\n\n", response->response);
}
