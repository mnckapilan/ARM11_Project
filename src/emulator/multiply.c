#include "multiply.h"

/*
 * This function first obtains the CPSR bits for C and V and retains their values.
 * Then it sets the values of N and Z accordingly
 */
void mul_set_CPSR(int32_t val, State cpu) {
    uint32_t currCPSR = read_from_register(cpu, CPSR_INDEX);
    uint32_t newCPSR = (currCPSR & CPSR_C) + (currCPSR & CPSR_V);
    if (val < 0) {
        newCPSR += CPSR_N;
    } else if (val == 0) {
        newCPSR += CPSR_Z;
    }

    write_to_register(cpu, CPSR_INDEX, newCPSR);
}

/* PRE: instruction is a multiply one */
void multiply(uint32_t instr, State cpu) {

    if (check_condition(instr, cpu) != 0) {
        uint32_t set = bits_extract(instr, MULT_SET_INDEX, MULT_SET_INDEX + BIT_SIZE);
        uint32_t acc = bits_extract(instr, MULT_ACC_INDEX, MULT_ACC_INDEX + BIT_SIZE);
        uint32_t rd = bits_extract(instr, MULT_RD_INDEX, MULT_RD_INDEX + REG_INDEX_SIZE);
        uint32_t rn = bits_extract(instr, MULT_RN_INDEX, MULT_RN_INDEX + REG_INDEX_SIZE);
        uint32_t rs = bits_extract(instr, MULT_RS_INDEX, MULT_RS_INDEX + REG_INDEX_SIZE);
        uint32_t rm = bits_extract(instr, MULT_RM_INDEX, MULT_RM_INDEX + REG_INDEX_SIZE);

        int32_t result = read_from_register(cpu, rm) * read_from_register(cpu, rs);
        if (acc == 1) {
            result += read_from_register(cpu, rn);
        }

        write_to_register(cpu, rd, result);

        if (set == 1) {
            mul_set_CPSR(result, cpu);
        }

    } else {
        fprintf(stderr, "Condition for multiply instruction not satisfied.\n");
    }
}

