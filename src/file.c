#include "../include/file.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    char *content = malloc(file_size + 1);
    if (!content) {
        perror("malloc");
        fclose(file);
        return NULL;
    }
    fread(content, 1, file_size, file);
    content[file_size] = '\0';
    fclose(file);
    return content;
}

void write_file(const char *filename, const char *content) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("fopen");
        return;
    }
    fputs(content, file);
    fclose(file);
}