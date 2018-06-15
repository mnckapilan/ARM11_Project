#ifndef EMULATOR_BLOCKDATATRANSFER_H
#define EMULATOR_BLOCKDATATRANSFER_H
#include "emulator_data_transfer.h"

#define REG_LIST_INDEX 0
#define REG_LIST_LENGTH 16
#define W_INDEX 21

uint32_t pre_increment_addressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList);

uint32_t post_increment_addressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList);

uint32_t pre_decrement_addressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList);

uint32_t post_decrement_addressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList);

uint32_t execute_block_data_transfer(uint32_t instruction, State cpu);

#endif
