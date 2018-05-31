//
// Created by Kapilan M on 30/05/2018.
//

#ifndef ARM11_09_EMULATOR_MULTIPLY_H
#define ARM11_09_EMULATOR_MULTIPLY_H

#include <ntsid.h>

typedef struct multiply_instr {
    int set;
    int acc;
    uint32_t rd; // destination
    uint32_t rn; // operand
    uint32_t rs; // operand
    uint32_t rm; // operand
    uint32_t operand2;
} multiply_instr;

typedef struct state {
    multiply_instr *multiply_instruction;
    uint32_t *reg;
    uint8_t *memory;
} state;

#endif
