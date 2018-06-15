#ifndef EMULATOR_DATA_PROCESSING_H
#define EMULATOR_DATA_PROCESSING_H

#include <stdint.h>
#include "emulator.h"
#include "emulator_utilities.h"

#define I_OFFSET 25
#define OPCODE_OFFSET 21
#define OPCODE_SIZE 4
#define S_OFFSET 20
#define RN_OFFSET 16
#define RD_OFFSET 12
#define OPERAND2_ROTATE_SCALE 2
#define OPERAND2_IMM_OFFSET 0
#define OPERAND2_IMM_SIZE 8
#define OPERAND2_ROTATE_OFFSET 8
#define OPERAND2_ROTATE_SIZE 4
#define OPERAND2_RM_OFFSET 0
#define OPERAND2_SHIFT_OFFSET 4
#define OPERAND2_SHIFT_SIZE 1
#define OPERAND2_SHIFT_TYPE_OFFSET 5
#define OPERAND2_SHIFT_TYPE_SIZE 2
#define OPERAND2_SHIFT_MAG_OFFSET 7
#define OPERAND2_SHIFT_MAG_SIZE 5
#define OPERAND2_RS_OFFSET 8

typedef enum {
    AND = 0,
    EOR = 1,
    SUB = 2,
    RSB = 3,
    ADD = 4,
    TST = 8,
    TEQ = 9,
    CMP = 10,
    ORR = 12,
    MOV = 13
} Opcode;

typedef struct {
    uint32_t val;
    uint32_t cOut;
} Operand2;

typedef enum {
    LSL = 0,
    LSR = 1,
    ASR = 2,
    ROR = 3
} Shift;

Operand2 logical_shift_left(uint32_t val, uint32_t shiftMagnitude);

Operand2 logical_shift_right(uint32_t val, uint32_t shiftMagnitude);

Operand2 arithmetic_shift_right(uint32_t val, uint32_t shiftMagnitude);

Operand2 rotate_right(uint32_t val, uint32_t rotate);

Operand2 interpret_imm_operand(uint32_t val);

Operand2 interpret_reg_operand(uint32_t val, State cpu);

void dp_set_CPSR(uint32_t result, uint32_t cOut, State cpu);

void logical_op(uint32_t val1, uint32_t val2, uint32_t setCPSR, State cpu, uint32_t dest, uint32_t cOut, Opcode op);

void arithmetic_op(uint32_t val1, uint32_t val2, State cpu, uint32_t dest, uint32_t setCPSR, uint32_t writeResult, Opcode op);

uint32_t data_processing(uint32_t instr, State cpu);

#endif
