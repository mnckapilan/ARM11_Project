//
// Created by Sukant Roy on 13/06/2018.
//

#include "assembler_blockDataTransfer.h"

uint32_t block_data_transfer(instruction *ins) {

    uint32_t condition = COND << 28u;
    uint32_t p = (ins->p) << 24u;
    uint32_t u = (ins->u) << 23u;
    uint32_t l = 1;
    uint32_t rn = (ins->rn) << 16u;
    uint32_t s = (ins->s) << 22u;
    uint32_t w = (ins->w) << 21u;
    uint32_t regList = (uint32_t) abs(ins->regList);

    if((ins->phrase)[0] == 's') {
        l = 0;
    }

    return condition | (1 << 27u) | p | u | s | w | (l << 20u ) | rn | regList;
}