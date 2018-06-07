#include "utilities.h"

uint32_t power_of_2(uint32_t val) {
    uint32_t result = 1;

    for (uint32_t i = 0; i < val; i++) {
        result *= 2;
    }

    return result;
}

/* Takes bits between begin and end --> [begin,end) */
uint32_t bits_extract(uint32_t value, uint32_t begin, uint32_t end) {
    assert(begin < end);
    uint32_t mask = power_of_2(end) - 1;
    return (value & mask) >> begin;
}

uint32_t max(uint32_t val1, uint32_t val2) {
    if (val1 > val2) {
        return val1;
    }
    return val2;
}

uint32_t min(uint32_t val1, uint32_t val2) {
    if (val1 < val2) {
        return val1;
    }
    return val2;
}



