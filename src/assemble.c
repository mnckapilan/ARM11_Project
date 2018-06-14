#include <stdlib.h>
#include "emulator/io.h"
#include "emulator/utilities.h"
#include "assembler_utilities.h"

#define NO_EXPECTED_ARGS 3

void run_assembler(FILE *source, FILE *bin_output);

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

    run_assembler(sourceFile, binaryOutputFile);

    exit(EXIT_SUCCESS);
}

void set_instruction(instruction *ins, char line[511], uint32_t *res,
                     uint32_t current_address, ST *symbol_table) {

}

void run_assembler(FILE *source, FILE *bin_output) {
    instruction *ins = malloc(sizeof(instruction));
    ST *symbol_table = malloc(sizeof(ST));

    int i = 0, *num = &i, no_lines;
    uint32_t current_address = 0;
    char *data, *save, *label;

    data = ""; //TODO

    if (data != NULL) {
        free(data);
    }

    label = malloc(511 * sizeof(char));

    char **array = init_2d_array(1, 511);
}