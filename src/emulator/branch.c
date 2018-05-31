#include "branch.h"
#include "instruction_constants.h"

#define BRANCH_OFFSET_MASK 0x00ffffff
#define BRANCH_OFFSET_SHIFT 2
#define BRANCH_SIGN_BIT_MASK 0x02000000
#define BRANCH_SIGN_EXTEND 0xfc000000

uint32_t check_condition(uint32_t instr, struct CPUState cpu)
{
    if ((read_from_register(cpu, CPSR_INDEX) & CPSR_MASK) == (instr & CPSR_MASK))
        return 0;

    return 1;
}

void branch(uint32_t instr, struct CPUState cpu)
{
    if (check_condition(instr, cpu))
    {
        uint32_t unsignedOffset = instr & BRANCH_OFFSET_MASK;
        unsignedOffset <<= BRANCH_OFFSET_SHIFT;
        if ((unsignedOffset & BRANCH_SIGN_BIT_MASK) == BRANCH_SIGN_BIT_MASK)
            unsignedOffset += BRANCH_SIGN_EXTEND;

        int32_t branchOffset = unsignedOffset;
        int32_t PCVal = read_from_register(cpu, PC_INDEX) + branchOffset;

        if (PCVal >= 0)
            write_to_register(cpu, PC_INDEX, PCVal);
        else
            fprintf(stderr, "Attempted to branch to a negative address.");
    }
}
