#include "emulator_block_data_transfer.h"

/* In pre-increment addressing, the address is incremented by the offset of 4 before the transfer of each register.
 * If R15 (PC) is stored to memory, the stored value is the address of the STM instruction plus 12 (applicable to any of
 * the addressing modes). Each bit in the register list corresponds to a register (1 means transfer, 0 means do not
 * transfer). For increment addressing (pre or post), an up to for loop is used. The registers are transferred in the order
 * lowest to highest for increment addressing mode. The lowest register gets transferred to or from the lowest memory address
 * (for any addressing mode).
 */
uint32_t pre_increment_addressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList) {
    uint32_t addr = baseRegValue;
    uint32_t regTransferBit;
    for (uint32_t i = 0; i < REG_LIST_LENGTH; i++) {
        regTransferBit = bits_extract(regList, i, i + 1);
        if (regTransferBit == 1) {
            addr += BYTES_PER_WORD;
            if ((lBit == 0) && (i == PC_INDEX)) {
                uint32_t stmAddress = read_from_register(cpu, PC_INDEX);
                write_to_memory(cpu, addr, stmAddress + BYTES_PER_WORD);
            }
            else {
                transfer_data(cpu, addr, lBit, i);
            }
        }
    }
    return addr;
}

// In post-increment addressing, the address is incremented by the offset after the transfer of each register.
uint32_t post_increment_addressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList) {
    uint32_t addr = baseRegValue;
    uint32_t regTransferBit;
    for (uint32_t i = 0; i < REG_LIST_LENGTH; i++) {
        regTransferBit = bits_extract(regList, i, i + 1);
        if (regTransferBit == 1) {
            if ((lBit == 0) && (i == PC_INDEX)) {
                uint32_t stmAddress = read_from_register(cpu, PC_INDEX);
                write_to_memory(cpu, addr, stmAddress + BYTES_PER_WORD);
            }
            else {
                transfer_data(cpu, addr, lBit, i);
            }
            addr += BYTES_PER_WORD;
        }
    }
    return addr;
}

/* In pre-decrement addressing, the address is decremented by the offset before the transfer of each register. For
 *  decrement addressing mode (pre/post), down to for loop is used. Registers are transferred in the order highest to lowest.
 */
uint32_t pre_decrement_addressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList) {
    uint32_t addr = baseRegValue;
    uint32_t regTransferBit;
    for (uint32_t i = REG_LIST_LENGTH - 1; i >= 0; i--) {
        regTransferBit = bits_extract(regList, i, i + 1);
        if (regTransferBit == 1) {
            addr -= BYTES_PER_WORD;
            if ((lBit == 0) && (i == PC_INDEX)) {
                uint32_t stmAddress = read_from_register(cpu, PC_INDEX);
                write_to_memory(cpu, addr, stmAddress + BYTES_PER_WORD);
            }
            else {
                transfer_data(cpu, addr, lBit, i);
            }
        }
    }
    return addr;
}

// In post-decrement addressing, the address is decremented by the offset after the transfer of each register.
uint32_t post_decrement_addressing(State cpu, uint32_t lBit, uint32_t baseRegValue, uint32_t regList) {
    uint32_t addr = baseRegValue;
    uint32_t regTransferBit;
    for (uint32_t i = REG_LIST_LENGTH - 1; i >= 0; i--) {
        regTransferBit = bits_extract(regList, i, i + 1);
        if (regTransferBit == 1) {
            if ((lBit == 0) && (i == PC_INDEX)) {
                uint32_t stmAddress = read_from_register(cpu, PC_INDEX);
                write_to_memory(cpu, addr, stmAddress + BYTES_PER_WORD);
            }
            else {
                transfer_data(cpu, addr, lBit, i);
            }
            addr -= BYTES_PER_WORD;
        }
    }
    return addr;
}

/* R15 (PC) should not been used as the base register in any LDM or STM instruction. The register list must be non-empty.
 * Any subset of the 16 registers or all of them may be specified in the register list.
 * The wBit is the write back bit, which determines whether write back of the modified base is required or not. An address
 * is written to the base register if wBit equals 1. If wBit equals 0, the base register retains its original value,
 * unless it was also in the transfer (register) list of a LDM instruction - overwritten by new loaded value. S bit is
 * ignored as PSR register is ignored. The registers are transferred in the order lowest to highest. The lowest register
 * gets transferred to or from the lowest memory address (for any addressing mode).
 */
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


