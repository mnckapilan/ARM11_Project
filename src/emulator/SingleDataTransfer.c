//
// Created by Sukant Roy on 28/05/2018.
//

#include <stdint.h>
#include "SingleDataTransfer.h"
#include "emulator_cpu.h"

/* void
build_instruction(int instruction[], int cond[], int immediateFlag, int pIndexingBit, int upBit, int loadStoreBit,
                  int baseReg[], int srcDestReg[], int offset[]) {
// int instruction[32];
    const int baseRegOffset = 12;
    const int srcDestRegOffset = 16;
    const int offsetOffset = 20;

    for (int i = 0; i < 4; i++) {
        instruction[i] = cond[i];
    }

    instruction[4] = 0;
    instruction[5] = 1;
    instruction[6] = immediateFlag;
    instruction[7] = pIndexingBit;
    instruction[8] = upBit;
    instruction[9] = 0;
    instruction[10] = 0;
    instruction[11] = loadStoreBit;

    for (int j = 12; j < 16; j++) {
        instruction[j] = baseReg[j - baseRegOffset];
    }

    for (int k = 16; k < 20; k++) {
        instruction[k] = srcDestReg[k - srcDestRegOffset];
    }

    for (int n = 20; n < 32; n++) {
        instruction[n] = offset[n - offsetOffset];
    }

} */

/* void execute_instruction(int baseReg[], int srcDestRegister[], int instruction[]) {
  //  interpret_cond(instruction); implement condition checking before execution of instruction to determine whether
  //  instruction should be executed or ignored: look at Cond field (condition code) in instruction and then check whether
  // it succeeds, by looking at CSPR flags
    int immOffsetFlag = instruction[6];
    int pIndexing = instruction[7];
    int upBit = instruction[8];
    int lBit = instruction[11];
    int shiftedReg[12];
    uint32_t imm;
    const int baseRegOffset = 12;
    const int offsetOffset = 20;
    int offsetValue;
    int baseAddress = computeBaseAddress(
            baseReg); //effectively converts binary value stored in base register into decimal
    int memoryAddress;
    if (immOffsetFlag == 1) {
        shiftedReg = interpret_offset_shiftedReg(
                instruction + offsetOffset); //implement interpret_offset_shiftedReg function
        offsetValue = compute_offset_shifted_reg(shiftedReg); //to implement
    } else if (immOffsetFlag == 0) {
        imm = interpret_offset_immediate(instruction + offsetOffset); // implement interpret_offset_immediate function
        offsetValue = imm;
    }

    if (pIndexing == 1) {
        memoryAddress = compute_memory_address(baseAddress, offsetValue, upBit); // to implement
        transferData(memoryAddress, srcDestRegister, lBit); // to implement
    } else if (pIndexing == 0) {
        transferData(baseAddress, srcDestRegister, lBit); //
        memoryAddress = compute_memory_address(baseAddress, offsetValue, upBit);
        baseReg = asBinary(memoryAddress); //
        for (int i = 0; i < 4; i++) {
            instruction[i + baseRegOffset] = baseReg[i];
        }
    }
} */

/* void interpret_cond(int cond[4]) {

} */

/*
int compute_memory_address(int baseAddress, int offset, int upBit) {
    if (upBit == 1)
        return baseAddress + offset;
    else
        return baseAddress - offset;
}

void transferData(int address, int srcDestReg[], int loadStoreBit) {
    if (loadStoreBit == 1) {

    } else if (loadStoreBit == 0) {

    }
}

int computeBaseAddress(int baseRegister[]) {
    return -1;
}

int *interpret_offset_shiftedReg(int offset[]) {

}

int compute_offset_shifted_reg(int shiftedReg[]) {

}

uint12_t interpret_offset_immediate(int offset[]) {

}

int *asBinary(int memAddress) {

} */

void execute_instruction(uint32_t instruction, struct CPUState cpu) {
    uint32_t offset;
    //uint32_t unsignedOffset;
    if (checkConditionCode(instruction, cpu) == 0) {
        uint32_t immediateMask = 1 << I_INDEX;
        uint32_t immediateOffset = (instruction & immediatemask) >> I_INDEX;
        uint32_t pMask = 1 << P_INDEX;
        uint32_t pBit = (instruction & pMask) >> P_INDEX;
        if (immediateOffset == 1) {

            // TODO: CASE WHERE OFFSET IS A SHIFTED REGISTER
        } else if (immediateOffset == 0) {
            uint32_t bitMask = 4095;
            offset = instruction & bitMask;
        }
        if (pBit == 1) {

        }
       else if (pBit == 0) {

        }
    }

}

void interpret_offset_immediate(uint32_t instruction, struct CPUState cpu) {

}

void interpret_offset_shifted_register(uint32_t instruction, struct CPUState cpu) {

}

int checkConditionCode(uint32_t instruction, struct CPUState cpu) {
    uint32_t cpsrContents = read_from_register(cpu.regs, CPSR_INDEX);
    uint32_t statusFlagBits = cpsrContents >> 28;
    uint32_t cond = instruction >> COND_INDEX;
    if (statusFlagBits == cond) {
        return 0;
    }
    return 1;
}