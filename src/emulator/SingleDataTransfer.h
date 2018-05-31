//
// Created by Sukant Roy on 29/05/2018.
//

#ifndef SRC_SINGLEDATATRANSFER_H
#define SRC_SINGLEDATATRANSFER_H

#include <stdint.h>

#define COND_INDEX 28
#define I_INDEX 25
#define P_INDEX 24
#define UP_INDEX 23
#define L_INDEX 20
#define RN_INDEX 16
#define RD_INDEX 12

void transferData(struct CPUState cpu, uint32_t instruction, uint16_t memAddr);

uint32_t compute_memory_address(uint32_t baseRegValue, uint16_t offset, uint32_t instruction);

int checkConditionCode(uint32_t instruction, struct CPUState cpu);

uint32_t interpretShiftCode(uint8_t shiftTypeCode, uint32_t rMRegValue, uint8_t constantShiftAmount);

uint16_t interpret_offset_shifted_reg(struct CPUState cpu, uint32_t instruction);

#endif
