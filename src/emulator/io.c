#include "io.h"

FILE *open_file(char *path) {
    FILE *file = fopen(path, "rb");
    return file;
}

void close_file(FILE *file) {
    fclose(file);
}

/* 
 * F_OK is a parameter passed to the access method to indicate we're checking to see
 * if the file exists.
 * The access function returns 0 if the file exists and -1 otherwise.
 */
int32_t file_exists(char *path) {
    return access(path, F_OK);
}

/*
 * Finds the end of the file, stores this position, restores the file position to the
 * beginning and returns the position which is the end of the file.
 */
uint32_t get_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    uint32_t size = (uint32_t) ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

FILE *load_file(char *path, uint32_t memorySize) {
    if (file_exists(path) != 0) {
        printf("Error: Specified file path doesn't exist.\n");
        return NULL;
    }

    FILE *file = open_file(path);

    if (get_size(file) > memorySize) {
        printf("Error: File is too large to store its contents in memory.\n");
        close_file(file);
        return NULL;
    }

    return file;
}
