#include "emulator.h"
#include "emulator_utilities.h"

/*
 * This allocates the memory and registers in the heap dynamically. The calloc() function initialises the allocated
 * space to 0 so there is no need to do this manually.
 */
State initialize_CPU() {
    State cpu;

    cpu.memory = calloc(NUM_MEMORY_LOCATIONS, sizeof(uint8_t));
    cpu.regs = calloc(NUM_REGISTERS, sizeof(uint32_t));

    return cpu;
}

/* Returns 0 if the address is within the range of existent memory addresses */ 
uint8_t memory_in_bounds(uint32_t addr) {

    switch(addr) {

        case GPIO_PINS_0_9 :
            printf("One GPIO pin from 0 to 9 has been accessed\n");
            return 0;

        case GPIO_PINS_10_19 :
            printf("One GPIO pin from 10 to 19 has been accessed\n");
            return 0;

        case GPIO_PINS_20_29 :
            printf("One GPIO pin from 20 to 29 has been accessed\n");
            return 0;

        case GPIO_CLEAR_ADDR :
            printf("PIN OFF\n");
            return 0;

        case GPIO_SET_ADDR :
            printf("PIN ON\n");
            return 0;

        default :
            break;
    }


    if (addr > (NUM_MEMORY_LOCATIONS - BYTES_PER_WORD)) {
        printf("Error: Out of bounds memory access at address 0x%08x\n", addr);
        return 1;
    }

    return 0;
}

/* Writes a value to memory at the specfied address */
void write_to_memory(State cpu, uint32_t addr, uint32_t val) {
    if (memory_in_bounds(addr) == 0) {
        switch (addr) {

            case GPIO_PINS_0_9 :
            case GPIO_PINS_10_19 :
            case GPIO_PINS_20_29 :
            case GPIO_CLEAR_ADDR :
            case GPIO_SET_ADDR :
                break;

            default :
                write_word(&cpu.memory[addr], val);
        }
    }
}

/* PRE: Memory address is in bounds */
uint32_t read_from_memory(State cpu, uint32_t addr) {

    switch(addr) {
        case GPIO_PINS_0_9 :
        case GPIO_PINS_10_19 :
        case GPIO_PINS_20_29 :
            return addr;

        default :
            return read_word(&cpu.memory[addr]);
    }

}

/* Returns 0 if the register being accessed is existent */
uint8_t register_in_bounds(uint32_t Rn) {
    if (Rn >= NUM_REGISTERS) {
        printf("Error: Out of bounds register access with register %u\n", Rn);
        return 1;
    }

    return 0;
}

/* Writes a value to a register specified by Rn */
void write_to_register(State cpu, uint32_t Rn, uint32_t val) {
    if (register_in_bounds(Rn) == 0) {
        write_register(&cpu.regs[Rn], val);
    }
}

/* PRE: Register index is in bounds */
uint32_t read_from_register(State cpu, uint32_t Rn) {
    return read_register(&cpu.regs[Rn]);
}

/* Increments the Program Counter to the next instruction
 * PC <- Pc + 4
 */
void increment_PC(State cpu) {
    cpu.regs[PC_INDEX] += BYTES_PER_WORD;
}

/* Returns the instruction pointed to by the Program Counter */
uint32_t get_next_instruction(State cpu) {
    return read_from_memory(cpu, read_register(&cpu.regs[PC_INDEX]));
}

/* Obtains the value of the Z bit from the CPSR */
uint32_t get_Z(uint32_t val) {
    return bits_extract(val, CPSR_Z_INDEX, CPSR_Z_INDEX + BIT_SIZE);
}

/* Obtains the value of the N bit from the CPSR */
uint32_t get_N(uint32_t val) {
    return bits_extract(val, CPSR_N_INDEX, CPSR_N_INDEX + BIT_SIZE);
}

/* Obtains the vlaue of the V bit from the CPSR */
uint32_t get_V(uint32_t val) {
    return bits_extract(val, CPSR_V_INDEX, CPSR_V_INDEX + BIT_SIZE);
}

