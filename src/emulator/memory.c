#include "memory.h"

/*
 * PRE: address passed will ensure all memory accesses are within bounds
 * Clears a word in memory by accessing each byte in the word and performing
 * a bitwise XOR with itself.
 */
void clear_word(uint8_t *memLoc) {
    uint8_t i;
    uint8_t *bytePtr = memLoc;
    for (i = 0; i < BYTES_PER_WORD; i++, bytePtr++) {
        (*bytePtr) ^= (*bytePtr);
    }
}

/* 
 * PRE: address passed will ensure all memory accesses are within bounds
 * Stores a word in memory in little endian ordering.
 * Extracts the bytes from the 32 bit value, from the least significant
 * to the most significant one stores these in ascending memory locations. 
 */
void write_word(uint8_t *memLoc, uint32_t val) {
    uint8_t i;
    uint8_t *bytePtr = memLoc;
    for (i = 0; i < BYTES_PER_WORD; i++, bytePtr++) {
        (*bytePtr) = (uint8_t) bits_extract(val, i * BYTE_SIZE, (i + 1) * BYTE_SIZE);
    }
}

/* 
 * PRE: address passed will ensure all memory accesses are within bounds
 * This reads 4 bytes from memory and shifts them accordingly such that
 * each byte occupies a single byte of the returned value. The bytes
 * are put together using bitwise OR.
 */
uint32_t read_word(uint8_t *memLoc) {
    uint8_t i;
    uint8_t *bytePtr = memLoc;
    uint32_t result = 0;
    for (i = 0; i < BYTES_PER_WORD; i++, bytePtr++) {
        result |= ((*bytePtr) << (i * BYTE_SIZE));
    }

    return result;
}
