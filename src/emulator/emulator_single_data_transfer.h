#ifndef SINGLEDATATRANSFER_H
#define SINGLEDATATRANSFER_H

#include <stdint.h>
#include "emulator_cpu.h"

void single_data_transfer(uint32_t instruction, struct CPUState cpu);

#endif
