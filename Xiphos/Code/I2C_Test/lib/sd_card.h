//*************************************************************************************
/** \file sd_card.h
 *    This file contains a class which allows SD cards to be read and written.  The
 *    intent is that SD cards can be convenient mechanisms for the storage of lots and
 *    lots of data. In fact, the base_text_serial class is used as an ancestor, so the
 *    SD card can be used to write data as text using "<<" operators. 
 *
 *    This version of the sd_card driver is based on ELM-FAT-FS, a package of DOS file
 *    system drivers found at http://elm-chan.org/fsw/ff/00index_e.html. It was chosen
 *    because it is open source and is currently being maintained, while some other
 *    packages are in a more static state, their authors having presumably moved on. 
 *
 *  Revisions:
 *    \li 06-09-2008 JRR Class created, based on C program example found at
 *                       http://www.captain.at/electronic-atmega-mmc.php
 *    \li 08-17-2008 JRR Actually found the time to get it working
 *    \li 12-09-2008 JRR Added FAT16 filesystem support using DOSFS package from
 *                       http://www.larwe.com/zws/products/dosfs/index.html
 *    \li 02-26-2009 JRR Found time to fix lots of bugs in DOSFS version
 *    \li 11-21-2009 JRR The DOSFS code is still buggy; trying ELM-FAT-FS version from
 *                       http://elm-chan.org/fsw/ff/00index_e.html
 *    \li 12-16-2009 JRR The ELM-FAT-FS version seems reasonably debugged and usable
 *
 *  License:
 *    This file is free software released under the Lesser GNU Public License, version
 *    2. This program is intended for educational use only, but it is not limited 
 *    thereto. It is provided without any guarantee whatsoever. Because the author has 
 *    no control over how this software may be used, the author cannot be responsible
 *    for any consequences of the use of this software. The ELM-FAT-FS package on which
 *    this program relies is subject to its own free software license, and any user of
 *    this software must agree to the terms of that license as well as the LGPL. See
 *    the ELM-FAT-FS link above for details about the license governing DOSFS use. 
 */
//*************************************************************************************

/// This define prevents this .h file from being included more than once in a .cc file
#ifndef _SD_CARD_H_
#define _SD_CARD_H_

#include <stdlib.h>							// Standard C functions
#include <avr/io.h>							// Definitions of special function regs.
#include "base_text_serial.h"				// Pull in the base class header file
#include "global_debug.h"					// Header for global serial debugging
#include "queue.h"							// Template for first-in-first-out buffer
extern "C"
{
	#include "ff.h"							// Header for DOS filesystem driver
	#include "diskio.h"						// The ELM-FAT-FS header for disk I/O
}


/** This is the size of blocks (like sectors) in bytes on the SD card. All writes 
 *  are done with blocks of data of this size, and the sd_card object must have
 *  a buffer which holds this many bytes. SD cards have a block size of 512 bytes.
 */

#define SD_BLOCK_SIZE		512

/** This is the size of a buffer which holds the characters in one line to be written
 *  to the SD card. Characters are written into the line buffer until an end of line 
 *  character is received (or some other character set with SD_SEND_NOW_CHAR) or 
 *  someone calls transmit_now(); then the data in the buffer is written to the card.
 */

#define SD_LINE_BUF_SIZE	128


/** This macro sets the type of the line buffer index, which needs to be 8 bits if the
 *  line buffer is less than 256 elements in size or 16 bits if it's larger.
 */

#if (SD_LINE_BUF_SIZE < 256)
	#define SD_L_BUF_IDX_T	uint8_t
#else
	#define SD_L_BUF_IDX_T	uint16_t
#endif


/** This macro chooses the port to which the SD card's card select bit is attached.
 */

#define SD_CS_PORT			PORTB


/** This macro chooses the data direction register for the port to which the SD card's
 *  card select bit is attached. It must match the port in SD_CS_PORT.
 */

#define SD_CS_DDR			DDRB


/** This macro sets a bitmask for the bit which is used as the SD card's card select
 *  bit.
 */

#define SD_CS_MASK			(1 << 0)


/** This is a character which will cause a line of data to be written to the SD card.
 *  If, for example, it is set to '\n', then whenever a newline character is written
 *  the data will be flushed to the card. Set this macro if data is written 
 *  infrequently to the card; this will help prevent data loss if power goes out
 *  after some data is taken but before the data can be written. However, if data is
 *  to be written frequently or in high volume, it is best to undef this macro because
 *  writing small amounts of data to the card wastes a lot of time and power and tends
 *  to wear out flash cards prematurely.
 */

// #define SD_SEND_NOW_CHAR	'\n'
#undef SD_SEND_NOW_CHAR


