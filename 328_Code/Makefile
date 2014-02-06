#--------------------------------------------------------------------------------------
# File:    Makefile for an AVR project
#          The makefile is the standard way to control the compilation and linking of 
#          C/C++ files into an executable file. This makefile is also used to control
#          the downloading of the executable file to the target processor and the 
#          generation of documentation for the project. 
#
# Version:  4-11-2004 JRR Original file
#           6-19-2006 JRR Modified to use AVR-JTAG-ICE for debugging
#          11-21-2008 JRR Added memory locations and removed extras for bootloader
#          11-26-2008 JRR Cleaned up; changed method of choosing programming method
#          11-14-2009 JRR Added make support to put library files into subdirectory
#	   01-04-2011 THC Configured for ME405 Board
#	   03-29-2011 THC Configured for Atmega32 in a protoboard with the USBTiny
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

# The name of the program you're building, and the list of object files. There must be
# an object file listed here for each *.c or *.cc file in your project. The make
# program will automatically figure out how to compile and link your C or C++ files 
# from the list of object files. TARGET will be the name of the downloadable program.
TARGET = hello_world
OBJS = $(TARGET).o

# Clock frequency of the CPU, in Hz. This number should be an unsigned long integer.
# For example, 16 MHz would be represented as 16000000UL. For ME405 boards, clocks are
# usually 4MHz for Ministrone and Swoop sensor boards, 8MHz for single-motor ME405 
# boards, and 16MHz for dual-motor ME405 boards
F_CPU = 20000000UL

# These codes are used to switch on debugging modes if they're being used. Several can
# be placed on the same line together to activate multiple debugging tricks at once.
# -DSERIAL_DEBUG       For general debugging through a serial device
# -DSTL_TRACE          For printing state transition traces on a serial device
# -DSTL_PROFILE        For doing profiling, measurement of how long tasks take to run
DBG = 

# Other codes, used for turning on special features in a given project, can be put here
OTHERS = 

# This define is used to choose the type of programmer from the following options: 
# bsd        - Parallel port in-system (ISP) programmer using SPI interface on AVR
# jtagice    - Serial or USB interface JTAG-ICE mk I clone from ETT or Olimex
# bootloader - Resident program in the AVR which downloads through USB/serial port
# PROG = bsd
# PROG = jtagice
# PROG = bootloader
PROG = usbtiny

# These defines specify the ports to which the downloader device is connected. 
# PPORT is for "bsd" on a parallel port, lpt1 on Windows or /dev/parport0 on Linux.
# JPORT is for "jtagice" on a serial port such as com1 or /dev/ttyS0, or usb-serial 
#       such as com4 or /dev/ttyUSB1, or aliased serial port such as /dev/avrjtag
# BPORT is for "bootloader", the USB/serial port program downloader on the AVR
# The usbtiny programmer doesn't need a port specification; it has a USB identifier
PPORT = /dev/parport0
JPORT = /dev/ttyUSB1
BPORT = /dev/ttyUSB0

# These are the name of the library file and of the subdirectory in which it is kept
LIB_FILE = me405.a
LIB_DIR = lib

#--------------------------------------------------------------------------------------
# This section specifies the type of CPU; uncomment one line for your processor. To add
# a new chip to the file, put its designation here and also define a code for CHIP in
# the section below. See the avrdude manual page for the code to use as CHIP. 

# MCU = atmega128
# MCU = atmega164p
# MCU = atmega324p
# MCU = atmega644
# MCU = atmega8
# MCU = at90s2313
# MCU = atmega8535
  MCU = atmega328p

################ End of the stuff the user is expected to need to change ##############

#--------------------------------------------------------------------------------------
# In this section, various variables are automatically set to match the MCU choice
# above. The CHIP variable is used by the downloader and must match MCU; fuse bytes are
# set some common settings for each processor and sometimes need to be adjusted.
# New chip specifications can be added to this file as needed. 

# ATmega128 set up for ME405 board with JTAG enabled
ifeq ($(MCU), atmega128)
  CHIP = m128
  EFUSE = 0xFF
  HFUSE = 0x11
  LFUSE = 0xEF
