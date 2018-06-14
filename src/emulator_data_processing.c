#include "emulator_data_processing.h"

/*
 * Logically shifts left if the number of bits to shift is between 1 and 31.
 * The carry out is set to the least significant bit which is lost due to the shift
 * (32 - shiftMagnitude)
 */
Operand2 logical_shift_left(uint32_t val, uint32_t shiftMagnitude) {
    assert(shiftMagnitude < WORD_SIZE);

    Operand2 result;
    result.val = val;
    result.cOut = 0;

    if (shiftMagnitude > 0) {
        result.cOut = bits_extract(val, WORD_SIZE - shiftMagnitude, WORD_SIZE - shiftMagnitude + BIT_SIZE);
        result.val = val << shiftMagnitude;
    }

    return result;
}

/*
 * Logically shifts right if the number of bits to shift is between 1 and 31.
 * The carry out is set to the most significant bit which is lost due to the shift
 */ 
Operand2 logical_shift_right(uint32_t val, uint32_t shiftMagnitude) {
    assert (shiftMagnitude < WORD_SIZE);

    Operand2 result;
    result.val = val;
    result.cOut = 0;

    if (shiftMagnitude > 0) {
        result.cOut = bits_extract(val, shiftMagnitude - 1, shiftMagnitude);
        result.val = val >> shiftMagnitude;
    }

    return result;
}

/*
 * Arithmetically shifts right if the number of positions to shift is between 1 and 31.
 * The sign bit of the original value is extracted, shifted left by 1 more than the shift
 * magnitude, then 1 is subtracted from it, setting bits 0 to shiftMagnitude to 1. This value
 * is then shifted by enough positions to the left that it occupies the upper bits of the word
 * that will be sign extended. Then the original value is right shifted and sign extended.
 */
Operand2 arithmetic_shift_right(uint32_t val, uint32_t shiftMagnitude) {
    assert(shiftMagnitude < WORD_SIZE);

    Operand2 result;
    result.val = val;
    result.cOut = 0;

    if (shiftMagnitude > 0) {
	
    	uint32_t signBit = bits_extract(val, WORD_SIZE - 1, WORD_SIZE);
    	uint32_t upperOrderBits = ((signBit << (shiftMagnitude + 1)) - 1) << (WORD_SIZE - shiftMagnitude - 1);
        result.cOut = bits_extract(val, shiftMagnitude - 1, shiftMagnitude);
        result.val = (val >> shiftMagnitude) | upperOrderBits;
    }

    return result;
}

/*
 * Rotates right if the rotate magnitude is between 1 and 31.
 * This extracts the bits which will be 'wrapped around' to the most significant bits of the 
 * value and shifts these to the top bits and adds these to the bits which will be right shifted
 * but not 'wrapped around' using logical OR.
 */
Operand2 rotate_right(uint32_t val, uint32_t rotate) {
    assert (rotate < WORD_SIZE);

    Operand2 result;
    result.val = bits_extract(val, rotate, WORD_SIZE);
    result.cOut = 0;

    if (rotate > 0) {
        result.val |= bits_extract(val, 0, rotate) << (WORD_SIZE - rotate);
        result.cOut = bits_extract(val, rotate - 1, rotate);
    }

    return result;
}

/*
 * PRE: The operand is of the immediate interpretation format. 
 * Extracts the bits to be rotated and the bits which determine the magnitude of the rotation
 * and calls the rotate_right function with the rotation scaled by 2 to perform the rotation.
 */ 
Operand2 interpret_imm_operand(uint32_t val) {
    uint32_t imm = bits_extract(val, OPERAND2_IMM_OFFSET, OPERAND2_IMM_OFFSET + OPERAND2_IMM_SIZE);
    uint32_t rotate = bits_extract(val, OPERAND2_ROTATE_OFFSET, OPERAND2_ROTATE_OFFSET + OPERAND2_ROTATE_SIZE);
    rotate *= OPERAND2_ROTATE_SCALE;

    return rotate_right(imm, rotate);
}

