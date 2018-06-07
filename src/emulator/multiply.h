#ifndef EMULATOR_MULTIPLY_H
#define EMULATOR_MULTIPLY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "utilities.h"
#include "emulator.h"

#define MULT_ACC_INDEX 21
#define MULT_SET_INDEX 20
#define MULT_RD_INDEX 16
#define MULT_RN_INDEX 12
#define MULT_RS_INDEX 8
#define MULT_RM_INDEX 0

void mul_set_CPSR(int32_t val, State cpu);

uint32_t multiply(uint32_t instr, State cpu);

#endif
