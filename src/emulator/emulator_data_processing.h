//
// Created by Jordan Bunke on 2018-05-30.
//

#ifndef ARM11_09_EMULATOR_DATA_PROCESSING_H
#define ARM11_09_EMULATOR_DATA_PROCESSING_H

#define MASK_IMM 0x000000ff
#define MASK_BIT_FOUR 0x00000010

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

typedef enum shift_type {
    LSL = 0,
    LSR = 1,
    ASR = 2,
    ROR = 3
} SHIFT_TYPE;

typedef struct data_processing {
    OPCODE opcode;
    uint32_t rn;
    uint32_t rd;
    uint32_t op2;
    int i;
    int s;
} data_processing;


// clashes with implementation in CPUstate
typedef struct emulator_state {
    data_processing *data_pro;
    uint32_t *reg;
    uint8_t *memory;
    cpsr *cpsr_flags;
};

void set_z(emulator_state *state, uint32_t res);
void set_n(emulator_state *state, uint32_t res);
void set_c_add(emulator_state *state, uint32_t arg1, uint32_t arg2);
void set_c_sub(emulator_state *state, uint32_t arg1, uint32_t arg2);
void set_c(emulator_state *state, int carry_out);
uint32_t rotate_right(uint32_t num);
void execute_data_proc(emulator_state *state);

#endif //ARM11_09_EMULATOR_DATA_PROCESSING_H
