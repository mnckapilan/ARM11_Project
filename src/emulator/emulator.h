#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "io.h"
#include "registers.h"
#include "memory.h"
#include "multiply.h"
#include "branch.h"
#include "halt.h"
#include "single_data_transfer.h"
#include "data_processing.h"

#define DATA_PROCESSING_MASK 0x0c000000
#define MULTIPLY_MASK 0x0fc000f0
#define SINGLE_DATA_TRANSFER_MASK 0x0c600000
#define BRANCH_MASK 0x0f000000
#define DATA_PROCESSING_FORMAT 0x00000000
#define MULTIPLY_FORMAT 0x00000090
#define SINGLE_DATA_TRANSFER_FORMAT 0x04000000
#define BRANCH_FORMAT 0x0a000000
#define HALT_MASK 0x00000000
#define HALT_FORMAT 0x00000000

typedef uint32_t (*Instr_ptr)(uint32_t, State);

typedef struct {
    uint8_t fetchedEmpty;
    uint8_t decodedEmpty;
    uint32_t fetched;
    uint32_t decodedInstr;
    Instr_ptr decoded;
} Stage;

typedef enum {
    EQ = 0,
    NE = 1,
    GE = 10,
    LT = 11,
    GT = 12,
    LE = 13,
    AL = 14
} ConditionCode;

State initialize_CPU();

uint8_t memory_in_bounds(uint32_t addr);

void write_to_memory(State cpu, uint32_t addr, uint32_t val);

uint32_t read_from_memory(State cpu, uint32_t addr);

uint8_t register_in_bounds(uint32_t Rn);

void write_to_register(State cpu, uint32_t Rn, uint32_t val);

uint32_t read_from_register(State cpu, uint32_t Rn);

void increment_PC(State cpu);

uint32_t get_next_instruction(State cpu);

uint32_t get_Z(uint32_t val);

uint32_t get_N(uint32_t val);

uint32_t get_V(uint32_t val);

uint32_t check_condition(uint32_t instr, State cpu);

void read_instructions(FILE *file, State cpu);

Instr_ptr decode(uint32_t instr);

void run_emulator(State cpu);

void free_emulator(State cpu);

#endif
