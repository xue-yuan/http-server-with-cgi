#include <stdio.h>

#include "utils.h"

const char *BASE_DIR = "./"; // TODO: Use function to get abs path
const char *STATIC_DIR = "./static";

inline void LOG_INFO(char *message) {
    printf("[!] %s\n", message);
}

inline void LOG_SUCCESS(char *message) {
    printf("[+] %s\n", message);
}

inline void LOG_WAIT(char *message) {
    printf("[*] %s\n", message);
}

inline void LOG_FAILED(char *message) {
    printf("[x] %s\n", message);
}

inline void exception(int boolean, char *success, char *failed) {
    if (boolean) {
        LOG_SUCCESS(success);
    } else {
        LOG_FAILED(failed);
    }
}
