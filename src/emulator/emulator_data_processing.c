//
// Created by Jordan Bunke on 2018-05-30.
//

#include "emulator_data_processing.h"
#include "utilities.h"
#include "emulator_cpu.c"

void execute_data_processing(emulator_state *state) {
    data_processing *data_pro = state->data_pro;
    int carry_out = 0;
    uint32_t res = 0;
    uint32_t operand_2;
    shift_type shift;

    if (data_pro->i) {
        /* operand_2 = data_pro->op2 % (2 ^ 8); */
        operand_2 = data_pro->op2 & MASK_IMM; //defined in emulator_data_processing.h
        int rot = (int) bits_extract(data_pro->op2, 8, 12) * 2;

        /* Each execution of the loop rotates operand2 right by a single bit. */
        for (int i = 0; i < rot; i++) {
            operand_2 = rotate_right(operand_2);
        }
    } else if (!(MASK_BIT_FOUR & data_pro->op2)) {
        shift = bits_extract(data_pro->op2, 5, 7);
        int shift_amount = (int) bits_extract(data_pro->op2, 7, 12);
        uint32_t rm = bits_extract(data_pro->op2, 0, 4);
        switch (shift) {
            case LSL :
                operand_2 = state->reg[rm] << shift_amount;
                carry_out = (int) bits_extract(state->reg[rm], 32 - shift_amount, (32 - shift_amount) + 1);
                break;
            case LSR :
                operand_2 = state->reg[rm] >> shift_amount;
                carry_out = (int) bits_extract(state->reg[rm], 0, shift_amount);
                break;
            case ASR :
                operand_2 = state->reg[rm] >> shift_amount;
                for (int i = 0; i < shift_amount; i++) {
                    operand_2 += (2 ^ (31 - i));
                }
                carry_out = (int) bits_extract(state->reg[rm], 0, shift_amount);
                break;
            case ROR :
                for (int i = 0; i < shift_amount; i++) {
                    operand_2 = rotate_right(operand_2);
                }
                carry_out = (int) bits_extract(state->reg[rm], 0, shift_amount);
                break;
        }
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
            case AND : case EOR : case ORR : case TEQ : case TST : case MOV :
                set_c(state, carry_out);
                break;
            case SUB : case CMP :
                set_c_sub(state, operand_2, state->reg[data_pro->rn]);
                break;
            case RSB :
                set_c_sub(state, state->reg[data_pro->rn], operand_2);
                break;
            case ADD :
                set_c_add(state, state->reg[data_pro->rn] , operand_2);
                break;
        }
        set_z(state, res);
        set_n(state, res);
    }
    return;
}

void set_z(emulator_state *state, uint32_t res) {
    state->cpsr_flags->z = 0;
    if (!res) {
        state->cpsr_flags->z = 1;
    }
}

void set_n(emulator_state *state, uint32_t res) {
    /* state->cpsr_flags->n = res / (2 ^ 31); */
    state->cpsr_flags->n = bits_extract(res, 31, 32);
}

void set_c_add(emulator_state *state, uint32_t arg1, uint32_t arg2) {
    state->cpsr_flags->c = 0;

    if (arg1 + arg2 > 0xffffffff) {
        state->cpsr_flags->c = 1;
    }
}

void set_c_sub(emulator_state *state, uint32_t arg1, uint32_t arg2) {
    state->cpsr_flags->c = 0;

    if (arg2 >= arg1) {
        state->cpsr_flags->c = 1;
    }
}

void set_c(emulator_state *state, int carry_out) {
    state->cpsr_flags->c = carry_out;
}

uint32_t rotate_right(uint32_t num) {
    if (num % 2) {
        num = (1 << 31) + (num >> 1);
    } else {
        num >>= 1;
    }
    return num;
}