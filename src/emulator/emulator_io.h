#ifndef EMULATOR_IO_H
#define EMULATOR_IO_H

#include <stdio.h>
#include <stdint.h>

FILE* open_file(char* path);

void close_file(FILE* path);

uint8_t file_exists(char* path);

uint32_t get_file_size(FILE* file);

uint8_t load_file(char* path, FILE* file, uint32_t memorySize);

#endif
