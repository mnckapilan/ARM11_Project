//
// Created by Kapilan M on 30/05/2018.
//

#ifndef ARM11_09_EMULATOR_MULTIPLY_H
#define ARM11_09_EMULATOR_MULTIPLY_H

#include <ntsid.h>

typedef void *PointerToBeCast;
// functions to apply shifting
typedef u_int32_t (*Shifter)(u_int32_t*, u_int32_t, u_int32_t, u_int32_t);
// typedef of execute function with dummy pointer arg
typedef void (*Execute)(PointerToBeCast);

#define BASE
u_int8_t cond;
u_int32_t * cpsr;
Execute function;

// BASE
// Defines the two key components of every decoded
// instruction
// the cond flag that is required to be checked on execution and the function to call on
// execution, which will take the address of the struct itself, fixing the base pointer from where it can
// fetch it's arguments

typedef struct
{
    BASE
    u_int32_t *op1;
    u_int32_t *op2;
    u_int32_t *acc;
    u_int32_t *des;
    u_int8_t     s;
} MultiplyInstruction;


#endif //ARM11_09_EMULATOR_MULTIPLY_H

