#ifndef EMULATOR_HALT_H
#define EMULATOR_HALT_H

#include <stdint.h>
#include <stdio.h>
#include "emulator.h"

#define BASE_10_POWER 10
#define MAX_NUM_REG_INDEX_SPACES 3
#define MAX_NUM_DIGITS_PER_WORD 10

void pad_with_spaces(uint32_t val, uint32_t max_no_spaces);

void print_register(uint32_t regIndex, State cpu);

void print_all_registers(State cpu);

void print_non_zero_memory(State cpu);

uint32_t halt(uint32_t instr, State cpu);

#endif
