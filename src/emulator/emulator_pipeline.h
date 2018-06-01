#ifndef EMULATOR_PIPELINE_H
#define EMULATOR_PIPELINE_H

#include <stdint.h>
#include "emulator_cpu.h"

struct PipelineStage {
    uint8_t fetchedEmpty;
    uint8_t decodedEmpty;
    uint32_t fetched;
    uint32_t decodedInstr;
    void (*decoded) (uint32_t, struct CPUState);
};

typedef void (*instr_ptr)(uint32_t, struct CPUState);

#endif