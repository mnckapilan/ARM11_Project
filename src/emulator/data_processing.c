#include "data_processing.h"

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

Operand2 arithmetic_shift_right(uint32_t val, uint32_t shiftMagnitude) {
    assert(shiftMagnitude < WORD_SIZE);

    uint32_t signBit = bits_extract(val, WORD_SIZE - 1, WORD_SIZE);
    uint32_t upperOrderBits = ((signBit << (shiftMagnitude + 1)) - 1) << (WORD_SIZE - shiftMagnitude - 1);

    Operand2 result;
    result.val = val;
    result.cOut = 0;

    if (shiftMagnitude > 0) {
        result.cOut = bits_extract(val, shiftMagnitude - 1, shiftMagnitude);
        result.val = (val >> shiftMagnitude) + upperOrderBits;
    }

    return result;
}

Operand2 rotate_right(uint32_t val, uint32_t rotate) {
    assert (rotate < WORD_SIZE);

    Operand2 result;
    result.val = bits_extract(val, rotate, WORD_SIZE);
    result.cOut = 0;

    if (rotate > 0) {
        result.val += bits_extract(val, 0, rotate) << (WORD_SIZE - rotate);
        result.cOut = bits_extract(val, rotate - 1, rotate);
    }

    return result;
}

Operand2 interpret_imm_operand(uint32_t val) {
    uint32_t imm = bits_extract(val, OPERAND2_IMM_OFFSET, OPERAND2_IMM_OFFSET + OPERAND2_IMM_SIZE);
    uint32_t rotate = bits_extract(val, OPERAND2_ROTATE_OFFSET, OPERAND2_ROTATE_OFFSET + OPERAND2_ROTATE_SIZE);
    rotate *= OPERAND2_ROTATE_SCALE;

    return rotate_right(imm, rotate);
}

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

/* PRE: CPSR flag is set */
void dp_set_CPSR(uint32_t result, uint32_t cOut, State cpu) {
    uint32_t currCPSR = read_from_register(cpu, CPSR_INDEX);
    uint32_t newCPSR = (currCPSR & CPSR_V);

    if (cOut == 1) {
        newCPSR += CPSR_C;
    }

    if (result == 0) {
        newCPSR += CPSR_Z;
    }

    newCPSR += (result & BIT31_MASK);

    write_to_register(cpu, CPSR_INDEX, newCPSR);
}

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

    if ((LARGEST_VAL - max(val1, val2)) < min(val1, val2)) {
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

        case ADD :
            arithmetic_op(val1, val2.val, cpu, rd, s, 1, opcode);
            break;

        case SUB :
            arithmetic_op(val1, val2.val, cpu, rd, s, 1, opcode);
            break;

        case RSB :
            arithmetic_op(val2.val, val1, cpu, rd, s, 1, opcode);
            break;

        case CMP :
            arithmetic_op(val1, val2.val, cpu, rd, s, 0, opcode);
            break;

        case MOV :
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
