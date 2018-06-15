//
// Created by Kapilan M on 06/06/2018.
//

#include "assembler_multiply.h"

uint32_t assembler_multiply(instruction *instructs) {

    uint32_t result;
    uint32_t Rd;
    uint32_t Rs;
    uint32_t Rm;

    //initiate the values to be zero
    uint32_t Rn = 0;
    uint32_t A = 0;
    uint32_t S = 0;

    uint32_t cond = COND_multiply;
    //bit 4-7 is the constant 1001

    uint32_t bit4_7 = 9;

    // mla uses additional register Rn and sets A bit to be 1
    if(!strcmp(instructs->phrase,"mla")) {
        A = 1;
        Rn = instructs->rn;
    }
    Rd = instructs->rd;
    Rs = instructs->rs;
    Rm = instructs->rm;

    //shift all fields to their corresponding and uses logical OR
    cond = cond << cond_Mask_MUL;
    A = A << A_Mask_MUL;
    S = S << S_Mask_MUL;
    Rn = Rn << Rn_Mask_MUL;
    Rd = Rd << Rd_Mask_MUL;
    bit4_7 = bit4_7 << Mask_4_7_MUL;
    Rs = Rs << Rs_Mask_MUL;
    result = cond | A | S | Rn | Rd | bit4_7 | Rs | Rm;
    return result;
}
