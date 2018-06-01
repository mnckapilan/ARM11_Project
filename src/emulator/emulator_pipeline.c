#include "emulator_pipeline.h"
#include "emulator_multiply.h"
#include "emulator_branch.h"
#include "emulator_halt.h"
#include "emulator_single_data_transfer.h"

#define DATA_PROCESSING_MASK 0x0c000000
#define MULTIPLY_MASK 0x0fc000f0
#define SINGLE_DATA_TRANSFER_MASK 0x0c600000
#define BRANCH_MASK 0x0f000000
#define DATA_PROCESSING_FORMAT 0
#define MULTIPLY_FORMAT 0x00000090
#define SINGLE_DATA_TRANSFER_FORMAT 0x04000000
#define BRANCH_FORMAT 0x0a000000

/*
 * The order of the decoding is set out such that the multiply mask is applied before the
 * data processing one since a multiply instruction would be categorized as a data processing
 * one if applied in the reverse order.
 */
instr_ptr decode(uint32_t instr) {
    if ((instr & MULTIPLY_MASK) == MULTIPLY_FORMAT)
        return &multiply;
    else if ((instr & SINGLE_DATA_TRANSFER_MASK) == SINGLE_DATA_TRANSFER_FORMAT)
        return &single_data_transfer;
    else if ((instr & BRANCH_MASK) == BRANCH_FORMAT)
        return &branch;
    else if ((instr & DATA_PROCESSING_MASK) == DATA_PROCESSING_FORMAT)
        return NULL; /*Data Processing */

    fprintf(stderr, "Instruction format not recognised");
    return NULL;
}

void run_pipeline(struct CPUState cpu) {
    struct PipelineStage pipelineStage;
    pipelineStage.decodedEmpty = 0;
    pipelineStage.fetchedEmpty = 0;
    pipelineStage.fetched = 0;
    pipelineStage.decoded = 0;

    while (pipelineStage.decoded != &halt) {
        if (pipelineStage.decodedEmpty == 1) {
            if (pipelineStage.decoded != NULL)
                pipelineStage.decoded(pipelineStage.decodedInstr, cpu);
        }

        if (pipelineStage.fetchedEmpty == 1) {

            pipelineStage.decoded = decode(pipelineStage.fetched);
            pipelineStage.decodedInstr = pipelineStage.fetched;
            if (pipelineStage.decodedEmpty == 0) {
                pipelineStage.decodedEmpty = 1;
            }

        } else {
            pipelineStage.fetchedEmpty = 1;
        }

        pipelineStage.fetched = get_next_instruction(cpu);
        increment_PC(cpu);
    }

    halt(0, cpu);
}
