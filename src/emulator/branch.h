#ifndef BRANCH_H
#define BRANCH_H

#include <stdint.h>
#include "emulator.h"
#include "utilities.h"

#define BRANCH_OFFSET_START 0
#define BRANCH_OFFSET_SIZE 24
#define BRANCH_OFFSET_SIGN_BIT 25
#define BRANCH_OFFSET_SHIFT 2
#define BRANCH_SIGN_EXTEND 0xfc000000

uint32_t branch(uint32_t instr, State cpu);

#endif
