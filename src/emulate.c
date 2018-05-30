#include <stdlib.h>
#include <stdio.h>
#include "emulator_io.h"

#define NUM_EXPECTED_ARGS 2
#define WORD_SIZE 32
#define NUM_REGISTERS 17
#define NUM_MEMORY_LOCATIONS 65536



int main(int argc, char **argv) {

    if (argc != NUM_EXPECTED_ARGS) {
        fprintf(stderr, "Emulator takes exactly one file as an argument.\n");
        exit(EXIT_FAILURE);
    }

    FILE *instrFile = NULL;

    if (load_file(argv[1], instrFile, NUM_MEMORY_LOCATIONS) != 0)
        exit(EXIT_FAILURE);
}

