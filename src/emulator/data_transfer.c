//
// Created by Sukant Roy on 13/06/2018.
//

#include "data_transfer.h"

/*
 * Transfers data from memory to a destination register or from a source register to  memory, depending on the value of
 * the load bit. If load bit equals 1, word is loaded from memory (fetched using supplied memory address) into the dest
 * register. If load bit equals 0, word read from source register is stored into memory at given address.
 * Index of source/dest register is rdRegIndex.
 */
uint32_t transferData(State cpu, uint32_t memAddr, uint32_t loadStoreBit, uint32_t rdRegIndex) {
    uint32_t memWord;
    if (loadStoreBit == 1) {
        if (memory_in_bounds(memAddr) == 0) {
            memWord = read_from_memory(cpu, memAddr);
            write_to_register(cpu, rdRegIndex, memWord);
            return 1;
        }
        return 0;
    } else {
        if (register_in_bounds(rdRegIndex) == 0) {
            memWord = read_from_register(cpu, rdRegIndex);
            write_to_memory(cpu, memAddr, memWord);
            return 1;
        }
        return 0;
    }
}