else ifeq ($(MCU), atmega164p)
  CHIP = m164p
  EFUSE = 0xFF
  HFUSE = 0x11
  LFUSE = 0xEF
# ATmega32 configured for Swoop sensor board with JTAG disabled to save power
else ifeq ($(MCU), atmega32)
  CHIP = m32
  EFUSE = 
  HFUSE = 0xD9
  LFUSE = 0xEF
# ATmega324P configured for Swoop sensor board with JTAG disabled
# Standard fuses FF19EF, bootloader fuses FFC8EF, low power fuses FF11EF
else ifeq ($(MCU), atmega324p)
  CHIP = m324p
  EFUSE = 0xFF
  HFUSE = 0x11
  LFUSE = 0xEF
# ATmega644 (note: the 644P needs a different MCU)
else ifeq ($(MCU), atmega644)
  CHIP = m644
  EFUSE = 0xFF
  HFUSE = 0x11
  LFUSE = 0xEF
# ATmega8 configured for Ministrone strain gauge interface boards
else ifeq ($(MCU), atmega8)
  CHIP = m8
# ATmega2313 configuration in case somebody has a few old ones lying around
else ifeq ($(MCU), at90s2313)
  CHIP = 2313
# ATmega8535 configuration for use with old chips gathering dust
else ifeq ($(MCU), atmega8535)
  CHIP = m8535
# ATmega328p commonly used in Arduino boards  
else ifeq ($(MCU), atmega328p)
  CHIP = m328p
  EFUSE = 0xFF
  HFUSE = 0xD9
  LFUSE = 0xF7
endif

#--------------------------------------------------------------------------------------
# Tell the compiler how hard to try to optimize the code. Optimization levels are:
# -O0  Don't try to optimize anything (even leaves empty delay loops in)
# -O1  Some optimizations; code usually smaller and faster than O0
# -O2  Pretty high level of optimization; often good compromise of speed and size
# -O3  Tries really hard to make code run fast, even if code size gets pretty big
# -Os  Tries to make code size small. Sometimes -O1 makes it smaller, though(?!)
OPTIM = -O2

# Define which compiler to use (CC) and some standard compiler options (STD).
CC = avr-gcc
STD = _GNU_SOURCE

# Usually this is just left at debugging level 0
DEBUGL = DEBUG_LEVEL=0

# The JTAG bitrate and IP port are used to configure the JTAG-ICE debugger
JTAGBITRATE = 1000000
IPPORT = :4242

# Any other compiler switches go here, for example short enumerations to save memory
OTHERS += -fshort-enums

#--------------------------------------------------------------------------------------
# This command exports the definitions of variables in this file so that Makefiles in
# subdirectories can make use of these settings. If you add variables above, they may
# need to be added here. Exception: stuff for the downloader is only used in this file.
export CC STD DEBUGL OPTIM MCU F_CPU DBG OTHERS

#--------------------------------------------------------------------------------------
# Inference rules show how to process each kind of file. 

# How to compile a .c file into a .o file
.c.o:
	$(CC) -c -g $(OPTIM) -mmcu=$(MCU) -I ./$(LIB_DIR) \
	-DF_CPU=$(F_CPU) $(DBG) $(OTHERS) $<

# How to compile a .cc file into a .o file
.cc.o:
	$(CC) -c -g $(OPTIM) -mmcu=$(MCU) -I ./$(LIB_DIR) \
	-DF_CPU=$(F_CPU) $(DBG) $(OTHERS) $<

# How to compile a .cpp file into a .o file
.cpp.o:
	$(CC) -c -g $(OPTIM) -mmcu=$(MCU) -I ./$(LIB_DIR) \
	-DF_CPU=$(F_CPU) $(DBG) $(OTHERS) $<

