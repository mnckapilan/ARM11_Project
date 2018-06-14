//
// Created by Sukant Roy on 13/06/2018.
//

#ifndef ARM11_09_EMULATOR_BLOCKDATATRANSFER_H
#define ARM11_09_EMULATOR_BLOCKDATATRANSFER_H
#include "data_transfer.h"

#define REG_LIST_INDEX 0
#define REG_LIST_LENGTH 16
#define W_INDEX 21
#define NUM_BYTES

uint32_t execute_block_data_transfer(uint32_t instruction, State cpu);

uint32_t preIncrementAddressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList);

uint32_t postIncrementAddressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList);

uint32_t preDecrementAddressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList);

uint32_t postDecrementAddressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList);

#endif
