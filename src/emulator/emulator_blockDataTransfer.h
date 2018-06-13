//
// Created by Sukant Roy on 13/06/2018.
//

#ifndef ARM11_09_EMULATOR_BLOCKDATATRANSFER_H
#define ARM11_09_EMULATOR_BLOCKDATATRANSFER_H
#include "single_data_transfer.h"

#define REG_LIST_INDEX 0
#define W_INDEX 21
#define S_INDEX 22

uint32_t execute_block_data_transfer(uint32_t instruction, State cpu);

void execute_LDM(uint32_t instruction, State cpu);

void execute_STM(uint32_t instruction, State cpu);

#endif
