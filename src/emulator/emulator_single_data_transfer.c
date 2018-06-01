#include <stdint.h>
#include "emulator_single_data_transfer.h"

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

#define COND_INDEX 28
#define I_INDEX 25
#define P_INDEX 24
#define UP_INDEX 23
#define L_INDEX 20
#define RN_INDEX 16
#define RD_INDEX 12

uint32_t interpretShiftCode(uint8_t shiftTypeCode, uint32_t rMRegValue, uint8_t constantShiftAmount) {
    uint32_t result;
    if (shiftTypeCode == 0) {
        return rMRegValue << constantShiftAmount;
    }
    else if (shiftTypeCode == 1) {
        return rMRegValue >> constantShiftAmount;
    }

    else if (shiftTypeCode == 2) {
        int32_t signedRmRegValue = rMRegValue;
        result = signedRmRegValue >> constantShiftAmount;
        return result;
    }
    else if (shiftTypeCode == 3) {
        int32_t signedRmRegValue = rMRegValue;
        result = (signedRmRegValue >> constantShiftAmount) | (signedRmRegValue << (noBits - constantShiftAmount));
        return result;
    }
}

uint16_t interpret_offset_shifted_reg(struct CPUState cpu, uint32_t instruction) {
    uint8_t rMRegIndex = instruction & rM_Mask;
    uint32_t rMRegValue = read_from_register(cpu, rMRegIndex);
    uint8_t bit4 = (instruction & bit4Mask) >> 4;
    uint8_t shiftTypeCode = (instruction & shiftTypeMask) >> 5;
    uint32_t offset_32bit;
    uint16_t offset;
    if (bit4 == 1) {
        uint8_t rSRegIndex = (instruction & rSRegMask) >> 8;
        uint32_t rSRegValue = read_from_register(cpu, rSRegIndex);
        uint8_t shiftAmount = rSRegValue & bottomByteMask;
        offset_32bit = interpretShiftCode(shiftTypeCode, rMRegValue, shiftAmount);
    }
    else if (bit4 == 0) {
        uint8_t constantShiftAmount = (instruction & const5bitShiftMask) >> 7;
        offset_32bit = interpretShiftCode(shiftTypeCode, rMRegValue, constantShiftAmount);
    }
    if (offset_32bit < NUM_MEMORY_LOCATIONS) {
       offset = offset_32bit;
    }
    return offset;
}

void transferData(struct CPUState cpu, uint32_t instruction, uint16_t memAddr) {
    uint8_t lBit = (instruction & lMask) >> L_INDEX;
    uint8_t rdRegIndex = (instruction & rdMask) >> RD_INDEX;
    uint32_t memWord;
    if (lBit == 1) {
        memWord = read_from_memory(cpu, memAddr);
        write_to_register(cpu, rdRegIndex, memWord);
    }
    else if (lBit == 0) {
        memWord = read_from_register(cpu, rdRegIndex);
        write_to_memory(cpu, memAddr, memWord);
    }
}

uint32_t compute_memory_address(uint32_t baseRegValue, uint16_t offset, uint32_t instruction) {
    uint8_t upBit = (instruction & upMask) >> UP_INDEX;
    if (upBit == 1) {
       return (baseRegValue + offset);
    }
    else if (upBit == 0) {
       return (baseRegValue - offset);
    }
}

int checkConditionCode(uint32_t instruction, struct CPUState cpu) {
    uint32_t cpsrContents = read_from_register(cpu, CPSR_INDEX);
    uint8_t statusFlagBits = cpsrContents >> 28;
    uint8_t cond = instruction >> COND_INDEX;
    if (statusFlagBits == cond) {
        return 0;
    }
    return 1;
}

void single_data_transfer(uint32_t instruction, struct CPUState cpu) {
    uint16_t offset;
    if (checkConditionCode(instruction, cpu) == 0) {
        uint8_t immediateOffset = (instruction & immediateMask) >> I_INDEX;
        uint8_t pBit = (instruction & pMask) >> P_INDEX;
        uint8_t baseRegIndex = (instruction & baseRegMask) >> RN_INDEX;
        uint32_t baseRegValue = read_from_register(cpu, baseRegIndex);
        if (immediateOffset == 1) {
            offset = interpret_offset_shifted_reg(cpu, instruction);
        } else if (immediateOffset == 0) {
            offset = instruction & offsetBitMask;
        }
        if (pBit == 1) {
            uint32_t address = compute_memory_address(baseRegValue, offset, instruction);
            if (address < NUM_MEMORY_LOCATIONS) {
                uint16_t memoryAddress = address;
                transferData(cpu, instruction, memoryAddress);
            }
        }
        else if (pBit == 0) {
            if (baseRegValue < NUM_MEMORY_LOCATIONS) {
                uint16_t memAddress = baseRegValue;
                transferData(cpu, instruction, memAddress);
                uint32_t address = compute_memory_address(baseRegValue, offset, instruction);
                write_to_register(cpu, baseRegIndex, address);
            }
        }
    }
}