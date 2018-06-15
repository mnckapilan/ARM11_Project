//
// Created by Kapilan M on 08/06/2018.
//

#include "assembler_data_processing.h"
#include "assembler_utilities.h"

uint32_t assembler_dataProcessing(instruction *ins){

    uint32_t result;
    //initiate the values to be 0
    uint32_t S = 0;
    uint32_t Rn = 0;
    uint32_t Rd = 0;

    //all data processing instructions have cond 0x0000000E
    uint32_t cond = COND_dataProcessing;

    // take I bit from loader
    uint32_t I = ins -> imm;

    // take Operand2 from loader
    uint32_t Operand2 = ins->operand2;

    //get an enum of the instruction
    OPCODE ins_op = getOpcodeDetails(ins->phrase);
    uint32_t opcode = ins_op;
    switch(ins_op) {
        case AND :
        case EOR :
        case ORR :
        case SUB :
        case RSB :
        case ADD :
            //set Rd and Rn
            Rd = ins->rd;
            Rn = ins->rn;
            break;

        case TST :
        case TEQ :
        case CMP :
            //set S to 1 and set Rn from corresponding input rn
            S = 1;
            Rn = ins->rn;
            break;

        case MOV :
            // uses just 1 register - Rd
            Rd = ins->rd;
            break;

    }
    //rotate value that is bigger than 8 bit to be an 8 bit with a shift value
    if(Operand2 > 0x000000FF) {
        Operand2 = convertOP2(Operand2);
    }


    /*shift each component  to the start positions of
    each field then combine them together by logical OR
    to get the binary instruction*/
    cond = cond << cond_Mask_DP;
    I = I << I_Mask_DP;
    opcode = opcode << opcode_Mask_DP;
    S = S << S_Mask_DP;
    Rn = Rn << Rn_Mask_DP;
    Rd = Rd << Rd_Mask_DP;
    result = cond | I | opcode | S | Rn | Rd | Operand2;

    return result;

}
