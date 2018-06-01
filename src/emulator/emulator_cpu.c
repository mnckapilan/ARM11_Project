#include "emulator_cpu.h"
#include "emulator_io.h"
#include <stdlib.h>
#include <stdio.h>

struct Instruction {
    uint8_t b0, b1, b2, b3;
};

/*
 * This allocates the memory and registers in the heap dynamically. The calloc() function initialises the allocated
 * space to 0 so there is no need to do this manually.
 */
struct CPUState initialize_CPU()
{
    uint8_t *memory = calloc(NUM_MEMORY_LOCATIONS, sizeof(uint8_t));

    uint32_t *regs = calloc(NUM_REGISTERS, sizeof(uint32_t));

    struct CPUState cpu;
    cpu.memory = memory;
    cpu.regs = regs;

    return cpu;
}

uint8_t memory_in_bounds(uint16_t addr)
{
    if (addr >= (NUM_MEMORY_LOCATIONS - BYTES_PER_WORD)) {
        fprintf(stderr, "Attempting to access memory location out of bounds.");
        return 1;
    }

    return 0;
}

void write_to_memory(struct CPUState cpu, uint16_t addr, uint32_t val)
{
    if (memory_in_bounds(addr) == 0)
        write_word(&cpu.memory[addr], val);
}

uint32_t read_from_memory(struct CPUState cpu, uint16_t addr)
{
    if (memory_in_bounds(addr) == 0)
        return read_word(&cpu.memory[addr]);
}

uint8_t register_in_bounds(uint8_t Rn)
{
    if (Rn >= NUM_GEN_PURPOSE_REGISTERS) {
        fprintf(stderr, "Attempting to access special purpose register.");
        return 1;
    }

    return 0;
}

void write_to_register(struct CPUState cpu, uint8_t Rn, uint32_t val)
{
    if (register_in_bounds(Rn) == 0)
        write_register(&cpu.regs[Rn], val);
}

uint32_t read_from_register(struct CPUState cpu, uint8_t Rn)
{
    if (register_in_bounds(Rn) == 0)
        return read_register(&cpu.regs[Rn]);
}

void increment_PC(struct CPUState cpu)
{
    cpu.regs[PC_INDEX] += BYTES_PER_WORD;
}

uint32_t get_next_instruction(struct CPUState cpu)
{
    return read_register(&cpu.regs[PC_INDEX]);
}

void read_instructions(FILE* file, struct CPUState cpu)
{
    uint16_t i;
    struct Instruction instr;
    uint32_t size = get_size(file) / BYTES_PER_WORD;

    for (i = 0; i < size; i++)
    {
        fread(&instr, sizeof(struct Instruction), 1, file);
        write_to_memory(cpu, i * BYTES_PER_WORD, create_word_from_bytes(&instr.b0));
    }
}