/*
 * PRE: The operand is of the register interpretation format.
 * This extracts the contents of the register specified in bits 0 to 3, determines the type
 * of shift from bits 5 and 6 and determines whether the value to be shifted originates from
 * the value passed in or from another register - obtaining the value accordingly.
 * Then a function pointer is used to call the correct shifting function specified from bits
 * 5 and 6. 
 */ 
Operand2 interpret_reg_operand(uint32_t val, State cpu) {
    uint32_t regIndex = bits_extract(val, OPERAND2_RM_OFFSET, OPERAND2_RM_OFFSET + REG_INDEX_SIZE);
    uint32_t regContents = read_from_register(cpu, regIndex);

    uint32_t shiftOrigin = bits_extract(val, OPERAND2_SHIFT_OFFSET, OPERAND2_SHIFT_OFFSET + OPERAND2_SHIFT_SIZE);
    Shift shiftType = bits_extract(val, OPERAND2_SHIFT_TYPE_OFFSET,
                                   OPERAND2_SHIFT_TYPE_OFFSET + OPERAND2_SHIFT_TYPE_SIZE);

    uint32_t shiftMagnitude = 0;

    if (shiftOrigin == 0) {

        shiftMagnitude = bits_extract(val, OPERAND2_SHIFT_MAG_OFFSET,
                                      OPERAND2_SHIFT_MAG_OFFSET + OPERAND2_SHIFT_MAG_SIZE);

    } else if (shiftOrigin == 1) {

        uint32_t rsIndex = bits_extract(val, OPERAND2_RS_OFFSET, OPERAND2_RS_OFFSET + REG_INDEX_SIZE);
        assert (rsIndex < NUM_GEN_PURPOSE_REGISTERS);
        shiftMagnitude = read_from_register(cpu, rsIndex);
        shiftMagnitude = bits_extract(shiftMagnitude, 0, BYTE_SIZE);

    }

    Operand2 (*shiftFunction)(uint32_t, uint32_t) = NULL;

    switch (shiftType) {
        case LSL :
            shiftFunction = &logical_shift_left;
            break;
        case LSR :
            shiftFunction = &logical_shift_right;
            break;
        case ASR :
            shiftFunction = &arithmetic_shift_right;
            break;
        case ROR :
            shiftFunction = &rotate_right;
            break;
        default :
            break;
    }

    return shiftFunction(regContents, shiftMagnitude);
}

/* PRE: CPSR flag is set 
 * The V bit is retained from the previous CPSR register.
 * If the carry out is 1, the C bit is set using logical OR with the new value.
 * If the result is zero, the Z bit is set in the same way.
 * The N bit is set to the value of the sign bit of the result.
 */
void dp_set_CPSR(uint32_t result, uint32_t cOut, State cpu) {
    uint32_t currCPSR = read_from_register(cpu, CPSR_INDEX);
    uint32_t newCPSR = (currCPSR & CPSR_V);

    if (cOut == 1) {
        newCPSR |= CPSR_C;
    }

    if (result == 0) {
        newCPSR |= CPSR_Z;
    }

    newCPSR |= (result & BIT31_MASK);

    write_to_register(cpu, CPSR_INDEX, newCPSR);
}

/*
 * Performs AND, EOR, ORR, TST and TEQ.
 * It writes the result to the destination register if the operation is AND, EOR or ORR.
 * It sets the CPSR if the set condition codes flag was set in the instruction.
 */ 
void
logical_op(uint32_t val1, uint32_t val2, uint32_t setCPSR, State cpu, uint32_t dest, uint32_t cOut, Opcode op) {

    uint32_t result = 0;

    switch (op) {

        case AND :
        case TST :
            result = val1 & val2;
            if (op == AND) {
                write_to_register(cpu, dest, result);
            }
            break;

        case EOR :
        case TEQ :
            result = val1 ^ val2;
            if (op == EOR) {
                write_to_register(cpu, dest, result);
            }
            break;

        case ORR:
            result = val1 | val2;
            write_to_register(cpu, dest, result);
            break;

        default :
            break;
    }

    if (setCPSR == 1) {
        dp_set_CPSR(result, cOut, cpu);
    }
}

