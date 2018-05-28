#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H

#include "emulator_registers.h"
#include "emulator_memory.h"

#define WORD_SIZE 32
#define NUM_REGISTERS 17
#define NUM_MEMORY_LOCATIONS 65536

typedef struct cpuState 
{
	memoryByte memory[NUM_MEMORY_LOCATIONS];
	reg regs[NUM_REGISTERS];	
} cpu;



#endif
