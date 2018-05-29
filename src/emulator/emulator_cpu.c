#include "emulator_cpu.h"
#include <stdlib.h>
#include <stdio.h>


struct CPUState initialize_CPU()
{
    uint8_t *memory = calloc(NUM_MEMORY_LOCATIONS, sizeof(uint8_t));

    for (int i = 0; i < NUM_MEMORY_LOCATIONS; i += WORD_SIZE / BYTE_SIZE)
        clear_word(&memory[i]);

    uint32_t *regs = calloc(NUM_REGISTERS, sizeof(uint32_t));

    for (int i = 0; i < NUM_REGISTERS; i++)
        clear_register(&regs[i]);


    struct CPUState cpu;
    cpu.memory = memory;
    cpu.regs = regs;

    return cpu;
}

uint8_t memory_in_bounds(uint16_t addr)
{
    if (addr >= (NUM_MEMORY_LOCATIONS - (WORD_SIZE / BYTE_SIZE))) {
        fprintf(stderr, "Attempting to access memory location out of bounds.");
        return 1;
    }

    return 0;
}

void write_to_memory(uint8_t *memory, uint16_t addr, uint32_t val)
{
    if (memory_in_bounds(addr) == 0)
        write_word(&memory[addr], val);
}

uint32_t read_from_memory(uint8_t *memory, uint16_t addr)
{
    if (memory_in_bounds(addr) == 0)
        return read_word(&memory[addr]);
}

uint8_t register_in_bounds(uint8_t Rn)
{
    if (Rn >= NUM_GEN_PURPOSE_REGISTERS) {
        fprintf(stderr, "Attempting to access special purpose register.");
        return 1;
    }

    return 0;
}

void write_to_register(uint32_t *regs, uint8_t Rn, uint32_t val)
{
    if (register_in_bounds(Rn) == 0)
        write_register(&regs[Rn], val);
}

uint32_t read_from_register(uint32_t *regs, uint8_t Rn)
{
    if (register_in_bounds(Rn) == 0)
        return read_register(&regs[Rn]);
}