/*
 * Performs ADD, SUB, RSB and CMP.
 * If the operation is ADD, it just adds the values as normal. 
 * Otherwise the bits of val2 are inverted and 1 is added (obtains the negative of val2 with
 * two's complement) and this is added to val1.
 * Carry out is set if overflow occurred:     val1 + val2 > 2^32 - 1
 *                                        <=> val1 > 2^32 - 1 - val2
 * Carry out is also set if the operation involved a subtraction and the second operand of the
 * subtraction was larger than the first (when considering signed values)
 * The result is written and the CPSR is updated depending on what was specified in the instruction.
 */ 
void arithmetic_op(uint32_t val1, uint32_t val2, State cpu, uint32_t dest, uint32_t setCPSR, uint32_t writeResult, Opcode op) {

    uint32_t result = 0;
    uint32_t cOut = 0;

    switch(op) {

        case ADD :
            result = val1 + val2;
            break;

        case SUB :
        case RSB :
        case CMP :
            result = val1 + (~(val2) + 1);
            break;

        default :
            break;

    }

    if ((LARGEST_VAL - val2) < val1) {
        cOut = 1;
    }

    if (((int32_t) val1 >= (int32_t) val2) && (op != ADD)) {
        cOut = 1;
    }

    if (writeResult == 1) {
        write_to_register(cpu, dest, result);
    }

    if (setCPSR == 1) {
        dp_set_CPSR(result, cOut, cpu);
    }
}

/*
 * If the condition in the most significant nibble of the instruction is satisfied by the CPSR,
 * the instruction runs.
 * The various operands needed for the instruction are extracted first.
 * The value in register RN is extracted and the value of Operand2 is obtained, depending on the
 * value of the I bit.
 * The different operations specified by the opcode are then called.
 */ 
uint32_t data_processing(uint32_t instr, State cpu) {
    if (check_condition(instr, cpu) == 0) {
        return 0;
    }
    uint32_t i = bits_extract(instr, I_OFFSET, I_OFFSET + BIT_SIZE);
    Opcode opcode = bits_extract(instr, OPCODE_OFFSET, OPCODE_OFFSET + OPCODE_SIZE);
    uint32_t s = bits_extract(instr, S_OFFSET, S_OFFSET + BIT_SIZE);
    uint32_t rn = bits_extract(instr, RN_OFFSET, RN_OFFSET + REG_INDEX_SIZE);
    uint32_t rd = bits_extract(instr, RD_OFFSET, RD_OFFSET + REG_INDEX_SIZE);

    uint32_t val1 = read_from_register(cpu, rn);
    Operand2 val2;

    if (i == 1) {
        val2 = interpret_imm_operand(instr);
    } else if (i == 0) {
        val2 = interpret_reg_operand(instr, cpu);
    }

    switch (opcode) {

        case AND :
        case EOR :
        case TST :
        case TEQ :
        case ORR :
            logical_op(val1, val2.val, s, cpu, rd, val2.cOut, opcode);
            break;

        case ADD : /* Val1 + Val2 -- Result written to register rd */
            arithmetic_op(val1, val2.val, cpu, rd, s, 1, opcode);
            break;

        case SUB : /* Val1 - Val2 -- Result written to register rd */
            arithmetic_op(val1, val2.val, cpu, rd, s, 1, opcode);
            break;

        case RSB : /* Val2 - Val1 -- Result written to register rd */
            arithmetic_op(val2.val, val1, cpu, rd, s, 1, opcode);
            break;

        case CMP : /* Val1 - Val2 -- Result not written */
            arithmetic_op(val1, val2.val, cpu, rd, s, 0, opcode);
            break;

        case MOV : /* Moves the value in Operand2 to register rd */
            write_to_register(cpu, rd, val2.val);
            if (s == 1) {
                dp_set_CPSR(val2.val, val2.cOut, cpu);
            }
            break;

        default :
            break;

    }

    return 1;
}
