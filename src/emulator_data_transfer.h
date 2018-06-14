#ifndef DATA_TRANSFER_H
#define DATA_TRANSFER_H
#include "emulator_utilities.h"
#include "emulator.h"

#define REG_WIDTH 4
#define P_INDEX 24
#define UP_INDEX 23
#define L_INDEX 20
#define RN_INDEX 16

uint32_t transfer_data(State cpu, uint32_t memAddr, uint32_t loadStoreBit, uint32_t rdRegIndex);

#endif