//-------------------------------------------------------------------------------------
/** This definition enables the correct port pins for the SPI port. The MISO pin is set
 *  as an input; MOSI, SCK, and SS are outputs. Note that even though SS is often not
 *  used by this routine, it needs to be set as an output, otherwise if anything drives
 *  it low, the SPI port automatically reconfigures itself as a slave and stops working
 *  as an interface to the SD card. Things seem more reliable with the pullup on MISO
 *  turned on also. 
 *
 *  The pins may vary by chip, so we define which pins are set as inputs and outputs
 *  on a chip by chip basis. The current list of recognized chips:
 *  \li AT90S2313, ATmega8, ATmega8535: Not enough RAM and can't be used with this
 *      version; the no-filesystem version might be usable on chips with RAM >= 1KB
 *  \li ATmega32, ATmega324(P), ATmega644(P): These work, but only very small programs
 *      can fit on the 32 and 324(P); chips such as the 644, with 4KB RAM, are best
 *  \li ATmega128: Not supported yet but should work well
 */

#if (defined __AVR_AT90S2313__ || defined __AVR_ATmega8__ \
	|| defined __AVR_ATmega8535__)
	#define MM_SETUP_PORT() 
	#warning "SD card with filesystem uses too much RAM to fit on this CPU"
#endif
#if (defined __AVR_ATmega644__ || defined __AVR_ATmega644P__ \
	|| defined __AVR_ATmega324__ || defined __AVR_ATmega324P__ \
	|| defined __AVR_ATmega32__ || defined __AVR_ATmega164P__)
	#define MM_SETUP_PORT() DDRB &= ~0x40; DDRB |= 0xB0; PORTB |= 0x40
#endif
#if (defined __AVR_ATmega128__ || defined __AVR_Atmega1281__)
	#warning "SPI pins for SD card not yet tested for this processor"
	#define MM_SETUP_PORT() DDRB &= ~0x08; DDRB |= 0x07; PORTB |= 0x08
#endif


//-------------------------------------------------------------------------------------
/** This union type helps to easily access the bytes in a 16-bit unsigned short 
 *  integer. 
*/

typedef union
{
	unsigned short int word;				///< The whole 16 bits
	unsigned char byte[2];				  ///< The bytes in the word as an array
} sd_word;


//-------------------------------------------------------------------------------------
/** This union type helps to easily access the bytes in a 32-bit unsigned long integer.
 */

typedef union
{
	unsigned long int dword;				///< The whole 32 bits
	unsigned char byte[4];				  ///< The bytes in the long as an array
} sd_u32;


