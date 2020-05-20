#include "router.h"

char *router(char *path, char *arg) {
    // Parameter* parameter = NULL;
    Parameter* parameter = argument_parse(arg);
    
    if ((!strcmp(path, "/")) || !strcmp(path, "/index") || !strcmp(path, "/index\n")) {
        return self_index(parameter);
    } else if (!strcmp(path, "/upload")) {
        return upload(parameter);
    } else if (!strcmp(path, "/view")) {
        return view(parameter);
    } else {
        return self_index(parameter);
    }

    return self_index(parameter);
}