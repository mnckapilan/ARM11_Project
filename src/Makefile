CC      = gcc
CFLAGS  = -Wall -std=c99 -Werror -pedantic
BUILD   = emulate assemble

all:    $(BUILD)
clean:	
	/bin/rm -f $(BUILD) *.o core

emulate:		emulate.o emulator.o emulator_io.o emulator_registers.o emulator_memory.o emulator_multiply.o emulator_branch.o emulator_data_transfer.o emulator_block_data_transfer.o emulator_single_data_transfer.o emulator_data_processing.o emulator_utilities.o emulator_halt.o
emulate.o:		emulator.h
emulator.o:		emulator.h emulator_io.h emulator_registers.h emulator_memory.h emulator_multiply.h emulator_branch.h emulator_block_data_transfer.h emulator_single_data_transfer.h emulator_data_processing.h emulator_utilities.h emulator_halt.h
emulator_io.o:		emulator_io.h emulator_utilities.h
emulator_registers.o: 	emulator_registers.h emulator_utilities.h
emulator_data_processing.o: emulator_data_processing.h emulator_utilities.h
emulator_data_transfer.o:   emulator_data_transfer.h emulator_utilities.h
emulator_block_data_transfer.o:  emulator_block_data_transfer.h emulator_data_transfer.h
emulator_single_data_transfer.o: emulator_single_data_transfer.h emulator_data_transfer.h
emulator_branch.o:	emulator_branch.h emulator_utilities.h
emulator_multiply.o:	emulator_multiply.h emulator_utilities.h
emulator_memory.o:	emulator_memory.h emulator_utilities.h
emulator_halt.o:	emulator_halt.h emulator_utilities.h
emulator_utilities.o:	emulator_utilities.h

assemble:               assemble.o assembler_branch.o assembler_data_processing.o  assembler_multiply.o assembler_single_data_transfer.o assembler_special.o assembler_utilities.o assembler_block_data_transfer.o
assemble.o:             assembler_branch.h assembler_data_processing.h assembler_multiply.h assembler_single_data_transfer.h assembler_special.h assembler_utilities.h assembler_block_data_transfer.h
assembler_branch.o:     assembler_branch.h assembler_utilities.h
assembler_data_processing.o : assembler_data_processing.h assembler_utilities.h
assembler_multiply.o:   assembler_multiply.h assembler_utilities.h
assembler_single_data_transfer.o: assembler_single_data_transfer.h assembler_utilities.h
assembler_special.o:    assembler_special.h assembler_utilities.h assembler_data_processing.h
assembler_block_data_transfer.o: assembler_block_data_transfer.h assembler_utilities.h
assembler_utilities.o:  assembler_utilities.h
