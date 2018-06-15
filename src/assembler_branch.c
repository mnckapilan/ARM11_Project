//
// Created by Jordan Bunke on 2018-06-12.
//

#include "assembler_branch.h"

uint32_t branch(instruction *ins, uint32_t current_address) {
    uint32_t offset = ins->expression - current_address - PIPELINE;
    /* type is the 2 char string for the cond type */
    char type[2];
    type[0] = (ins->phrase)[1];
    type[1] = (ins->phrase)[2];
    uint32_t condition = getCond(type);

    return (condition << 28u) | (5u << 25u) | ((offset >> 2u) & 0x00ffffff);
}