//-------------------------------------------------------------------------------------
/** This class implements an interface to an SD card. Data is stored up in a buffer, 
 *  then written to the card when the buffer becomes full or when the send_now 
 *  modifier is inserted into the SD card object's output stream. (Alternatively, one
 *  can specify that carriage returns cause writing to the card using the macro 
 *  SD_SEND_NOW_CHAR in sd_card.h.) The SPI interface to the SD card is used, as it is
 *  much simpler and requires less wires than the full high-speed interface. The SPI
 *  interface is a bit slow, though. 
 *
 *  The ELM-FAT-FS library is used for its low-level and FAT file system drivers. This
 *  library can be found at http://elm-chan.org/fsw/ff/00index_e.html. The filesystem 
 *  library interfaces with the sd_card class through two sets of C functions. The 
 *  first set of functions is in files ff.h and ff.c; these functions provide the
 *  functionality of a file system: initializing and mounting volumes, opening files
 *  and directories, reading and writing data, and so on. The second set of functions
 *  is a set of callback functions which are provided in sd_card.cc. Written in C, 
 *  these callbacks are called by the ELM-FAT-FS library to provide platform and media
 *  specific access; they allow the AVR processor to work with the SD card. Most of 
 *  the C functions in sd_card.cc are slightly modified versions of functions from the
 *  AVR-specific example files distributed with the ELM-FAT-FS library. 
 * 
 *  \section sd_prep_card Preparing an SD Card for Use
 *    The SD cards used by this program should be formatted as MS-DOS style volumes in
 *    the "floppy disk" style, not the "hard disk" style. This means that there should
 *    not be a partition table, with one or more partitions each having a filesystem. 
 *    Instead, the disk's main volume should be formatted. Many SD cards come formatted
 *    in the wrong style when purchased; the following steps will help determine if a
 *    card is formatted in the right or wrong way: 
 *    \li Plug the card into a Linux computer. 
 *    \li Open the card in a file browser; ignore the contents and close the browser. 
 *        Opening the card caused it to be automatically mounted, which is why it was
 *        worth doing. 
 *    \li Open a terminal window (konsole or gnome-terminal) and type 'mount'. Look 
 *        near the end of the bunch of text which comes up for a line that looks like
 *        this: \n <tt>/dev/sde1 on /media/disk type vfat
 *        (rw,nosuid,nodev,uhelper=hal,uid=1000,utf8,shortname=mixed,flush)</tt>
 *    \li If the code <tt>/dev/xxxx</tt> on the left of the line ends with a number, 
 *        such as <tt>/dev/sde1</tt>, you have the wrong kind of formatting; it will 
 *        need to be reformatted. If the code has no number, such as <tt>/dev/sdf</tt>,
 *        then you should be able to use the card as is. 
 *    \li The device name is usually <tt>/dev/sdxx</tt>, but on some computers the name 
 *        may be different, such as <tt>/dev/mmcblk0</tt>. In this case, partitions may
 *        be named like <tt>/dev/mmcblk0p1</tt>, and the device name is actually 
 *        <tt>/dev/mmcblk0</tt> including the "0" at the end. 
 *    \li Unmount the card. In KDE, use the device mounter's "eject" button; in Gnome
 *        right-click on the device icon and choose unmount. 
 *
 *    The following process will format an SD card in "floppy disk style" on Linux: 
 *    \li Get to a Linux computer on which you have root (sudo) access. You need root
 *        access in order to write directly to the SD card. 
 *    \li Plug the card into an SD/MMC slot in the computer. Do not open it up in a
 *        file manager or mount it. 
 *    \li Repeat the steps for checking the card's formatting if necessary to find the
 *        device name <tt>/dev/xxxn</tt> for the card. Note that in <tt>/dev/xxxn</tt>,
 *        the "\c xxx" is letters (for example sdf) and the "\t n" is a number, almost
 *        always \c 1. 
 *    \li <b>WARNING:</b> You need to be very careful in the following command: <b>Do
 *        not enter the designation of one of your computer's hard drives</b>, as this
 *        command would then reformat your system drive and effectively wipe out all 
 *        the data on that drive. 
 *    \li Run the following command: \n
 *        <tt>sudo mkfs -t msdos -I /dev/xxx</tt>  \n Note that
 *        the number is left off the device name. What you are doing is formatting the
 *        disk without any partitions; the numbers referred to partitions on the disk. 
 *    \li Remove the card and re-insert it in the computer. Make sure you can open it
 *        in a file manager as usual. 
 *    \li Unmount the card, remove it from the PC, and you're ready to use it to save
 *        some data. 
 *
 *  \section sd_card_drv Using the SD Card Driver Software
 *    In order to use the SD card driver, one must have an AVR board with an SD card
 *    slot, and the board must be configured to operate with Vcc at 3.3 volts. It is
 *    possible that multiple SD cards could be used on the same board, but this has 
 *    not been tested, and it would be rather weird to want to do so. Hot swapping the
 *    SD card is possible, but it's usually not a very good idea because of hardware
 *    considerations: SD cards have big internal power supply capacitance and tend to
 *    inflict brownouts on the AVR when plugged in. Also, making sure that all your
 *    reads and writes are finished before swapping the card can be a problem. 
 *
 *    Configuration is done by modifying values in sd_card.h and ffconf.h. Recommended
 *    settings for ffconf.h (which configures the filesystem drivers) include the 
 *    following:
 *    <table border="0" cellspacing="1">
 *      <tr><td> _FS_TINY </td><td> 0 </td><td> unless you have >= 4K SRAM </td></tr>
 *      <tr><td> _FS_READONLY </td><td> 0 </td><td> to enable writing </td></tr>
 *      <tr><td> _FS_MINIMIZE </td><td> 1 </td><td> to save space </td></tr>
 *      <tr><td> _USE_STRFUNC </td><td> 0 </td><td> unless you need strings </td></tr>
 *      <tr><td> _USE_MKFS </td><td> 0 </td><td> unless you need to format </td></tr>
 *      <tr><td> _USE_FORWARD </td><td> 0 </td><td> unless you need...this </td></tr>
 *      <tr><td> _CODE_PAGE </td><td> 1 </td><td> for ASCII coding </td></tr>
 *      <tr><td> _USE_LFN </td><td> 0 </td><td> to use short (8.3) filenames </td></tr>
 *    </table> 
 *
 *    Recommended settings for sd_card.h include the following:
 *    <table border="0" cellspacing="1">
 *      <tr><td> SD_BLOCK_SIZE </td><td> 512 </td>
 *                             <td> This must be 512 for all SD cards </td></tr>
 *      <tr><td> SD_LINE_BUF_SIZE </td><td> 256 </td>
 *             <td> 256 Anything from about 16 to 512 should work; this value
 *                  can be adjusted to balance speed with memory usage. </td></tr>
 *    </table> 
 *
 *    The processs of reading or writing a file consists of the following steps: 
 *    \li Mount the card. This is Unix-talk for getting data from the boot record
 *        about where the partitions and base directories on the card can be found.
 *    \li Open a file using a method such as open_file_readonly(), 
 *        open_file_overwrite(), etc. 
 *    \li If lots of data is being logged, one can use the open_new_data_file() method
 *        which automatically creates file names in a sequence. 
 *    \li Use the overloaded shift operator "<<" to write data to the file, or use the
 *        more primitive methods getchar(), putchar(), and puts() to read and write
 *        characters from and to the file. 
 *    \li If at all possible, use the close_file() method to make sure all the data is
 *        saved to the file. If a file is not closed properly, some data may be left 
 *        in the memory buffer and not written to the disk. 
 *
 *  \section sd_tested Functions tested and working
 *  \li Opening an SD card formatted without a partition
 *  \li Automatically creating data files in the root directory and writing to them
 *
 *  \section sd_bugs Bugs
 *    \li When using SD cards formatted with one or more partitions, sometimes the
 *        files written by this driver aren't visible to a computer reading the card.
 *        Workaround: Make sure to use cards which don't have partitions on them. 
 *    \li Writing to the SD card can be very slow, and this code does not attempt to 
 *        prevent blocking of the rest of the system during card writes. 
 *    \li Long file names have not been tested. They're probably not a good idea for
 *        a small embedded system anyway. 
 *
 *  \section sd_int_comp Internal stuff
 *  The DSTATUS flag byte, given in sd_card.c, holds some status bits for the disk:
 *    \li 0x01: STA_NOINIT
 *    \li 0x02: STA_NODISK
 *    \li 0x04: STA_PROTECT
 *
 *  The FRESULT enumeration, given in ff.c, holds the result of a file operation. 
 *  Its values are:
 *    \li 0:  FR_OK
 *    \li 1:  FR_DISK_ERR
 *    \li 2:  FR_INT_ERR
 *    \li 3:  FR_NOT_READY
 *    \li 4:  FR_NO_FILE
 *    \li 5:  FR_NO_PATH
 *    \li 6:  FR_INVALID_NAME
 *    \li 7:  FR_DENIED
 *    \li 8:  FR_EXIST
 *    \li 9:  FR_INVALID_OBJECT
 *    \li 10: FR_WRITE_PROTECTED
 *    \li 11: FR_INVALID_DRIVE
 *    \li 12: FR_NOT_ENABLED
 *    \li 13: FR_NO_FILESYSTEM
 *    \li 14: FR_MKFS_ABORTED
 *    \li 15: FR_TIMEOUT
 *
 */

