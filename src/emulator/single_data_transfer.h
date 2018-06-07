#ifndef SINGLEDATATRANSFER_H
#define SINGLEDATATRANSFER_H

#include <stdint.h>
#include "emulator.h"
#include "utilities.h"

#define REG_WIDTH 4
#define OFFSET_WIDTH 12
#define RS_INDEX 8
#define SHIFT_TYPE_INDEX 5
#define SHIFT_TYPE_WIDTH 2
#define CONST_5BIT_INDEX 7
#define I_INDEX 25
#define P_INDEX 24
#define UP_INDEX 23
#define L_INDEX 20
#define RN_INDEX 16
#define RD_INDEX 12
#define OFFSET_INDEX 0
#define RM_INDEX 0
#define BIT4_INDEX 4
#define BYTE_WIDTH 8
#define NOBITS 32

uint32_t interpret_shift_code(uint32_t shiftTypeCode, uint32_t rMRegValue, uint32_t constantShiftAmount);

uint32_t interpret_offset_shifted_reg(State cpu, uint32_t instruction);

uint32_t transferData(State cpu, uint32_t instruction, uint32_t memAddr);

uint32_t compute_memory_address(uint32_t baseRegValue, uint32_t offset, uint32_t instruction);

uint32_t single_data_transfer(uint32_t instruction, State cpu);

#endif
