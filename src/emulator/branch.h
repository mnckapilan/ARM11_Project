#ifndef BRANCH_H
#define BRANCH_H

#include <stdint.h>
#include "emulator.h"
#include "utilities.h"

#define BRANCH_OFFSET_MASK 0x00ffffff
#define BRANCH_OFFSET_SHIFT 2
#define BRANCH_SIGN_BIT_MASK 0x02000000
#define BRANCH_SIGN_EXTEND 0xfc000000

uint32_t branch(uint32_t instr, State cpu);

#endif
