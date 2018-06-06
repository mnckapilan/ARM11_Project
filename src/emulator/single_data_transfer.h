#ifndef SINGLEDATATRANSFER_H
#define SINGLEDATATRANSFER_H

#include <stdint.h>
#include "emulator.h"

#define immediateMask 0x02000000
#define pMask 0x01000000
#define baseRegMask 0x000F0000
#define rM_Mask 0x0000000F
#define bit4Mask 0x00000010
#define const5bitShiftMask 0x00000F80
#define offsetBitMask 0x00000FFF
#define lMask 0x00100000
#define rdMask 0x00001000
#define upMask 0x00800000
#define shiftTypeMask 0x00000060
#define noBits 32
#define rSRegMask 0x00000F00
#define bottomByteMask 0x000000FF

#define I_INDEX 25
#define P_INDEX 24
#define UP_INDEX 23
#define L_INDEX 20
#define RN_INDEX 16
#define RD_INDEX 12

uint32_t interpretShiftCode(uint8_t shiftTypeCode, uint32_t rMRegValue, uint8_t constantShiftAmount);

uint16_t interpret_offset_shifted_reg(State cpu, uint32_t instruction);

void transferData(State cpu, uint32_t instruction, uint16_t memAddr);

uint32_t compute_memory_address(uint32_t baseRegValue, uint16_t offset, uint32_t instruction);

uint32_t single_data_transfer(uint32_t instruction, State cpu);

#endif
