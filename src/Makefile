CC      = gcc
CFLAGS  = -Wall -std=c99 -Werror -pedantic
BUILD   = emulate

all:    $(BUILD)
clean:	
	/bin/rm -f $(BUILD) *.o core

emulate:		emulate.o emulator.o emulator_io.o emulator_registers.o emulator_memory.o emulator_multiply.o emulator_branch.o emulator_data_transfer.o emulator_block_data_transfer.o emulator_single_data_transfer.o emulator_data_processing.o emulator_utilities.o emulator_halt.o
emulate.o:		emulator.h
emulator.o:		emulator.h emulator_io.h emulator_registers.h emulator_memory.h emulator_multiply.h emulator_branch.h emulator_block_data_transfer.h emulator_single_data_transfer.h emulator_data_processing.h emulator_utilities.h emulator_halt.h
io.o:			emulator_io.h emulator_utilities.h
registers.o: 		emulator_registers.h emulator_utilities.h
data_processing.o:	emulator_data_processing.h emulator_utilities.h
data_transfer.o:        emulator_data_transfer.h emulator_utilities.h
block_data_transfer.o:  emulator_block_data_transfer.h emulator_data_transfer.h
single_data_transfer.o: emulator_single_data_transfer.h emulator_data_transfer.h
branch.o:		emulator_branch.h emulator_utilities.h
multiply.o:		emulator_multiply.h emulator_utilities.h
memory.o:		emulator_memory.h emulator_utilities.h
halt.o:			emulator_halt.h emulator_utilities.h
utilities.o:		emulator_utilities.h
