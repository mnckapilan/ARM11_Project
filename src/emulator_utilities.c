#include "emulator_utilities.h"

/* Finds 2 ^ val through repeated multiplication */
uint32_t power_of_2(uint32_t val) {
    uint32_t result = 1;

    for (uint32_t i = 0; i < val; i++) {
        result *= 2;
    }

    return result;
}

/* 
 * Takes bits between begin and end --> [begin,end)
 * Obtains a mask of all 1's by calculating (2 ^ end) - 1, applies the
 * mask to the value with bitwise AND then shifts the value right such that the 
 * least significant bit which is returned is the begin bit 
 */
uint32_t bits_extract(uint32_t value, uint32_t begin, uint32_t end) {
    assert(begin < end);
    uint32_t mask = power_of_2(end) - 1;
    return (value & mask) >> begin;
}

/* Computes the maximum of two unsigned integers */
uint32_t max(uint32_t val1, uint32_t val2) {
    if (val1 > val2) {
        return val1;
    }
    return val2;
}

/* Computes the minimum of two unsigned integers */
uint32_t min(uint32_t val1, uint32_t val2) {
    if (val1 < val2) {
        return val1;
    }
    return val2;
}