class sd_card : public base_text_serial
{
	protected:
		/// This buffer holds data which is to be saved into a block on the SD card.
		uint8_t sector_buffer[SD_BLOCK_SIZE];

		/// This buffer holds one line of text to be written to the card.
		queue<char,SD_L_BUF_IDX_T,SD_LINE_BUF_SIZE> line_buffer;

		uint8_t* p_line_buffer;				///< Points to buffer for lines of text
		int index;							///< Index into buffer at which to put data
		unsigned long write_addr;			///< Address to which data is being written

		/// This is a filesystem data structure (see ff.h). 
		FATFS the_fat_fs;

		/// This item holds a result code from opening a directory and/or a file.
		FRESULT dir_file_result;

		/// This is a directory data structure (see ff.h).
		DIR the_dir;

		/// This data structure stores information about a file on the disk.
		FIL the_file;

	public:
		// The constructor sets up the SD card interface
		sd_card (void);

		// This method overrides the ready method in base_text_serial to check the card
		bool ready_to_send (void);

		bool putchar (char);				// Write one character into the data buffer
		void puts (char const*);			// Write a string constant to serial port

		// This method writes a block to the SD card immediately
		void transmit_now (void);

		// This method "mounts" the SD card by reading the partition table
		FRESULT mount (unsigned char);

		// Open a directory so we can read and write files therein
		bool open_directory (const char*);

		// Open a file in read-only mode
		FRESULT open_file_readonly (const char*);

		// Open a file in write mode so that old data will be overwritten
		FRESULT open_file_overwrite (const char*);

		// Open a file in append mode so that data can be written to it
		FRESULT open_file_append (const char*);

		// Close a file by flushing all data from the buffer to the disk
		FRESULT close_file (void);

		// Method to open a data file, automatically making a new name for it
		uint16_t open_new_data_file (/* char*, */ char const*, char const*);
};

#endif // _SD_CARD_H_
