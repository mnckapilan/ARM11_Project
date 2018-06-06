#include "halt.h"

void pad_with_spaces(int32_t val, uint32_t max_no_spaces) {

    uint32_t num_spaces = 0;

    if (val == 0) {
        val++;
    } else if (val < 0) {
        num_spaces++;
        val = -val;
    }

    while (val > 0) {
        num_spaces++;
        val /= BASE_10_POWER;
    }

    num_spaces = max_no_spaces - num_spaces;

    for (uint32_t i = 0; i < num_spaces; i++) {
        fprintf(stdout, " ");
    }
}

void print_register(uint32_t regIndex, State cpu) {
    uint32_t val = read_from_register(cpu, regIndex);

    pad_with_spaces(val, MAX_NUM_DIGITS_PER_WORD + 1);

    fprintf(stdout, "%i (0x%08x)\n", val, val);
}

void print_all_registers(State cpu) {
    fprintf(stdout, "Registers:\n");

    for (uint32_t i = 0; i < NUM_GEN_PURPOSE_REGISTERS; i++) {
        fprintf(stdout, "$%u", i);
        pad_with_spaces(i, MAX_NUM_REG_INDEX_SPACES);
        fprintf(stdout, ":");
        print_register(i, cpu);
    }

    fprintf(stdout, "PC  :");
    print_register(PC_INDEX, cpu);

    fprintf(stdout, "CPSR:");
    print_register(CPSR_INDEX, cpu);
}

void print_little_endian(uint32_t val) {
    fprintf(stdout, "0x");
    for (uint32_t i = 0; i < BYTES_PER_WORD; i++) {
        uint8_t valToPrint = (uint8_t) bits_extract(val, i * BYTE_SIZE, (i + 1) * BYTE_SIZE);
        fprintf(stdout, "%02x", valToPrint);
    }
}

void print_non_zero_memory(State cpu) {
    fprintf(stdout, "Non-zero memory:\n");

    for (uint32_t i = 0; i < NUM_MEMORY_LOCATIONS; i = i + BYTES_PER_WORD) {

        uint32_t val = read_from_memory(cpu, i);

        if (val != 0) {
            fprintf(stdout, "0x%08x: ", i);
            print_little_endian(val);
            fprintf(stdout, "\n");
        }
    }
}

uint32_t halt(uint32_t instr, State cpu) {
    print_all_registers(cpu);
    print_non_zero_memory(cpu);
    return 0;
}
