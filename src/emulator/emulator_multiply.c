//
// Created by Kapilan M on 30/05/2018.
//


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "emulator_multiply.h"
#include "utilities.h"
#include "emulator_cpu.h"

#define ACC_MASK 0x00200000
#define SET_MASK 0x00100000
#define RD_MASK 0x000f0000
#define RN_MASK 0x0000f000
#define RS_MASK 0x00000f00
#define RM_MASK 0x0000000f

uint8_t get_bit(uint32_t instr, uint32_t mask) {
    if ((instr & mask) == 0)
        return 0;

    return 1;
}

/* This applies the mask and shifts the bits which identify the register
 * down to the lowest order nibble which is then returned
 */
uint8_t get_register(uint32_t instr, uint32_t mask) {
    return (uint8_t)((instr & mask) >> mask);
}

void static set_CPSR(int32_t val, struct CPUState cpu) {
    if (val < 0)
        write_to_register(cpu, CPSR_INDEX, CPSR_N);
    else if (val == 0)
        write_to_register(cpu, CPSR_INDEX, CPSR_Z);
}

/* PRE: instruction is a multiply one */
void multiply(uint32_t instr, struct CPUState cpu) {

    if (check_any_condition(instr, cpu)) {
        uint8_t set = get_bit(instr, SET_MASK);
        uint8_t acc = get_bit(instr, ACC_MASK);
        uint8_t rd = get_register(instr, RD_MASK);
        uint8_t rn = get_register(instr, RN_MASK);
        uint8_t rs = get_register(instr, RS_MASK);
        uint8_t rm = get_register(instr, RM_MASK);

        int32_t result = read_from_register(cpu, rm) * read_from_register(cpu, rs);
        if (acc == 1)
            result += read_from_register(cpu, rn);

        write_to_register(cpu, rd, result);

        if (set == 1)
            set_CPSR(result, cpu);

    } else {
        fprintf(stderr, "Condition for multiply instruction not satisfied.");
    }
}

