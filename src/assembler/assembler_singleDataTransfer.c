//
// Created by Jordan Bunke on 2018-06-08.
//

#include "assembler_singleDataTransfer.h"

uint32_t single_data_transfer(instruction *ins) {
    uint32_t condition = Cond;
    uint32_t offset = ins->operand2;
    uint32_t rd = ins->rd;
    uint32_t rn = ins->rn;
    uint32_t p = (ins->p) << P;
    uint32_t l = 1;

    if ((ins->phrase)[0] == 's') {
        l = 0;
    }

    /* Instruction flags and components are shifted into place and or'd to return
     * uint32_t instruction. */
    return p | (1 << 26) | (rd << 12) | (condition << 28) | abs(offset)
            | (l << 20) | (rn << 16) | (ins->u ? U : 0) | ((ins->imm == 0) << 25);
}