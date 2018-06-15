//
// Created by Jordan Bunke on 2018-06-14.
//

#include "assembler_special.h"

uint32_t assembler_special(instruction *ins) {
    uint32_t res;

    if (!strcmp(ins->phrase, "lsl")) {
        uint32_t  shift_value = ins->operand2;
        uint32_t operand2 = ins->rd;
        shift_value <<= 7;
        operand2 |= shift_value;
        ins->imm = 0;
        ins->operand2 = operand2;
        ins->phrase = "mov";
        res = assembler_dataProcessing(ins);
    }

    return res;
}