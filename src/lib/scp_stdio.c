#include "scp.stdio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print(const char* message) {
    printf("%s", message);
}

void println(const char* message) {
    printf("%s\n", message);
}

void read(char* buffer, int size) {
    fgets(buffer, size, stdin);
    // 移除末尾的换行符
    int len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
}

char* readline() {
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // 移除末尾的换行符
        int len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        return strdup(buffer);
    }
    return NULL;
}

void println_int(int value) {
    printf("%d\n", value);
}