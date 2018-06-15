//
// Created by Jordan Bunke on 2018-06-12.
//

#ifndef ARM11_09_ASSEMBLER_BRANCH_H
#define ARM11_09_ASSEMBLER_BRANCH_H

#include "assembler_utilities.h"
#include <stdint.h>

#define PIPELINE 8

uint32_t branch(instruction *ins, uint32_t current_address);
uint32_t branch_with_link(instruction *ins, uint32_t current_address);

#endif //ARM11_09_ASSEMBLER_BRANCH_H
