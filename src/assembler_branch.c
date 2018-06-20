//
// Created by Jordan Bunke on 2018-06-12.
//

#include "assembler_branch.h"

uint32_t branch(instruction *ins, uint32_t current_address) {
    int offset = ins->expression - current_address - PIPELINE;
    /* type is the 2 char string for the cond type */
    char type[2];

    uint32_t link_flag = 0;

    if ((ins->phrase)[1] == 'l') {
        link_flag = 1;
    }

    type[0] = (ins->phrase)[2 - link_flag];
    type[1] = (ins->phrase)[3 - link_flag];
    uint32_t condition = getCond(type);

    return (condition << 28u) | (5u << 25u) | (link_flag << 24u) |
           ((offset >> 2u) & 0x00ffffff);
}