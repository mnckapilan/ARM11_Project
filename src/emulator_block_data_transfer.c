#include "emulator_block_data_transfer.h"

uint32_t pre_increment_addressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList) {
    uint32_t addr = baseRegValue;
    uint32_t regTransferBit;
    for (uint32_t i = 0; i < REG_LIST_LENGTH; i++) {
        regTransferBit = bits_extract(regList, i, i + 1);
        if (regTransferBit == 1) {
            addr += BYTES_PER_WORD;
            transfer_data(cpu, addr, lBit, i);
        }
    }
    return addr;
}

uint32_t post_increment_addressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList) {
    uint32_t addr = baseRegValue;
    uint32_t regTransferBit;
    for (uint32_t i = 0; i < REG_LIST_LENGTH; i++) {
        regTransferBit = bits_extract(regList, i, i + 1);
        if (regTransferBit == 1) {
            transfer_data(cpu, addr, lBit, i);
            addr += BYTES_PER_WORD;
        }
     }
      return addr;
}

uint32_t pre_decrement_addressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList) {
    uint32_t addr = baseRegValue;
    uint32_t regTransferBit;
    for (uint32_t i = REG_LIST_LENGTH - 1; i >= 0; i--) {
        regTransferBit = bits_extract(regList, i, i + 1);
        if (regTransferBit == 1) {
            addr -= BYTES_PER_WORD;
            transfer_data(cpu, addr, lBit, i);
        }
    }
    return addr;
}

uint32_t post_decrement_addressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList) {
    uint32_t addr = baseRegValue;
    uint32_t regTransferBit;
    for (uint32_t i = REG_LIST_LENGTH - 1; i >= 0; i--) {
        regTransferBit = bits_extract(regList, i, i + 1);
        if (regTransferBit == 1) {
            transfer_data(cpu, addr, lBit, i);
            addr -= BYTES_PER_WORD;
        }
    }
    return addr;
}

uint32_t block_data_transfer(uint32_t instruction, State cpu) {
    if (check_condition(instruction, cpu) == 0) {
        return 0;
    }
    uint32_t pBit = bits_extract(instruction, P_INDEX, P_INDEX + 1);
    uint32_t upBit = bits_extract(instruction, UP_INDEX, UP_INDEX + 1);
    uint32_t lBit = bits_extract(instruction, L_INDEX, L_INDEX + 1);
    uint32_t wBit = bits_extract(instruction, W_INDEX, W_INDEX + 1);
    uint32_t baseRegIndex = bits_extract(instruction, RN_INDEX, RN_INDEX + REG_WIDTH);
    uint32_t baseRegValue = read_from_register(cpu, baseRegIndex);
    uint32_t regList = bits_extract(instruction, REG_LIST_INDEX, REG_LIST_LENGTH);
    assert(baseRegIndex != 15);
    assert(regList > 0);
    uint32_t address;

    if ((pBit == 1) && (upBit == 1)) {
       address = pre_increment_addressing(cpu, lBit, baseRegValue, regList);
    } else if ((pBit == 0) && (upBit == 1)){
      address = post_increment_addressing(cpu, lBit, baseRegValue, regList);
    }
    else if ((pBit == 1) && (upBit == 0)){
        address = pre_decrement_addressing(cpu, lBit, baseRegValue, regList);
    }
    else if ((pBit == 0) && (upBit == 0)) {
        address = post_decrement_addressing(cpu, lBit, baseRegValue, regList);
    }
    if (wBit == 1) {
        write_to_register(cpu, baseRegIndex, address);
    }
    return 1;
}


