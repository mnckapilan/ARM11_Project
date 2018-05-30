//
// Created by Jordan Bunke on 2018-05-30.
//

#ifndef ARM11_09_EMULATOR_DATA_PROCESSING_H
#define ARM11_09_EMULATOR_DATA_PROCESSING_H

typedef struct cpsr {
    int n;
    int z;
    int c;
    int v;
} cpsr;

typedef enum {
    AND = 0,
    EOR = 1,
    SUB = 2,
    RSB = 3,
    ADD = 4,
    TST = 8,
    TEQ = 9,
    CMP = 10,
    ORR = 12,
    MOV = 13
} OPCODE;

typedef struct data_processing {
    u_int32_t rn;
    u_int32_t rd;
    u_int32_t op2;
    int i;
    int s;
} data_processing;

typedef struct emulator_state {
    data_processing *data_pro;
    u_int32_t *reg;
    uint8_t *memory;
    cpsr *cpsr_flags;
};

void execute_data_proc(emulator_state *st);

#endif //ARM11_09_EMULATOR_DATA_PROCESSING_H
