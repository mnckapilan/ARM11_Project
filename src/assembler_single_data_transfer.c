//
// Created by Jordan Bunke on 2018-06-08.
//

#include "assembler_single_data_transfer.h"

uint32_t single_data_transfer(instruction *ins) {
    /* Instruction flags and components are shifted into place and or'd to return
     * uint32_t instruction. */
    uint32_t condition = COND << 28u;
    uint32_t offset = (uint32_t) abs(ins->operand2);
    uint32_t rd = (ins->rd) << 12u;
    uint32_t rn = (ins->rn) << 16u;
    uint32_t imm = (ins->imm) << 25u;
    uint32_t p = (ins->p) << 24u;
    uint32_t u = (ins->u) << 23u;
    uint32_t l = 1;

    if ((ins->phrase)[0] == 's') {
        l = 0;
    }

    return condition | (1u << 26u) | imm | p | u
           | (l << 20u) | rn | rd | offset;
}