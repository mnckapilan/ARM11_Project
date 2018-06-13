#include "halt.h"

/*
 * Takes a value and a maximum number of spaces to be printed.
 * Prints more spaces if the number has fewer digits.
 */
void pad_with_spaces(uint32_t val, uint32_t max_no_spaces) {

    uint32_t num_spaces = max_no_spaces;

    if (val == 0) {
        num_spaces--; 
    } else if (bits_extract(val, SIGN_BIT, SIGN_BIT + BIT_SIZE) == 1) { /* If the number is negative */
        val -= 1;                                                       /* Convert to positive equivalent */
        val = ~(val);
        if (bits_extract(val, SIGN_BIT, SIGN_BIT + BIT_SIZE) == 0) {    /* If the resulting number is now positive, remove a space */
            num_spaces--;
        }
    }

    while ((val > 0) && (num_spaces > 0)) {                             /* Keep dividing by 10 for each digit encountered and remove a space */
        num_spaces--;
        val /= BASE_10_POWER;
    }

    for (uint32_t i = 0; i < num_spaces; i++) {                         /* Print spaces */
        printf(" ");
    }
}

/* Prints a index and prints its value */
void print_register(uint32_t regIndex, State cpu) {
    uint32_t val = read_from_register(cpu, regIndex);

    printf(" ");
    pad_with_spaces(val, MAX_NUM_DIGITS_PER_WORD);

    printf("%i (0x%08x)\n", val, val);               /* Print the signed integer value of the register contents and the hex value, with 8 digits */
}

/* Prints all the general purpose registers, the PC and CPSR */
void print_all_registers(State cpu) {
    printf("Registers:\n");

    for (uint32_t i = 0; i < NUM_GEN_PURPOSE_REGISTERS; i++) {
        printf("$%u", i);
        pad_with_spaces(i, MAX_NUM_REG_INDEX_SPACES);
        printf(":");
        print_register(i, cpu);
    }

    printf("PC  :");
    print_register(PC_INDEX, cpu);

    printf("CPSR:");
    print_register(CPSR_INDEX, cpu);
}

/* 
 * Memory is stored in a little endian order in the memory, but it is retrieved in words
 * constructed into its 32 bit value. This function is needed to break that value back down into 
 * bytes.
 */
void print_little_endian(uint32_t val) {                                        
    printf("0x");
    for (uint32_t i = 0; i < BYTES_PER_WORD; i++) {
        uint8_t valToPrint = (uint8_t) bits_extract(val, i * BYTE_SIZE, (i + 1) * BYTE_SIZE);  /* Iterates over every byte in the word */
        printf("%02x", valToPrint);                                                            /* Using the bits extract to extract each byte */
    }
}

/* 
 * Obtains the value of each memory location and if it is non zero, its 
 * value is printed.
 */
void print_non_zero_memory(State cpu) {
    printf("Non-zero memory:\n");

    for (uint32_t i = 0; i < NUM_MEMORY_LOCATIONS; i = i + BYTES_PER_WORD) {

        uint32_t val = read_from_memory(cpu, i);

        if (val != 0) {
            printf("0x%08x: ", i);
            print_little_endian(val);
            printf("\n");
        }
    }
}

/* Calls the routines to print the registers and non zero memory locations */
uint32_t halt(uint32_t instr, State cpu) {
    print_all_registers(cpu);
    print_non_zero_memory(cpu);
    return 0;
}
