#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H

#include "emulator_registers.h"
#include "emulator_memory.h"
#include "emulator_constants.h"

struct CPUState
{
	uint8_t *memory ;
	uint32_t *regs;
};

struct CPUState initialize_CPU();

void write_to_memory(uint8_t *memory, uint16_t addr, uint32_t val);

uint32_t read_from_memory(uint8_t *memory, uint16_t addr);

void write_to_register(uint32_t *regs, uint8_t Rn, uint32_t val);

uint32_t read_from_register(uint32_t *regs, uint8_t Rn);

#endif
1