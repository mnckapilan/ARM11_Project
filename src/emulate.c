#include <stdlib.h>
#include <stdio.h>
#include "emulator/emulator_io.h"
#include "emulator/emulator_cpu.h"
#include "emulator/emulator_registers.h"
#include "emulator/emulator_memory.h"

#define NUM_EXPECTED_ARGS 2

int main(int argc, char **argv) {
	
	if (argc != NUM_EXPECTED_ARGS) {
		fprintf(stderr, "Emulator takes exactly one file as an argument.\n");
		exit(EXIT_FAILURE);
	}

	FILE* instrFile = load_file(argv[1], NUM_MEMORY_LOCATIONS);

	if (instrFile == NULL)
		exit(EXIT_FAILURE);

	struct CPUState cpu = initialize_CPU();

	read_instructions(instrFile, cpu);
	
	return 0;
}

