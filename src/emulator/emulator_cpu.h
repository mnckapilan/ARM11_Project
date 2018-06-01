#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H

#include <stdio.h>
#include "emulator_registers.h"
#include "emulator_memory.h"
#include "emulator_constants.h"

struct CPUState
{
	uint8_t *memory;
	uint32_t *regs;
};

struct CPUState initialize_CPU();

void write_to_memory(struct CPUState cpu, uint16_t addr, uint32_t val);

uint32_t read_from_memory(struct CPUState cpu, uint16_t addr);

void write_to_register(struct CPUState cpu, uint8_t Rn, uint32_t val);

uint32_t read_from_register(struct CPUState cpu, uint8_t Rn);

void incrementPC(struct CPUState cpu);

uint32_t getPC(struct CPUState cpu);

void read_instructions(FILE* file, struct CPUState cpu);

#endif
