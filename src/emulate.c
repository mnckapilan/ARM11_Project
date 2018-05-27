#include <stdlib.h>
#include <stdio.h>
#include "emulator/emulator_io.h"

#define NUM_EXPECTED_ARGS 2
#define WORD_SIZE 32
#define NUM_REGISTERS 17
#define NUM_MEMORY_LOCATIONS 65536



int main(int argc, char **argv) {
	
	FILE* instrFile;

	if (load_file(argv[1], instrFile, NUM_MEMORY_LOCATIONS) != 0)
		exit(EXIT_FAILURE);
}

