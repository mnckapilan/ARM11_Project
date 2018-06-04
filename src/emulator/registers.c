#include "registers.h"

void write_register(uint32_t *reg, uint32_t val) {
    (*reg) = val;
}

uint32_t read_register(uint32_t *reg) {
    return (*reg);
}

void clear_register(uint32_t *reg) {
    (*reg) ^= (*reg);
}
