//
// Created by Kapilan M on 08/06/2018.
//

#ifndef ARM11_09_ASSEMBLER_DATAPROCESSING_H
#define ARM11_09_ASSEMBLER_DATAPROCESSING_H

#include "assembler_utilities.h"

#define COND_dataProcessing 0x0000000E
#define cond_Mask_DP 28
#define I_Mask_DP 25
#define opcode_Mask_DP 21
#define S_Mask_DP 20
#define Rn_Mask_DP 16
#define Rd_Mask_DP 12

uint32_t assembler_dataProcessing(instruction *ins);

#endif //ARM11_09_ASSEMBLER_DATAPROCESSING_H
