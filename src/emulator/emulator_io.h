#ifndef EMULATOR_IO_H
#define EMULATOR_IO_H

#include <stdio.h>
#include <stdint.h>

FILE* open_file(char* path);

void close_file(FILE* path);

int32_t file_exists(char* path);

uint32_t get_size(FILE* file);

FILE* load_file(char* path, uint32_t memorySize);

#endif
