//
// Created by Sukant Roy on 13/06/2018.
//

#ifndef ARM11_09_DATA_TRANSFER_H
#define ARM11_09_DATA_TRANSFER_H
#include "utilities.h"
#include "emulator.h"


#define REG_WIDTH 4
#define P_INDEX 24
#define UP_INDEX 23
#define L_INDEX 20
#define RN_INDEX 16

uint32_t transferData(State cpu, uint32_t memAddr, uint32_t loadStoreBit, uint32_t rdRegIndex);


#endif
