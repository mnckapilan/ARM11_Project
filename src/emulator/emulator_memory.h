#ifndef EMULATOR_MEMORY_H
#define EMULATOR_MEMORY_H

#include <stdint.h>

uint32_t create_word_from_bytes(uint8_t* firstBytePtr);

void clear_word(uint8_t *memLoc);

void write_word(uint8_t *memLoc, uint32_t val);

uint32_t read_word(uint8_t *memLoc);

#endif
