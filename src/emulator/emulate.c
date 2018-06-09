#include <stdlib.h>
#include <stdio.h>
#include "emulator.h"

#define NUM_EXPECTED_ARGS 2 /* Program itself + file argument */

int main(int argc, char **argv) {

    if (argc != NUM_EXPECTED_ARGS) {
        printf("Emulator takes exactly one file as an argument.\n");
        return 1;
    }

    FILE *instrFile = load_file(argv[1], NUM_MEMORY_LOCATIONS);

    /* If file doesn't exist -- exit with non zero status code */
    if (instrFile == NULL) {
        return 1;
    }

    State cpu = initialize_CPU(); /* Start emulator with zero memory and registers */

    read_instructions(instrFile, cpu); /* Load the instructions into memory */

    close_file(instrFile);

    run_emulator(cpu); /* Run the execution pipeline */

    free_emulator(cpu); /* Free dynamically allocated memory in the emulator */

    return 0;
}

