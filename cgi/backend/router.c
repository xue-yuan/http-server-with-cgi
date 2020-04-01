#include "router.h"

char *router(char *arg) {
    if ((!strcmp(arg, "/")) || !strcmp(arg, "/index") || !strcmp(arg, "/index\n")) {
        return self_index();
    } else if (!strcmp(arg, "/upload")) {
        return upload();
    } else if (!strcmp(arg, "/view")) {
        return view();
    }

    return "";
}