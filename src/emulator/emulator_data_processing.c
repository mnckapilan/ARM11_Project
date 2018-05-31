//
// Created by Jordan Bunke on 2018-05-30.
//

#include "emulator_data_processing.h"
#include "utilities.h"

void execute_data_processing(emulator_state *state) {
    data_processing *data_pro = state->data_pro;
    int carry_out = 0;
    uint32_t res = 0;
    uint32_t operand_2;

    if (data_pro->i) {
        /* operand_2 = data_pro->op2 % (2 ^ 8); */
        operand_2 = data_pro->op2 & MASK_IMM; //defined in emulator_data_processing.h
        int rot = (int) bits_extract(data_pro->op2, 8, 12) * 2;

        /* Each execution of the loop rotates operand2 right by a single bit. */
        for (int i = 0; i < rot; i++) {
            operand_2 = rotate_right(operand_2);
        }
    } else {

    }

    switch (data_pro->opcode) {
        case AND :
            state->reg[data_pro->rd] = state->reg[data_pro->rn] & operand_2;
            break;
        case EOR :
            state->reg[data_pro->rd] = state->reg[data_pro->rn] ^ operand_2;
            break;
        case SUB :
            state->reg[data_pro->rd] = state->reg[data_pro->rn] - operand_2;
            break;
        case RSB :
            state->reg[data_pro->rd] = operand_2 - state->reg[data_pro->rn];
            break;
        case ADD :
            state->reg[data_pro->rd] = state->reg[data_pro->rn] + operand_2;
            break;
        case TST :
            result = state->reg[data_pro->rn] & operand_2;
            break;
        case TEQ :
            result = state->reg[data_pro->rn] ^ operand_2;
            break;
        case CMP :
            result = state->reg[data_pro->rn] - operand_2;
            break;
        case ORR :
            state->reg[data_pro->rd] = state->reg[data_pro->rn] | operand_2;
            break;
        case MOV :
            state->reg[data_pro->rd] = operand_2;
            break;
    }

    if (data_pro->s) {
        switch (data_pro->opcode) {
            case MOV :
                break;
            case CMP :
                break;
            case RSB :
                break;
            case ADD :
                break;
        }
        setZ(state, res);
        setN(state, res);
    }
    return;
}

void setZ(emulator_state *state, uint32_t res) {
    state->cpsr_flags->z = 0;
    if (!res) {
        state->cpsr_flags->z = 1;
    }
}

void setN(emulator_state *state, uint32_t res) {
    /* state->cpsr_flags->n = res / (2 ^ 31); */
    state->cpsr_flags->n = bits_extract(res, 31, 32);
}

uint32_t rotate_right(uint32_t num) {
    if (num % 2) {
        num = (1 << 31) + (num >> 1);
    } else {
        num >>= 1;
    }
    return num;
}