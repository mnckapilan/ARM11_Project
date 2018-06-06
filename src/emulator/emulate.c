#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"

#define NUM_EXPECTED_ARGS 2

int main(int argc, char **argv) {

    if (argc != NUM_EXPECTED_ARGS) {
        fprintf(stderr, "Emulator takes exactly one file as an argument.\n");
        exit(EXIT_FAILURE);
    }

    FILE *instrFile = load_file(argv[1], NUM_MEMORY_LOCATIONS);

    if (instrFile == NULL) {
        exit(EXIT_FAILURE);
    }

    State cpu = initialize_CPU();

    read_instructions(instrFile, cpu);

    close_file(instrFile);

    run_emulator(cpu);

    free_emulator(cpu);

    exit(EXIT_SUCCESS);
}

