#include "single_data_transfer.h"

/* This function decodes the shift type bits in the 32 bit instruction (bits 5 and 6) for the case in which the offset is
 * interpreted as a shifted register (I bit equals 1). It applies a shift operation to the value held in register Rm, with
 * a specified shift amount (unsigned), and returns the result. The shift operation type is specified by the shift type
 * code (0 - logical left shift, 1 - logical right shift, 2 - arithmetic right shift, 3 - rotate right).
 */
uint32_t interpretShiftCode(uint8_t shiftTypeCode, uint32_t rMRegValue, uint8_t constantShiftAmount) {
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
        result = (signedRmRegValue >> constantShiftAmount) | (signedRmRegValue << (NOBITS - constantShiftAmount));
    }

    return result;
}

/* This function interprets offset as a shifted register instead of an immediate offset (I bit = 1). A shift operation,
 * specified by shift type bits, is applied to value of register rM. The shift amount depends on the value of bit 4 in
 * from the offset. If bit 4 equals 0, the shift value is specified by a constant amount - 8 bit unsigned int. If bit 4
 * equals 1, the shift amount is specified by another register, rS. The bottom byte of rS specifies the shift amount.
 */

uint16_t interpret_offset_shifted_reg(State cpu, uint32_t instruction) {
    uint8_t rMRegIndex = bits_extract(instruction, RM_INDEX, RM_INDEX + REG_WIDTH);
    uint32_t rMRegValue = read_from_register(cpu, rMRegIndex);
    uint8_t bit4 = bits_extract(instruction, BIT4_INDEX, BIT4_INDEX + 1);
    uint8_t shiftTypeCode = bits_extract(instruction, SHIFT_TYPE_INDEX, SHIFT_TYPE_INDEX + SHIFT_TYPE_WIDTH);
    uint32_t offset_32bit = 0;
    uint16_t offset = 0;

    if (bit4 == 1) {
        uint8_t rSRegIndex = bits_extract(instruction, RS_INDEX, RS_INDEX + REG_WIDTH);
        uint32_t rSRegValue = read_from_register(cpu, rSRegIndex);
        uint8_t shiftAmount = bits_extract(rSRegValue, 0, BYTE_WIDTH);;
        offset_32bit = interpretShiftCode(shiftTypeCode, rMRegValue, shiftAmount);
    } else if (bit4 == 0) {
        uint8_t constantShiftAmount = bits_extract(instruction, CONST_5BIT_INDEX, CONST_5BIT_INDEX + 5);
        offset_32bit = interpretShiftCode(shiftTypeCode, rMRegValue, constantShiftAmount);
    }
    if (offset_32bit < NUM_MEMORY_LOCATIONS) {
        offset = offset_32bit;
    }
    return offset;
}

/* Transfers data from memory to a destination register or from a source register to  memory, depending on the value of
 * the load bit. If load bit equals 1, word is loaded from memory (fetched using supplied memory address) into the dest
 * register. If load bit equals 0, word read from source register is stored into memory at given address.
 * Index of source/dest register is rdRegIndex.
 */
void transferData(State cpu, uint32_t instruction, uint16_t memAddr) {
    uint8_t lBit = bits_extract(instruction, L_INDEX, L_INDEX + 1);
    uint8_t rdRegIndex = bits_extract(instruction, RD_INDEX, RD_INDEX + REG_WIDTH);
    uint32_t memWord;

    if (lBit == 1) {
        memWord = read_from_memory(cpu, memAddr);
        write_to_register(cpu, rdRegIndex, memWord);
    } else if (lBit == 0) {
        memWord = read_from_register(cpu, rdRegIndex);
        write_to_memory(cpu, memAddr, memWord);
    }
}

/* Computes memory address to based on value of up bit. Uses value stored in base register Rn and previously computed offset
 * value. Offset is either added or subtracted from base register.
 */
uint32_t compute_memory_address(uint32_t baseRegValue, uint16_t offset, uint32_t instruction) {
    uint32_t upBit = bits_extract(instruction, UP_INDEX, UP_INDEX + 1);

    if (upBit == 1) {
        return (baseRegValue + offset);
    }
    return (baseRegValue - offset);
}

<<<<<<< HEAD
=======

>>>>>>> 1c4445e614429fbe5c8aab1508feef5d4de27af8
/* Main function for executing single data transfer instruction. Computes unsigned offset - if I bit (immediateOffset)
 * equals 1, offset is interpreted as a shifted register and its value is accordingly computed, otherwise offset
 * interpreted as an immediate value. Value of base register is retrieved using baseRegIndex. If pBit equals 1 (pre-indexing),
 * the offset is added/subtracted to the base register before transferring the data, otherwise (post-indexing), offset is
 * added/subtracted to base register after transferring. Pre-indexing does not change value of base register, post-indexing
 * does (by the offset).
 */
uint32_t single_data_transfer(uint32_t instruction, State cpu) {
    uint16_t offset;
    if (check_condition(instruction, cpu) != 0) {
<<<<<<< HEAD
        uint8_t immediateOffset = (instruction & immediateMask) >> I_INDEX;
        uint8_t pBit = (instruction & pMask) >> P_INDEX;
        uint8_t baseRegIndex = (instruction & baseRegMask) >> RN_INDEX;
=======
        uint8_t immediateOffset = bits_extract(instruction, I_INDEX, I_INDEX + 1);
        uint8_t pBit = bits_extract(instruction, P_INDEX, P_INDEX + 1);
        uint8_t baseRegIndex = bits_extract(instruction, RN_INDEX, RN_INDEX + REG_WIDTH);
>>>>>>> 1c4445e614429fbe5c8aab1508feef5d4de27af8
        uint32_t baseRegValue = read_from_register(cpu, baseRegIndex);
        if (immediateOffset == 1) {
            offset = interpret_offset_shifted_reg(cpu, instruction);
        } else if (immediateOffset == 0) {
            offset = bits_extract(instruction, OFFSET_INDEX, OFFSET_INDEX + OFFSET_WIDTH);
        }

        if (pBit == 1) {

            uint32_t address = compute_memory_address(baseRegValue, offset, instruction);
            if (address < NUM_MEMORY_LOCATIONS) {
                uint16_t memoryAddress = address;
                transferData(cpu, instruction, memoryAddress);
            }

        } else if (pBit == 0) {

            if (baseRegValue < NUM_MEMORY_LOCATIONS) {
                uint16_t memAddress = baseRegValue;
                transferData(cpu, instruction, memAddress);
                uint32_t address = compute_memory_address(baseRegValue, offset, instruction);
                write_to_register(cpu, baseRegIndex, address);
            }
        }

        return 1;
    }

    fprintf(stderr, "Condition for single data transfer instruction not satisfied.\n");
    return 0;
}
