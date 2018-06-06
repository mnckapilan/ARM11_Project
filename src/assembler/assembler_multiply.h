//
// Created by Kapilan M on 06/06/2018.
//

#ifndef ARM11_09_ASSEMBLER_MULTIPLY_H
#define ARM11_09_ASSEMBLER_MULTIPLY_H
#define COND_multiply 0x0000000E
#define cond_Mask_MUL 28
#define A_Mask_MUL 21
#define S_Mask_MUL 20
#define Rn_Mask_MUL 12
#define Rd_Mask_MUL 16
#define Mask_4_7_MUL 4
#define Rs_Mask_MUL 8

uint32_t assembler_multiply(instruction *instructs);

#endif //ARM11_09_ASSEMBLER_MULTIPLY_H
