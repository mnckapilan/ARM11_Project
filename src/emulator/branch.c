#include "branch.h"

uint32_t branch(uint32_t instr, State cpu) {
    if (check_condition(instr, cpu) == 0) {
        return 0;
    }

    uint32_t unsignedOffset = instr & BRANCH_OFFSET_MASK;
    unsignedOffset <<= BRANCH_OFFSET_SHIFT;
    if ((unsignedOffset & BRANCH_SIGN_BIT_MASK) == BRANCH_SIGN_BIT_MASK) {
        unsignedOffset += BRANCH_SIGN_EXTEND;
    }

    int32_t branchOffset = unsignedOffset;
    int32_t PCVal = read_from_register(cpu, PC_INDEX) + branchOffset;

    if (PCVal >= 0) {
        write_to_register(cpu, PC_INDEX, PCVal);
    } else {
        printf("Attempted to branch to a negative value address.\n");
    }

    return 1;
}
