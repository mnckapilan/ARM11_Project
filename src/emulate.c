#include <stdlib.h>
#include <stdio.h>
#define NUM_EXPECTED_ARGS 2

int main(int argc, char **argv) {
	
	if (argc != NUM_EXPECTED_ARGS) {
	 	printf("The emulator takes exactly one file argument\n");
	  	return -1;	
	}

	FILE *binFile;
	binFile = fopen(argv[1], "rb");
	
	if (binFile == NULL) {
		printf("Error opening file %p\n", argv[1]);
		exit(1);
	}
	
	return 0;
}
