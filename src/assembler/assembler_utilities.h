//
// Created by Kapilan M on 04/06/2018.
//

#ifndef ARM11_09_ASSEMBLER_DEFINITIONS_H
#define ARM11_09_ASSEMBLER_DEFINITIONS_H
#include <stdint.h>

typedef struct instructions {

    char *phrase;
    uint32_t operand2;
    uint32_t rd;
    uint32_t rn;
    uint32_t rs;
    uint32_t rm;
    uint32_t imm;
    uint32_t expression;
    uint32_t lastAdd;
    uint32_t p;
    uint32_t u;

} instruction;


//enums for OPCODEs
typedef enum {
    AND = 0,
    EOR = 1,
    SUB = 2,
    RSB = 3,
    ADD = 4,
    TST = 8,
    TEQ = 9,
    CMP = 10,
    ORR = 12,
    MOV = 13
} OPCODE;



#endif //ARM11_09_ASSEMBLER_DEFINITIONS_H
