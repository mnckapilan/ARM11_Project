#include <stdlib.h>
#include <stdio.h>
#include "io.h"
#include "utilities.h"

#define NO_EXPECTED_ARGS 3

int main(int argc, char **argv) {
    if (argc != NO_EXPECTED_ARGS) {
        fprintf(stderr, "Assembler takes exactly two files as arguments.\n");
        exit(EXIT_FAILURE);
    }
    char* sourceFilePath = argv[1];
    char* binaryFilePath = argv[2];

    FILE* sourceFile = load_file(sourceFilePath, NUM_MEMORY_LOCATIONS);
    FILE* binaryOutputFile = load_file(binaryFilePath, NUM_MEMORY_LOCATIONS);
    if (sourceFile == NULL || binaryOutputFile == NULL) {
        exit(EXIT_FAILURE);
    }


    exit(EXIT_SUCCESS);
}
