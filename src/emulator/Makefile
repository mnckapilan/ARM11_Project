CC      = gcc
CFLAGS  = -Wall -std=c99 -Werror -pedantic
BUILD   = emulate

all:    $(BUILD)
clean:	
	/bin/rm -f $(BUILD) *.o core

emulate:		emulate.o emulator.o io.o registers.o memory.o multiply.o branch.o single_data_transfer.o data_processing.o utilities.o halt.o
emulate.o:		emulator.h
emulator.o:		memory.h multiply.h branch.h single_data_transfer.h data_processing.h halt.h registers.h io.h emulator.h
io.o:			io.h
registers.o: 		registers.h
data_processing.o:	data_processing.h utilities.h
single_data_transfer.o: single_data_transfer.h utilities.h
branch.o:		branch.h utilities.h
multiply.o:		multiply.h utilities.h
memory.o:		memory.h utilities.h
halt.o:			halt.h utilities.h
utilities.o:		utilities.h
