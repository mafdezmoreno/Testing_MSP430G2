OBJECTS=blink.o
MAP=blink.map
MAKEFILE=Makefile

ifeq ($(OS),Windows_NT)
	ifeq ($(shell uname -o),Cygwin)
		RM= rm -rf
	else
		RM= del /q
	endif
else
	RM= rm -rf
endif

#GCC_DIR = $(abspath $(dir $(lastword $(MAKEFILE)))/../../bin)
GCC_DIR  = ${HOME}/ti/msp430-gcc/include
#SUPPORT_FILE_DIRECTORY = $(abspath $(dir $(lastword $(MAKEFILE)))/../../include)
SUPPORT_FILE_DIRECTORY = ${HOME}/ti/msp430-gcc/include
DEVICE  = MSP430G2452
#CC      = $(GCC_DIR)/msp430-elf-gcc
CC = msp430-elf-gcc # Added to path
#GDB     = $(GCC_DIR)/msp430-elf-gdb
GDB = msp430-elf-gdb #Added to path

CFLAGS = -I $(SUPPORT_FILE_DIRECTORY) -mmcu=$(DEVICE) -Og -Wall -g
LFLAGS = -L $(SUPPORT_FILE_DIRECTORY) -Wl,-Map,$(MAP),--gc-sections 

all: ${OBJECTS}
	$(CC) $(CFLAGS) $(LFLAGS) $? -o $(DEVICE).out

clean: 
	$(RM) $(OBJECTS)
	$(RM) $(MAP)
	$(RM) *.out
	
debug: all
	$(GDB) $(DEVICE).out