# This rule controls the linking of the target program from object files. The target 
# is saved as an ELF debuggable binary, downloadable .hex, and raw binary .bin.  Also
# created is a listing file .lst, which shows generated machine and assembly code.
$(TARGET).elf:  $(OBJS)
	$(CC) $(OBJS) $(LIB_FILE) -g -mmcu=$(MCU) -o $(TARGET).elf
	@avr-objdump -h -S $(TARGET).elf > $(TARGET).lst
	@avr-objcopy -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex
	@avr-objcopy -j .text -j .data -O binary $(TARGET).elf $(TARGET).bin
	@avr-size $(TARGET).elf

# This rule makes the files in the library subdirectory. It is normally called when
# one is first building stuff after downloading the project tree, or after updating 
# library files
$(LIB_FILE):  
	$(MAKE) -C $(LIB_DIR)
	mv -f $(LIB_DIR)/me405.a .

#--------------------------------------------------------------------------------------
# Make the main file of this project.  This target is invoked when the user just types 
# 'make' as opposed to 'make <target>.'  This should be the first target in Makefile.

all:  $(LIB_FILE) $(TARGET).elf

#--------------------------------------------------------------------------------------

#--------------------------------------------------------------------------------------
# This is a dummy target that doesn't do anything. It's included because the author 
# belongs to a faculty labor union and has been instilled with reverence for laziness.

nothing:

#--------------------------------------------------------------------------------------
# 'make install' will make the project, then download the program using whichever 
# method has been selected -- ISP cable, JTAG-ICE module, or USB/serial bootloader

install:  $(LIB_FILE) $(TARGET).elf
  ifeq ($(PROG), bsd)
	avrdude -p $(CHIP) -P $(PPORT) -c bsd -V -E noreset -Uflash:w:$(TARGET).hex
  else ifeq ($(PROG), jtagice)
	avarice -e -p -f $(TARGET).elf -j $(JPORT)
  else ifeq ($(PROG), bootloader)
	ruby bootloader.rb $(BPORT) $(TARGET).bin
  else ifeq ($(PROG), usbtiny)
	avrdude -p $(CHIP) -c usbtiny -V -E noreset -Uflash:w:$(TARGET).hex
  else
	@echo "ERROR: No programmer" $(PROG) "in the Makefile"
  endif

#--------------------------------------------------------------------------------------
# 'make debug' will make the project, then download it with the JTAG-ICE-I interface
# and start up a debugger. The JTAG-ICE-I only works with the ATmega32, ATmega128, and
# a few other somewhat older processors; it won't work with the ATmega324/644.

debug:  $(TARGET).elf $(GDBINITFILE)
	@avarice --capture -B $(JTAGBITRATE) -j $(JPORT) $(IPPORT) &
	@sleep 1
	@ddd --debugger "avr-gdb -x $(GDBINITFILE)"
	@killall -q avarice || /bin/true

$(GDBINITFILE): $(TARGET).hex
	@echo "file $(TARGET).elf"            > $(GDBINITFILE)
	@echo "target remote localhost:4242" >> $(GDBINITFILE)
	@echo "load"                         >> $(GDBINITFILE) 
	@echo "break main"                   >> $(GDBINITFILE)
	@echo "continue"                     >> $(GDBINITFILE)

#--------------------------------------------------------------------------------------
# 'make term' will run the serial port emulator GTK-Term, a hacked version of which
# can be used to download programs to the bootloader with the ctrl-D command.

term:  $(TARGET).elf
	@gtkterm --to-send $(TARGET).bin &

#--------------------------------------------------------------------------------------
# 'make fuses' will set up the processor's fuse bits in a "standard" mode. Standard is
# a setup in which there is no bootloader but the ISP and JTAG interfaces are enabled. 

