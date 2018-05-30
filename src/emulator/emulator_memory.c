#include "emulator_memory.h"
#include "emulator_constants.h"

uint32_t create_word_from_bytes(uint8_t* firstBytePtr)
{
    uint8_t i;
    uint8_t* bytePtr = firstBytePtr;
    uint32_t result = 0;
    for (i = 0; i < BYTES_PER_WORD; i++, bytePtr++)
        result += ((*bytePtr) << (BYTE_SIZE * i));

    return result;
}

/* PRE: address passed will ensure all memory accesses are within bounds */
void clear_word(uint8_t *memLoc)
{
    uint8_t i;
    uint8_t *bytePtr = memLoc;
    for (i = 0; i < BYTES_PER_WORD; i++, bytePtr++)
    {
        (*bytePtr) ^= (*bytePtr);
    }
}

/* PRE: address passed will ensure all memory accesses are within bounds */
void write_word(uint8_t *memLoc, uint32_t val)
{
    uint8_t i;
    uint8_t *bytePtr = memLoc;
    for (i = 0; i < BYTES_PER_WORD; i++, bytePtr++)
        (*bytePtr) = (uint8_t) (val >> (BYTE_SIZE * i));
}

/* PRE: address passed will ensure all memory accesses are within bounds */
uint32_t read_word(uint8_t *memLoc)
{
    uint8_t i;
    uint8_t *bytePtr = memLoc;
    uint32_t result = 0;
    for (i = 0; i < BYTES_PER_WORD; i++, bytePtr++)
        result += ((*bytePtr) << (BYTE_SIZE * i));

    return result;
}


