#include "multiply.h"

/*
 * This function first obtains the CPSR bits for C and V and retains their values.
 * Then it sets the values of N if the result is negative and Z is the result is 0
 */
void mul_set_CPSR(int32_t val, State cpu) {
    uint32_t currCPSR = read_from_register(cpu, CPSR_INDEX);
    uint32_t newCPSR = (currCPSR & CPSR_C) + (currCPSR & CPSR_V);
    if (val < 0) {
        newCPSR |= CPSR_N;
    } else if (val == 0) {
        newCPSR |= CPSR_Z;
    }

    write_to_register(cpu, CPSR_INDEX, newCPSR);
}

/*
 * The instruction is executed only if the CPSR condition is satisfied. 
 * First the instruction is split into its operands using the bits extracting function.
 * The standard multiplication is performed and the accumulated value is added if necessary.
 * Then the result is written to the destination register and the CPSR is updated if necessary.
 */
uint32_t multiply(uint32_t instr, State cpu) {

    if (check_condition(instr, cpu) == 0) {
        return 0;
    }

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

    return 1;
}

