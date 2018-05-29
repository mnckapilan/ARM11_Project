/*
 * Created by Kapilan M on 26/05/2018.
 * Purpose: to read the binary file input from specified path
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "emulator_header.h"

FILE* open_file(char* path)
{
    FILE* file = fopen(path, "rb");
    return file;
}

void close_file(FILE* file)
{
    fclose(file);
}


/*
 * F_OK is a parameter passed to the access method to indicate we're checking to see
 * if the file exists.
 * The access function returns 0 if the file exists and -1 otherwise.
 */
uint8_t file_exists(char* path)
{
    return access (path, F_OK);
}


/*
 * Finds the end of the file, stores this position, restores the file position to the
 * beginning and returns the position which is the end of the file.
 */
uint32_t get_size(FILE* file)
{
    fseek(file, 0, SEEK_END);
    uint32_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

uint8_t load_file(char* path, FILE* file, uint32_t memorySize)
{
	if (file_exists(path) != 0) {
			fprintf(stderr, "Specified file path doesn't exist.\n");
			return 1;
	}

	file = open_file(path);

	if (get_size(file) > memorySize) {
			fprintf(stderr, "File is too large to store its contents in memory.\n");
	 		close_file(file);
			return 1;
	}

	return 0;
}
