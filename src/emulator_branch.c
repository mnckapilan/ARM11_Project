#include "emulator_branch.h"

/*
 * The instruction is executed if the CPSR condition code is satisfied by
 * the most significant nibble of the instruction.
 * The offset is extracted form the instruction, bit 23 is extracted
 * and the value of this bit is copied into bits 24 to 31 (sign extend).
 * The Program Counter is updated if the offset does not make its value negative.
 */
uint32_t branch(uint32_t instr, State cpu) {
    if (check_condition(instr, cpu) == 0) {
        return 0;
    }

    uint32_t offset = bits_extract(instr, BRANCH_OFFSET_START, BRANCH_OFFSET_START + BRANCH_OFFSET_SIZE);
    offset <<= BRANCH_OFFSET_SHIFT;
    
    if (bits_extract(offset, BRANCH_OFFSET_SIGN_BIT, BRANCH_OFFSET_SIGN_BIT + BIT_SIZE) == 1) {
        offset |= BRANCH_SIGN_EXTEND;
    }

    uint32_t PCVal = read_from_register(cpu, PC_INDEX) + offset;

    if (bits_extract(PCVal, SIGN_BIT, SIGN_BIT + BIT_SIZE) == 0) {
        write_to_register(cpu, PC_INDEX, PCVal);
    } else {
        printf("Attempted to branch to a negative value address.\n");
    }

    return 1;
}
