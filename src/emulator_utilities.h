#ifndef EMULATOR_CONSTANTS_H
#define EMULATOR_CONSTANTS_H

#include <stdint.h>
#include <assert.h>

#define NUM_REGISTERS 17
#define NUM_GEN_PURPOSE_REGISTERS 13
#define NUM_MEMORY_LOCATIONS 65536
#define WORD_SIZE 32
#define STACK_SIZE_WORDS 2048
#define STACK_BASE 0x0000FFFC
#define STACK_POINTER_INDEX 13
#define LINK_REG_INDEX 14
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
#define CPSR_N_INDEX 31
#define CPSR_Z_INDEX 30
#define CPSR_V_INDEX 29
#define BIT31_MASK 0x80000000
#define SIGN_BIT 31
#define LARGEST_VAL 0xffffffff
#define BIT_SIZE 1
#define REG_INDEX_SIZE 4

/* Stores the state of the emulator memory and registers */
typedef struct {
    uint8_t *memory;   /* Array of bytes for memory */
    uint32_t *regs;    /* Array of 4 bytes (1 word) for registers */
} State;

uint32_t power_of_2(uint32_t val);

uint32_t bits_extract(uint32_t value, uint32_t begin, uint32_t end);

uint32_t max(uint32_t val1, uint32_t val2);

uint32_t min(uint32_t val1, uint32_t val2);

#endif
