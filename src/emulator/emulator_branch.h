#ifndef BRANCH_H
#define BRANCH_H

#include <stdint.h>
#include <emulator_cpu.h>

void branch(uint32_t instr, struct CPUState cpu);

#endif
