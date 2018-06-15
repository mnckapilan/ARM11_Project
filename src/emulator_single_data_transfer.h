#ifndef SINGLEDATATRANSFER_H
#define SINGLEDATATRANSFER_H
#include "emulator_data_transfer.h"

#define OFFSET_WIDTH 12
#define RS_INDEX 8
#define RD_INDEX 12
#define SHIFT_TYPE_INDEX 5
#define SHIFT_TYPE_WIDTH 2
#define CONST_5BIT_INDEX 7
#define I_INDEX 25
#define OFFSET_INDEX 0
#define RM_INDEX 0
#define BIT4_INDEX 4
#define BYTE_WIDTH 8
#define NOBITS 32

uint32_t interpret_shift_code(uint32_t shiftTypeCode, uint32_t rMRegValue, uint32_t constantShiftAmount);

uint32_t interpret_offset_shifted_reg(State cpu, uint32_t instruction);

uint32_t compute_memory_address(uint32_t baseRegValue, uint32_t offset, uint32_t instruction);

uint32_t single_data_transfer(uint32_t instruction, State cpu);

#endif
