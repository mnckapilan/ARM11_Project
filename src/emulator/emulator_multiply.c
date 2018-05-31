#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "emulator_multiply.h"
#include "utilities.h"

void multiply(state *st) {
    multiply_instr *multiply = st->multiply_instruction;
    uint32_t result;

    result = (st->reg[multiply->rm] * st->reg[multiply->rs]);

    /*   if accumulate */
    if (st->multiply_instruction->acc) {
        result += (uint32_t) st->reg[multiply->rn];
    }
    st->reg[multiply->rd] = result;

    //update n and z flags
    st->cpsrFlag->n = bits_extract(result, 31, 32);
    st->cpsrFlag->z = 0;
    if (!result) {
        st->cpsrFlag->z = 1;
    }
}
