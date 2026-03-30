#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *filename);
void write_file(const char *filename, const char *content);

#endif // FILE_H