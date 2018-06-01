//
// Created by Kapilan M on 31/05/2018.
//

#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdint.h>
#include "emulator_cpu.h"

#define CPSR_MASK 0xf0000000
#define PC_PIPELINE_OFFSET 8
#define CPSR_N 0x80000000
#define CPSR_Z 0x40000000
#define CPSR_C 0x20000000
#define CPSR_V 0x10000000

uint32_t bits_extract(uint32_t value, int begin, int end);

/* Returns 1 if the condition is satisfied, 0 otherwise */
uint32_t check_condition(uint32_t instr, struct CPUState cpu, uint32_t mask);

uint32_t check_any_condition(uint32_t instr, struct CPUState cpu);

#endif
