#include "single_data_transfer.h"

/* 
 * This function decodes the shift type bits in the 32 bit instruction (bits 5 and 6) for the case in which the offset is
 * interpreted as a shifted register (I bit equals 1). It applies a shift operation to the value held in register Rm, with
 * a specified shift amount (unsigned), and returns the result. The shift operation type is specified by the shift type
 * code (0 - logical left shift, 1 - logical right shift, 2 - arithmetic right shift, 3 - rotate right).
 */
uint32_t interpret_shift_code(uint32_t shiftTypeCode, uint32_t rMRegValue, uint32_t constantShiftAmount) {
    uint32_t result = 0;
    if (shiftTypeCode == 0) {

        result = rMRegValue << constantShiftAmount;

    } else if (shiftTypeCode == 1) {

        result = rMRegValue >> constantShiftAmount;

    } else if (shiftTypeCode == 2) {

        int32_t signedRmRegValue = rMRegValue;
        result = signedRmRegValue >> constantShiftAmount;

    } else if (shiftTypeCode == 3) {

        int32_t signedRmRegValue = rMRegValue;
        result = (signedRmRegValue >> constantShiftAmount) | (signedRmRegValue << (NOBITS - constantShiftAmount)); // change this implementation
                // and work with unsigned values/ints (uint32_t)
    }

    return result;
}

/*
 * This function interprets offset as a shifted register instead of an immediate offset (I bit = 1). A shift operation,
 * specified by shift type bits, is applied to value of register rM. The shift amount depends on the value of bit 4 in
 * from the offset. If bit 4 equals 0, the shift value is specified by a constant amount - 8 bit unsigned int. If bit 4
 * equals 1, the shift amount is specified by another register, rS. The bottom byte of rS specifies the shift amount.
 */

uint32_t interpret_offset_shifted_reg(State cpu, uint32_t instruction) {
    uint32_t rMRegIndex = bits_extract(instruction, RM_INDEX, RM_INDEX + REG_WIDTH);
    uint32_t rMRegValue = read_from_register(cpu, rMRegIndex);
    uint32_t bit4 = bits_extract(instruction, BIT4_INDEX, BIT4_INDEX + 1);
    uint32_t shiftTypeCode = bits_extract(instruction, SHIFT_TYPE_INDEX, SHIFT_TYPE_INDEX + SHIFT_TYPE_WIDTH);
    uint32_t offset_32bit = 0;

    if (bit4 == 1) {
        uint32_t rSRegIndex = bits_extract(instruction, RS_INDEX, RS_INDEX + REG_WIDTH);
        uint32_t rSRegValue = read_from_register(cpu, rSRegIndex);
        uint32_t shiftAmount = bits_extract(rSRegValue, 0, BYTE_WIDTH);;
        offset_32bit = interpret_shift_code(shiftTypeCode, rMRegValue, shiftAmount);
    } else if (bit4 == 0) {
        uint32_t constantShiftAmount = bits_extract(instruction, CONST_5BIT_INDEX, CONST_5BIT_INDEX + 5);
        offset_32bit = interpret_shift_code(shiftTypeCode, rMRegValue, constantShiftAmount);
    }
    return offset_32bit;
}

/* 
 * Computes memory address to based on value of up bit. Uses value stored in base register Rn and previously computed offset
 * value. Offset is either added or subtracted from base register.
 */
uint32_t compute_memory_address(uint32_t baseRegValue, uint32_t offset, uint32_t instruction) {
    uint32_t upBit = bits_extract(instruction, UP_INDEX, UP_INDEX + 1);

    if (upBit == 1) {
        return (baseRegValue + offset);
    }
    return (baseRegValue - offset);
}

/* 
 * Main function for executing single data transfer instruction. Computes unsigned offset - if I bit (immediateOffset)
 * equals 1, offset is interpreted as a shifted register and its value is accordingly computed, otherwise offset
 * interpreted as an immediate value. Value of base register is retrieved using baseRegIndex. If pBit equals 1 (pre-indexing),
 * the offset is added/subtracted to the base register before transferring the data, otherwise (post-indexing), offset is
 * added/subtracted to base register after transferring. Pre-indexing does not change value of base register, post-indexing
 * does (by the offset).
 */
uint32_t single_data_transfer(uint32_t instruction, State cpu) {

    uint32_t offset = 0;
    if (check_condition(instruction, cpu) == 0) {
        return 0;
    }
    uint32_t lBit = bits_extract(instruction, L_INDEX, L_INDEX + 1);
    uint32_t immediateOffset = bits_extract(instruction, I_INDEX, I_INDEX + 1);
    uint32_t pBit = bits_extract(instruction, P_INDEX, P_INDEX + 1);
    uint32_t baseRegIndex = bits_extract(instruction, RN_INDEX, RN_INDEX + REG_WIDTH);
    uint32_t baseRegValue = read_from_register(cpu, baseRegIndex);
    uint32_t rdRegIndex = bits_extract(instruction, RD_INDEX, RD_INDEX + REG_WIDTH);

    if (immediateOffset == 1) {
        offset = interpret_offset_shifted_reg(cpu, instruction);
    } else if (immediateOffset == 0) {
        offset = bits_extract(instruction, OFFSET_INDEX, OFFSET_INDEX + OFFSET_WIDTH);
    }

    if (pBit == 1) {
        uint32_t address = compute_memory_address(baseRegValue, offset, instruction);
        transferData(cpu, address, lBit, rdRegIndex);
        }
     else {
        transferData(cpu, baseRegValue, lBit, rdRegIndex);
        uint32_t address = compute_memory_address(baseRegValue, offset, instruction);
        write_to_register(cpu, baseRegIndex, address);
    }
    return 1;
}
