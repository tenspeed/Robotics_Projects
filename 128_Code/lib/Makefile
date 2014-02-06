#--------------------------------------------------------------------------------------
# File:    Makefile for an AVR project SUBDIRECTORY
#          The makefile is the standard way to control the compilation and linking of 
#          C/C++ files into an executable file. This makefile is also used to control
#          the downloading of the executable file to the target processor and the 
#          generation of documentation for the project. 
#
# Version:  4-11-2004 JRR Original file
#           6-19-2006 JRR Modified to use AVR-JTAG-ICE for debugging
#          11-21-2008 JRR Added memory locations and removed extras for bootloader
#          11-26-2008 JRR Cleaned up; changed method of choosing programming method
#          12-14-2009 JRR Made subdirectory version of Makefile
#
# Relies   The avr-gcc compiler and avr-libc library
# on:      The avrdude downloader, if downloading through an ISP port
#          AVR-Insight or DDD and avarice, if debugging with the JTAG port
#          Doxygen, for automatic documentation generation
#
# Copyright 2006-2009 by JR Ridgely.  This makefile is intended for use in educational 
# courses only, but its use is not restricted thereto. It is released under the terms 
# of the Lesser GNU Public License with no warranty whatsoever, not even an implied
# warranty of merchantability or fitness for any particular purpose. Anyone who uses 
# this file agrees to take all responsibility for any and all consequences of that use. 
#--------------------------------------------------------------------------------------

# This subdirectory Makefile is to be called by an upper directory Makefile which sets
# the various defines for compilation
LIB_OBJS = global_debug.o mechutil.o base232.o base_text_serial.o rs232int.o queue.o \
           stl_timer.o stl_task.o ff.o sd_card.o 

LIB_NAME = me405.a


#--------------------------------------------------------------------------------------
# Inference rules show how to process each kind of file. 
# How to compile a .c file into a .o file
.c.o:
	$(CC) -c -g $(OPTIM) -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(DBG) $(OTHERS) $<

# How to compile a .cc file into a .o file
.cc.o:
	$(CC) -c -g $(OPTIM) -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(DBG) $(OTHERS) $<

# How to compile a .cpp file into a .o file
.cpp.o:
	$(CC) -c -g $(OPTIM) -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(DBG) $(OTHERS) $<

#--------------------------------------------------------------------------------------
# This rule controls the linking of the library from object files. The target is saved
# in an archive file.  Also created is a listing file .lst. 
$(LIB_NAME):  $(LIB_OBJS)
# 	$(CC) $(OBJS) -g -mmcu=$(MCU) -o $(TARGET).elf
	@avr-ar -r $(LIB_NAME) $(LIB_OBJS)

#--------------------------------------------------------------------------------------
# Make the main file of this project. This target is invoked when the user just types 
# 'make' as opposed to 'make <target>.' This should be the first target in Makefile.

all:  # $(LIB_NAME)
	@echo 'MAKEFLAGS: $(MAKEFLAGS)'
	@echo 'MAKEOVERRIDES: $(MAKEOVERRIDES)'

#--------------------------------------------------------------------------------------
# 'make doc' will use Doxygen to create documentation for the project.

doc:
	doxygen doxygen.conf

#--------------------------------------------------------------------------------------
# 'make clean' will erase the compiled files, listing files, etc. so you can
# restart the building process from a clean slate.

clean:
	rm -f *.o $(LIB_NAME) *~
