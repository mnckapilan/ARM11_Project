#ifndef EMULATOR_MEMORY_H
#define EMULATOR_MEMORY_H

#include <stdint.h>
#include "emulator_utilities.h"

void clear_word(uint8_t *memLoc);

void write_word(uint8_t *memLoc, uint32_t val);

uint32_t read_word(uint8_t *memLoc);

#endif
