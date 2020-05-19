#include "request.h"

Request *parse_request(char *raw_request) {
    Request *request = (Request *)malloc(sizeof(Request));
    if (!request) {
        return NULL;
    }

    size_t method_len = strcspn(raw_request, " ");
    if (memcmp(raw_request, S_GET, method_len) == 0 ) {
        request->method = GET;
    } else if (memcmp(raw_request, S_POST, method_len) == 0) {
        request->method = POST;
    } else {
        request->method = UNDEFINED;
    }
    raw_request += method_len + 1;

    size_t uri_len = strcspn(raw_request, " ");
    request->uri = (char *)malloc(uri_len + 1);
    if (!request->uri) {
        free_request(request);
        return NULL;
    }
    memcpy(request->uri, raw_request, uri_len);
    request->uri[uri_len] = '\0';
    raw_request += uri_len + 1;

    size_t version_len = strcspn(raw_request, "\r\n");
    request->version = (char *)malloc(version_len + 1);
    if (!request->version) {
        free_request(request);
        return NULL;
    }
    memcpy(request->version, raw_request, version_len);
    request->version[version_len] = '\0';
    raw_request += version_len + 2;

    Header *headers = NULL;
    Header *next_header = NULL;
    while (raw_request[0]!='\r' && raw_request[1]!='\n') {
        next_header = headers;
        headers = (Header *)malloc(sizeof(Header));
        if (!headers) {
            free_request(request);
            return NULL;
        }

        size_t name_len = strcspn(raw_request, ":");
        headers->name = (char *)malloc(name_len + 1);
        if (!headers->name) {
            free_request(request);
            return NULL;
        }
        memcpy(headers->name, raw_request, name_len);
        headers->name[name_len] = '\0';
        raw_request += name_len + 2;

        size_t value_len = strcspn(raw_request, "\r\n");
        headers->value = (char *)malloc(value_len + 1);
        if (!headers->value) {
            free_request(request);
            return NULL;
        }
        memcpy(headers->value, raw_request, value_len);
        headers->value[value_len] = '\0';
        raw_request += value_len + 2;
        headers->next_header = next_header;
    }
    request->headers = headers;
    raw_request += 2;

    size_t body_len = strlen(raw_request);
    request->body = (char *)malloc(body_len + 1);
    memcpy(request->body, raw_request, body_len);
    request->body[body_len] = '\0';

    return request;
}

void free_header(Header *header) {
    if (header) {
        if (header->name)
            free(header->name);
        if (header->value)
            free(header->value);
        free_header(header->next_header);
        free(header);
    }
}

void free_request(Request *request) {
    if (request) {
        if (request->uri)
            free(request->uri);
        if (request->version)
            free(request->version);
        if (request->body)
            free(request->body);
        free_header(request->headers);
        free(request);
    }
}

void print_request(Request *request) {
    printf("-----\n");
    LOG_INFO("Request Info: ");
    printf("Method: %d\n", request->method);
    printf("URI: %s\n", request->uri);
    printf("HTTP-Version: %s\n", request->version);
    printf("Other Header: \n");
    for (Header *h=request->headers; h!=NULL; h=h->next_header) {
        printf("\t%s: %s\n", h->name, h->value);
    }
    printf("Body: %s\n\n", request->body);
}
