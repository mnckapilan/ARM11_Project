#ifndef EMULATOR_CPU_H
#define EMULATOR_CPU_H

#include <stdio.h>
#include <stdlib.h>
#include "emulator_io.h"
#include "emulator_registers.h"
#include "emulator_memory.h"
#include "emulator_multiply.h"
#include "emulator_branch.h"
#include "emulator_halt.h"
#include "emulator_single_data_transfer.h"
#include "emulator_data_processing.h"
#include "emulator_utilities.h"

#define DATA_PROCESSING_MASK 0x0c000000
#define MULTIPLY_MASK 0x0fc000f0
#define SINGLE_DATA_TRANSFER_MASK 0x0c600000
#define BRANCH_MASK 0x0e000000
#define DATA_PROCESSING_FORMAT 0x00000000
#define MULTIPLY_FORMAT 0x00000090
#define SINGLE_DATA_TRANSFER_FORMAT 0x04000000
#define BRANCH_FORMAT 0x0a000000
#define HALT_MASK 0x00000000
#define HALT_FORMAT 0x00000000

#define GPIO_PINS_0_9 0x20200000
#define GPIO_PINS_10_19 0x20200004
#define GPIO_PINS_20_29 0x20200008
#define GPIO_CLEAR_ADDR 0x20200028
#define GPIO_SET_ADDR 0x2020001c


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