fuses: nothing
  ifeq ($(PROG), bsd)
	avrdude -p $(CHIP) -P $(PPORT) -c $(PROG) -V -E noreset -Ulfuse:w:$(LFUSE):m 
	avrdude -p $(CHIP) -P $(PPORT) -c $(PROG) -V -E noreset -Uhfuse:w:$(HFUSE):m 
	avrdude -p $(CHIP) -P $(PPORT) -c $(PROG) -V -E noreset -Uefuse:w:$(EFUSE):m
  else ifeq ($(PROG), jtagice)
	avarice -e -j $(JPORT) --write-fuses EF19EF
	# FF11EF
  else ifeq ($(PROG), usbtiny)
	avrdude -p $(CHIP) -c usbtiny -q -V -Ulfuse:w:$(LFUSE):m
	avrdude -p $(CHIP) -c usbtiny -q -V -Uhfuse:w:$(HFUSE):m
	avrdude -p $(CHIP) -c usbtiny -q -V -Uefuse:w:$(EFUSE):m
  else
	@echo "ERROR: Only bsd/ISP, JTAG-ICE, or USBtiny can program fuse bytes"
  endif

#--------------------------------------------------------------------------------------
# 'make readfuses' will see what the fuses currently hold

readfuses: nothing
  ifeq ($(PROG), bsd)
	@echo "ERROR: Not yet programmed to read fuses with bsd/ISP cable"
  else ifeq ($(PROG), jtagice)
	@avarice -e -j $(JPORT) --read-fuses
  else ifeq ($(PROG), bootloader)
	@echo "ERROR: Not yet programmed to read fuses via bootloader"
  else
	@echo "ERROR: No known device specified to read fuses"
  endif

#-----------------------------------------------------------------------------
# 'make reset' will read a byte of lock bits, ignore it, and reset the chip
# This can be used to un-freeze the chip after a 'make freeze'
reset:
  ifeq ($(PROG), bsd)
	avrdude -c bsd -p $(CHIP) -P $(PPORT) -c $(PROG) -V -E noreset \
		-Ulfuse:r:/dev/null:r
  else ifeq ($(PROG), usbtiny)
	avrdude -p $(CHIP) -c usbtiny -q -V -E noreset -Ulfuse:r:/dev/null:r
  else
	@echo "ERROR: make reset only works with parallel ISP cable"
  endif

#-----------------------------------------------------------------------------
# 'make freeze' will read a byte of lock bits and leave the chip in reset
freeze:
  ifeq ($(PROG), bsd)
	avrdude -c bsd -p $(CHIP) -P $(PPORT) -c $(PROG) -V -E reset \
		-Ulfuse:r:/dev/null:r
#   else ifeq ($(PROG), usbtiny)
# 	avrdude -p $(CHIP) -c usbtiny -q -V -E reset -Ulfuse:r:/dev/null:r
  else
	@echo "ERROR: make freeze only works with parallel ISP cable"
  endif

#--------------------------------------------------------------------------------------
# 'make doc' will use Doxygen to create documentation for the project.

doc:  $(TARGET).elf
	doxygen doxygen.conf

#--------------------------------------------------------------------------------------
# 'make clean' will erase the compiled files, listing files, etc. so you can restart
# the building process from a clean slate. It's also useful before committing files to
# a CVS or SVN or Git repository. 

clean:
	rm -f *.o $(TARGET).hex $(TARGET).lst $(TARGET).elf $(TARGET).bin *~

#--------------------------------------------------------------------------------------
# 'make cleaner' will clean the current directory and subdirectories as well. This
# cleanup should be performed before archiving things in an SVN/CVS/Git repository.

cleaner:  clean
	$(MAKE) -C $(LIB_DIR) clean
	rm -f $(LIB_FILE)
	rm -fr html rtf latex

#--------------------------------------------------------------------------------------
# 'make help' will show a list of things this makefile can do

help:
	@echo 'make          - Build program file ready to download'
	@echo 'make install  - Build program and download with parallel ISP cable'
	@echo 'make debug    - Build program and debug using JTAG-ICE module'
	@echo 'make freeze   - Stop processor with parallel cable RESET line'
	@echo 'make reset    - Reset processor with parallel cable RESET line'
	@echo 'make doc      - Generate documentation with Doxygen'
	@echo 'make clean    - Remove compiled files in topmost directory'
	@echo 'make cleaner  - Clean subdirectories too; use before archiving files'
	@echo ' '
	@echo 'Notes: 1. Other less commonly used targets are in the Makefile'
	@echo '       2. You can combine targets, as in "make clean all"'
