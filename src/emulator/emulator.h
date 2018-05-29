#ifndef emulator_io_h
#define emulator_io_h

#include <stdio.h>
#include <stdint.h>

FILE* open_file(char* path);

void close_file(FILE* path);

uint8_t file_exists(char* path);

uint32_t get_file_size(FILE* file);

uint8_t load_file(char* path, FILE* file, uint32_t memorySize);


typedef void *PointerToBeCast;
// functions to apply shifting
typedef u_int32_t (*Shifter)(u_int32_t*, u_int32_t, u_int32_t, u_int32_t);
// typedef of execute function with dummy pointer arg
typedef void (*Execute)(PointerToBeCast);

#define BASE
u_int8_t cond;
u_int32_t * cpsr;
Execute function;

// BASE
// Defines the two key components of every decoded
// instruction
// the cond flag that is required to be checked on execution and the function to call on
// execution, which will take the address of the struct itself, fixing the base pointer from where it can
// fetch it's arguments

typedef struct
{
    BASE
    u_int32_t *op1;
    u_int32_t *op2;
    u_int32_t *acc;
    u_int32_t *des;
    u_int8_t     s;
} MultiplyInstruction;
#endif
