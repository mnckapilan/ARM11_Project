#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "emulator.h"

void multiply(PointerToBeCast base) {

    //cast to correct struct type
    MultiplyInstruction *i = (MultiplyInstruction *) base;
    //calculate and then move into destination
    u_int32_t acc = 0;
    if (i->acc) {
        acc = *i->acc;
    }
    *(i->des) = *i->op1 * *i->op2 + acc;
}
