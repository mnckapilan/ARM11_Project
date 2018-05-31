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
    AND = 0, /* rn AND op2 */
    EOR = 1, /* rn EOR op2 */
    SUB = 2, /* rn - op2 */
    RSB = 3, /* op2 - rn */
    ADD = 4, /* rn + op2 */
    TST = 8, /* AND but not written */
    TEQ = 9, /* EOR but not written */
    CMP = 10, /* SUB but not written */
    ORR = 12, /* rn OR op2 */
    MOV = 13 /* op2 */
} OPCODE;

typedef struct data_processing {
    OPCODE opcode;
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

void execute_data_proc(emulator_state *state);

#endif //ARM11_09_EMULATOR_DATA_PROCESSING_H