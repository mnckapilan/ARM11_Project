//
// Created by Jordan Bunke on 2018-06-12.
//

#include "assembler_branch.h"

uint32_t branch(instruction *ins, uint32_t current_address) {
    int offset = ins->expression - current_address - PIPELINE;
    /* type is the 2 char string for the cond type */
    char type[2];
    type[0] = (ins->phrase)[1];
    type[1] = (ins->phrase)[2];
    uint32_t condition = getCond(type);

    return (condition << 28u) | (5u << 25u) | ((offset >> 2u) & 0x00ffffff);
}

uint32_t branch_with_link(instruction *ins, uint32_t current_address) {
    int offset = ins->expression - current_address - PIPELINE;

    char type[2];
    type[0] = (ins->phrase)[1];
    type[1] = (ins->phrase)[2];
    uint32_t condition = getCond(type);

    // link bit flag encoded
    return (condition << 28u) | (5u << 25u) | (1u << 24u) |
           ((offset >> 2u) & 0x00ffffff);
}