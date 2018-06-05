#ifndef EMULATOR_CONSTANTS_H
#define EMULATOR_CONSTANTS_H

#include <stdint.h>
#include <assert.h>

#define NUM_REGISTERS 17
#define NUM_GEN_PURPOSE_REGISTERS 13
#define NUM_MEMORY_LOCATIONS 65536
#define WORD_SIZE 32
#define BYTE_SIZE 8
#define BYTES_PER_WORD 4
#define PC_INDEX 15
#define CPSR_INDEX 16
#define CPSR_FLAGS_OFFSET 28
#define NUM_CPSR_FLAGS 4
#define CPSR_N 0x80000000
#define CPSR_Z 0x40000000
#define CPSR_C 0x20000000
#define CPSR_V 0x10000000
#define BIT31_MASK 0x80000000
#define LARGEST_VAL 0xffffffff
#define BIT_SIZE 1
#define REG_INDEX_SIZE 4

typedef struct {
    uint8_t *memory;
    uint32_t *regs;
} State;

uint32_t power_of_2(uint32_t val);

uint32_t bits_extract(uint32_t value, uint32_t begin, uint32_t end);

#endif