/* Returns non zero if the condition is satisfied, 0 otherwise */
uint32_t check_condition(uint32_t instr, State cpu) {
    ConditionCode conditionCode = bits_extract(instr, CPSR_FLAGS_OFFSET, CPSR_FLAGS_OFFSET + NUM_CPSR_FLAGS);

    uint32_t CPSR = read_from_register(cpu, CPSR_INDEX);

    switch (conditionCode) {
	    								/* Satisfied if */

        case EQ :							/* Z set */
            return get_Z(CPSR);

        case NE : 							/* Z clear */
            return !get_Z(CPSR);

        case GE : 							/* N equal to V */
            if (get_N(CPSR) == get_V(CPSR)) {
                return 1;
            }
            return 0;

        case LT : 							/* N not equal to V */
            if (get_N(CPSR) != get_V(CPSR)) {
                return 1;
            }
            return 0;

        case GT :							/* Z clear AND (N equal to V) */
            if ((get_Z(CPSR) == 0) && (get_N(CPSR) == get_V(CPSR))) {
                return 1;
            }
            return 0;

        case LE :							/* Z set OR (N not equal to V) */
            if ((get_Z(CPSR) == 1) || (get_N(CPSR) != get_V(CPSR))) {
                return 1;
            }
            return 0;

        case AL :							/* Always satisfied */
            return 1;

        default :							/* Not satisfied */
            return 0;
    }
}

/*
 * Reads the instructions one word at a time (4 bytes) and loads these into
 * memory into contiguous locations, starting from 0x00000000.
 */ 
void read_instructions(FILE *file, State cpu) {

    uint32_t instr = 0;
    uint32_t size = get_size(file) / BYTES_PER_WORD; 		  /* Obtain the size in bytes of the file */

    for (uint32_t i = 0; i < size; i++) {
        if (fread(&instr, sizeof(uint32_t), 1, file) == 1) {      /* Read the next word from the file */
        	write_to_memory(cpu, i * BYTES_PER_WORD, instr);  /* Write the word to memory if it was read */
	}
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

/*
 * Simulates the execution of the instruction pipeline.
 * Initially the pipeline is empty. 
 * The first instruction is fetched.
 * That instruction is decoded, a function pointer to execute it is obtained and a new insruction is fetched.
 * The decoded instruction is executed, the fetched intruction is decoded and a new instruction is fetched.
 * This repeats until the all zero instruction (halt) instruction is encountered.
 */ 
void run_emulator(State cpu) {
    Stage pipelineStage;
    pipelineStage.decodedEmpty = 1; /* Initially no decoded instruction */
    pipelineStage.fetchedEmpty = 1; /* Initially no fetched instruction */
    pipelineStage.fetched = 0;
    pipelineStage.decoded = 0;

    while (pipelineStage.decoded != &halt) {                                            /* While the decoded instruction is not the halt one */

        if ((pipelineStage.decodedEmpty == 0) && (pipelineStage.decoded != NULL)) {     /* If there is a decoded instruction */

            uint32_t executed = pipelineStage.decoded(pipelineStage.decodedInstr, cpu); /* Call the function to execute it, storing the success of execution */
                                                                                        /*                               (1, for successful, 0 unsuccessful) */

            if ((pipelineStage.decoded == &branch) && (executed == 1)) {                /* If the executed instruction was a branch and it executed successfully */
                pipelineStage.decodedEmpty = 1;                                         /* Clear the pipeline                                                    */
                pipelineStage.fetchedEmpty = 1;
            }
        }

        if (pipelineStage.fetchedEmpty == 0) {                                          /* If there is a fetched instruction in the pipeline */

            pipelineStage.decoded = decode(pipelineStage.fetched);		        /* Decode the instruction and store the instruction bits for its future execution*/
            pipelineStage.decodedInstr = pipelineStage.fetched;

            if (pipelineStage.decodedEmpty == 1) {
                pipelineStage.decodedEmpty = 0;
            }

        } else {
            pipelineStage.fetchedEmpty = 0;
        }

        pipelineStage.fetched = get_next_instruction(cpu);                              /* Get the next instruction for the pipeline */
        increment_PC(cpu);                                                              /* Increment the Program Counter */
    }

    halt(0, cpu);								        /* After the pipeline finishes, call the halt function to output the final emulator state */
}

/* Frees the memory allocated to the memory and registers */
void free_emulator(State cpu) {
    free(cpu.regs);
    free(cpu.memory);
}
