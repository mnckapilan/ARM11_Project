//
// Created by Kapilan M on 31/05/2018.
//

#include "utilities.h"

//  takes bits between begin and end --> [begin,end)
uint32_t bits_extract(uint32_t value, int begin, int end)
{
    uint32_t mask = (1 << (end - begin)) - 1;
    return (value >> begin) & mask;
}

/* Returns 1 if the condition is satisfied, 0 otherwise */
uint32_t check_condition(uint32_t instr, struct CPUState cpu, uint32_t mask)
{
    if ((read_from_register(cpu, CPSR_INDEX) & mask) == (instr & mask))
        return 1;

    return 0;
}

uint32_t check_any_condition(uint32_t instr, struct CPUState cpu)
{
    return check_condition(instr, cpu, CPSR_N) | check_condition(instr, cpu, CPSR_Z)
            | check_condition(instr, cpu, CPSR_C) | check_condition(instr, cpu, CPSR_V);
}


