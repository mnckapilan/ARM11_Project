#include "emulator.h"
#include "utilities.h"

/*
 * This allocates the memory and registers in the heap dynamically. The calloc() function initialises the allocated
 * space to 0 so there is no need to do this manually.
 */
State initialize_CPU() {
    uint8_t *memory = calloc(NUM_MEMORY_LOCATIONS, sizeof(uint8_t));

    uint32_t *regs = calloc(NUM_REGISTERS, sizeof(uint32_t));

    State cpu;
    cpu.memory = memory;
    cpu.regs = regs;

    return cpu;
}

uint8_t memory_in_bounds(uint32_t addr) {
    if (addr > (NUM_MEMORY_LOCATIONS - BYTES_PER_WORD)) {
        printf("Error: Out of bounds memory access at address 0x%08x\n", addr);
        return 1;
    }

    return 0;
}

void write_to_memory(State cpu, uint32_t addr, uint32_t val) {
    if (memory_in_bounds(addr) == 0) {
        write_word(&cpu.memory[addr], val);
    }
}

/* PRE: Memory address is in bounds */
uint32_t read_from_memory(State cpu, uint32_t addr) {
    return read_word(&cpu.memory[addr]);
}

uint8_t register_in_bounds(uint32_t Rn) {
    if (Rn >= NUM_REGISTERS) {
        printf("Error: Out of bounds register access with register %u\n", Rn);
        return 1;
    }

    return 0;
}

void write_to_register(State cpu, uint32_t Rn, uint32_t val) {
    if (register_in_bounds(Rn) == 0) {
        write_register(&cpu.regs[Rn], val);
    }
}

/* PRE: Register index is in bounds */
uint32_t read_from_register(State cpu, uint32_t Rn) {
    return read_register(&cpu.regs[Rn]);
}

void increment_PC(State cpu) {
    cpu.regs[PC_INDEX] += BYTES_PER_WORD;
}

uint32_t get_next_instruction(State cpu) {
    return read_from_memory(cpu, read_register(&cpu.regs[PC_INDEX]));
}

uint32_t get_Z(uint32_t val) {
    return bits_extract(val, CPSR_Z_INDEX, CPSR_Z_INDEX + BIT_SIZE);
}

uint32_t get_N(uint32_t val) {
    return bits_extract(val, CPSR_N_INDEX, CPSR_N_INDEX + BIT_SIZE);
}

uint32_t get_V(uint32_t val) {
    return bits_extract(val, CPSR_V_INDEX, CPSR_V_INDEX + BIT_SIZE);
}

/* Returns non zero if the condition is satisfied, 0 otherwise */
uint32_t check_condition(uint32_t instr, State cpu) {
    ConditionCode conditionCode = bits_extract(instr, CPSR_FLAGS_OFFSET, CPSR_FLAGS_OFFSET + NUM_CPSR_FLAGS);

    uint32_t CPSR = read_from_register(cpu, CPSR_INDEX);

    switch (conditionCode) {

        case EQ :
            return get_Z(CPSR);

        case NE :
            return !get_Z(CPSR);

        case GE :
            if (get_N(CPSR) == get_V(CPSR)) {
                return 1;
            }
            return 0;

        case LT :
            if (get_N(CPSR) != get_V(CPSR)) {
                return 1;
            }
            return 0;

        case GT :
            if ((get_Z(CPSR) == 0) && (get_N(CPSR) == get_V(CPSR))) {
                return 1;
            }
            return 0;

        case LE :
            if ((get_Z(CPSR) == 1) || (get_N(CPSR) != get_V(CPSR))) {
                return 1;
            }
            return 0;

        case AL :
            return 1;

        default :
            return 0;
    }
}

void read_instructions(FILE *file, State cpu) {

    uint32_t instr = 0;
    uint32_t size = get_size(file) / BYTES_PER_WORD;

    for (uint32_t i = 0; i < size; i++) {
        fread(&instr, sizeof(uint32_t), 1, file);
        write_to_memory(cpu, i * BYTES_PER_WORD, instr);
    }
}

/*
 * The order of the decoding is set out such that the multiply mask is applied before the
 * data processing one since a multiply instruction would be categorized as a data processing
 * one if applied in the reverse order.
 */
Instr_ptr decode(uint32_t instr) {
    if ((instr & MULTIPLY_MASK) == MULTIPLY_FORMAT) {

        return &multiply;

    } else if ((instr & SINGLE_DATA_TRANSFER_MASK) == SINGLE_DATA_TRANSFER_FORMAT) {

        return &single_data_transfer;

    } else if ((instr & BRANCH_MASK) == BRANCH_FORMAT) {

        return &branch;

    } else if (((instr & DATA_PROCESSING_MASK) == DATA_PROCESSING_FORMAT) && (instr != 0)) {

        return &data_processing;

    } else if ((instr & HALT_MASK) == HALT_FORMAT) {

        return &halt;
    }

    printf("Error: Instruction format not recognised\n");
    return NULL;
}

void run_emulator(State cpu) {
    Stage pipelineStage;
    pipelineStage.decodedEmpty = 1;
    pipelineStage.fetchedEmpty = 1;
    pipelineStage.fetched = 0;
    pipelineStage.decoded = 0;

    while (pipelineStage.decoded != &halt) {

        if ((pipelineStage.decodedEmpty == 0) && (pipelineStage.decoded != NULL)) {

            uint32_t executed = pipelineStage.decoded(pipelineStage.decodedInstr, cpu);

            if ((pipelineStage.decoded == &branch) && (executed == 1)) {
                pipelineStage.decodedEmpty = 1;
                pipelineStage.fetchedEmpty = 1;
            }
        }

        if (pipelineStage.fetchedEmpty == 0) {

            pipelineStage.decoded = decode(pipelineStage.fetched);
            pipelineStage.decodedInstr = pipelineStage.fetched;

            if (pipelineStage.decodedEmpty == 1) {
                pipelineStage.decodedEmpty = 0;
            }

        } else {
            pipelineStage.fetchedEmpty = 0;
        }

        pipelineStage.fetched = get_next_instruction(cpu);
        increment_PC(cpu);
    }

    halt(0, cpu);
}

void free_emulator(State cpu) {
    free(cpu.regs);
    free(cpu.memory);
}
