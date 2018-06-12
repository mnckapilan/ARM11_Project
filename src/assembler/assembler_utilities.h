//
// Created by Kapilan M on 04/06/2018.
//

#ifndef ARM11_09_ASSEMBLER_DEFINITIONS_H
#define ARM11_09_ASSEMBLER_DEFINITIONS_H
#include <stdint.h>
#include <string.h>

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

//converts an number larger than 8 bits to a number less than or equal to 8 bits with a rotation
uint32_t convertOP2(uint32_t op2_32bit) {
    uint32_t rot;
    uint32_t result = op2_32bit;
    int condition = 1;
    int rotate_value = 0;
    while(condition) {
        //check bit0 and bit1
        int bit0 = (op2_32bit & 1);
        int bit1 = ((op2_32bit & 2)>>1);
        //stop the while loop either bit0 or bit1 is 1
        if((bit0) || (bit1)) {
            condition = 0;
        }
        if(condition) {
            //shift value = n -> shift 2n times
            op2_32bit = op2_32bit >> 2;
            rotate_value++;
        }
    }

    if(0 != rotate_value) {
        //count rotate backward (1 round is 16)
        rot = (uint32_t)(16 - rotate_value);
        //rot is the shift value which starts at bit 8
        result = op2_32bit | (rot << 8);
    }
    return result;
}

OPCODE getOpcodeDetails(char * phrase) {
    OPCODE result;
    if(!strcmp(phrase,"and")) {
        result = AND;
    } else if(!strcmp(phrase,"eor")) {
        result = EOR;
    } else if(!strcmp(phrase,"sub")) {
        result = SUB;
    } else if(!strcmp(phrase,"rsb")) {
        result = RSB;
    } else if(!strcmp(phrase,"add")) {
        result = ADD;
    } else if(!strcmp(phrase,"orr")) {
        result = ORR;
    } else if(!strcmp(phrase,"mov")) {
        result = MOV;
    } else if(!strcmp(phrase,"tst")) {
        result = TST;
    } else if(!strcmp(phrase,"teq")) {
        result = TEQ;
    } else if(!strcmp(phrase,"cmp")) {
        result = CMP;

    }
    return result;
}


#endif //ARM11_09_ASSEMBLER_DEFINITIONS_H
