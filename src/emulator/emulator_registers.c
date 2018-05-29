#include "emulator_registers.h"

#include <stdio.h>
#include <stdint.h>

/* Need to create a function to write to a register */
void write_register(uint32_t *reg, uint32_t val)
{
    (*reg) = val;
}

/* Need to create a function to read from a register */
uint32_t read_register(uint32_t *reg)
{
    return (*reg);
}

/* Function to zero a register (XOR with itself) */
void clear_register(uint32_t *reg)
{
    (*reg) ^= (*reg);
}
