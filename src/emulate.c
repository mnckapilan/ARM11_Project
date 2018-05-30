#include <stdlib.h>
#include <stdio.h>
#include "emulator/emulator_io.h"
#include "emulator/emulator_cpu.h"
#include "emulator/emulator_registers.h"
#include "emulator/emulator_memory.h"
#include "emulator_io.h"

#define NUM_EXPECTED_ARGS 2

int main(int argc, char **argv) {

    if (argc != NUM_EXPECTED_ARGS) {
        fprintf(stderr, "Emulator takes exactly one file as an argument.\n");
        exit(EXIT_FAILURE);
    }

	if (load_file(argv[1], instrFile, NUM_MEMORY_LOCATIONS) != 0)
		exit(EXIT_FAILURE);

	struct CPUState cpu = initialize_CPU();
	
	exit (EXIT_SUCCESS);
}

