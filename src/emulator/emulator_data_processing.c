//
// Created by Jordan Bunke on 2018-05-30.
//

#include "emulator_data_processing.h"

void execute_data_processing(emulator_state *state) {
    data_processing *data_pro = state->data_pro;
    u_int32_t result = 0;
    u_int32_t operand_2;

    if (data_pro->i) {
        operand_2 = data_pro->op2 % (2 ^ 8);
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
}
