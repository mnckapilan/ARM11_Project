#ifndef EMULATOR_HALT_H
#define EMULATOR_HALT_H

#include <stdint.h>
#include "emulator_cpu.h"

void halt(uint32_t instr, struct CPUState cpu);

#endif
