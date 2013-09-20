//*************************************************************************************
/** \file sd_card.cpp
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

#include <string.h>							// Header for handling file name strings
#include "sd_card.h"						// Pull in the base class header file


/** This typedef defines a status byte which holds information about the state of a
 *  drive. If it's 0, the drive is working and ready for use; when one or more bits 
 *  are 1, different exception conditions defined as STA_xxx are indicated.
 */
typedef BYTE DSTATUS;

#define STA_NOINIT			0x01	///< Drive not initialized code in DSTATUS
#define STA_NODISK			0x02	///< No medium in the drive code in DSTATUS
#define STA_PROTECT			0x04	///< Write protected code in DSTATUS

#define CTRL_SYNC			0		///< Sync command for write fns. in disk_ioctrl()
#define GET_SECTOR_COUNT	1		///< Command used in f_mkfs() in disk_ioctrl()
#define GET_SECTOR_SIZE		2		///< Command code in disk_ioctrl()
#define GET_BLOCK_SIZE		3		///< Mandatory for only f_mkfs() in disk_ioctrl()
#define CTRL_POWER			4		///< Command code in disk_ioctrl()
#define CTRL_LOCK			5		///< Command code in disk_ioctrl()
#define CTRL_EJECT			6		///< Command code in disk_ioctrl()
#define MMC_GET_TYPE		10		///< Command code in disk_ioctrl()
#define MMC_GET_CSD			11		///< Command code in disk_ioctrl()
#define MMC_GET_CID			12		///< Command code in disk_ioctrl()
#define MMC_GET_OCR			13		///< Command code in disk_ioctrl()
#define MMC_GET_SDSTAT		14		///< Command code in disk_ioctrl()

#define CMD0		(0x40+0)		///< GO_IDLE_STATE command for SD/MMC card
#define CMD1		(0x40+1)		///< SEND_OP_COND (MMC) command for SD/MMC card
#define	ACMD41		(0xC0+41)		///< SEND_OP_COND (SDC) command for SD/MMC card
#define CMD8		(0x40+8)		///< SEND_IF_COND command for SD/MMC card
#define CMD9		(0x40+9)		///< SEND_CSD command for SD/MMC card
#define CMD10		(0x40+10)		///< SEND_CID command for SD/MMC card
#define CMD12		(0x40+12)		///< STOP_TRANSMISSION command for SD/MMC card
#define ACMD13		(0xC0+13)		///< SD_STATUS (SDC) command for SD/MMC card
#define CMD16		(0x40+16)		///< SET_BLOCKLEN command for SD/MMC card
#define CMD17		(0x40+17)		///< READ_SINGLE_BLOCK command for SD/MMC card
#define CMD18		(0x40+18)		///< READ_MULTIPLE_BLOCK command for SD/MMC card
#define CMD23		(0x40+23)		///< SET_BLOCK_COUNT (MMC) command for SD/MMC card
#define	ACMD23		(0xC0+23)		///< SET_WR_BLK_ERASE_COUNT (SDC) command for card
#define CMD24		(0x40+24)		///< WRITE_BLOCK command for SD/MMC card
#define CMD25		(0x40+25)		///< WRITE_MULTIPLE_BLOCK command for SD/MMC card
#define CMD55		(0x40+55)		///< APP_CMD command for SD/MMC card
#define CMD58		(0x40+58)		///< READ_OCR command for SD/MMC card

#define CT_MMC			0x01				///< MMC ver 3 card type flag
#define CT_SD1			0x02				///< SD ver 1 card type flag
#define CT_SD2			0x04				///< SD ver 2 card type flag
#define CT_SDC			(CT_SD1|CT_SD2)		///< SD card type flag
#define CT_BLOCK		0x08				///< Block addressing card type flag


/// This macro causes the SD card to be selected by lowering its chip select bit.
#define SELECT()			SD_CS_PORT &= ~SD_CS_MASK

/// This macro causes the SD card to be deselected by raising its chip select bit.
#define DESELECT()			SD_CS_PORT |= SD_CS_MASK

/** This macro is specified to turn SD card power on, but it does nothing because the
 *  SD card in the current design has no socket power control.
 */
#define MM_power_on()

/** This macro is specified to turn SD card power off, but it does nothing because the
 *  SD card in the current design has no socket power control.
 */
#define MM_power_off()

/** This macro is defined to set a slow SPI clock speed, but it does nothing because
 *  it's not actually needed. 
 */
#define	FCLK_SLOW()

/** This macro is defined to set a fast SPI clock speed, but it does nothing because
 *  it's not actually needed. 
 */
#define	FCLK_FAST()


/** This variable stores the status of the card, indicating if there's a card which
 *  has been initialized in the slot or not.
 */
static volatile DSTATUS Stat = STA_NOINIT;


/** This byte stores flags which indicate the card type.
 */
static uint8_t CardType;


//-------------------------------------------------------------------------------------
/** This macro transmits a byte to the SD card via the SPI interface. 
 *  @param dat The data byte to be transmitted
 */

#define MM_xmit_spi(dat) 	SPDR = (dat); loop_until_bit_is_set (SPSR, SPIF)


//-------------------------------------------------------------------------------------
/** This function receives a byte from the SD card via the SPI interface. 
 *  @return The byte which was received
 */

extern "C"	// This function is compiled as C, not C++
{
	static BYTE MM_rcvr_spi (void)
	{
		SPDR = 0xFF;
		loop_until_bit_is_set (SPSR, SPIF);
		return (SPDR);
	}
} // Extern "C"


//-------------------------------------------------------------------------------------
/** This macro receives a byte through the SPI interface. It is intended to be a bit
 *  faster than calling the MM_rcvr_spi() function.
 *  @param dst A pointer to a byte in which to store the received data
 */

#define MM_rcvr_spi_m(dst)	SPDR = 0xFF; \
							loop_until_bit_is_set (SPSR, SPIF); \
							*(dst) = SPDR


//-------------------------------------------------------------------------------------
/** This function waits for the SD card to become ready. It tries to read a byte from
 *  the SPI port; if it gets 0xFF, that means what we're waiting for has happened. 
 */

extern "C"	// This function is compiled as C, not C++
{
	static BYTE MM_wait_ready (void)
	{
		BYTE res;
		uint32_t delay = 0L;

		MM_rcvr_spi ();
		do
			res = MM_rcvr_spi ();
		while ((res != 0xFF) && (++delay < 1000000L));

		return (res);
	}
} // Extern "C"


//-------------------------------------------------------------------------------------
/** This function deselects the SD card and releases the SPI bus. 
 */

extern "C"	// This function is compiled as C, not C++
{
	static void MM_release_spi (void)
	{
		DESELECT ();
		MM_rcvr_spi ();
	}
} // Extern "C"


//-------------------------------------------------------------------------------------
/** This function receives a data packet from the SD card. 
 *  @param buff Pointer to a data buffer in which to store received data
 *  @param btr The number of bytes to receive (must be multiple of 4)
 *  @return True if data was successfully received, false if there were problems
 */

extern "C"	// This function is compiled as C, not C++
{
	static bool MM_rcvr_datablock (BYTE *buff, UINT btr)
	{
		BYTE token;
		uint32_t delay = 0L;

		do								// Wait for data packet in timeout of 200ms
		{
			token = MM_rcvr_spi ();
		} 
		while ((token == 0xFF) && (++delay < 100000L));

		if (token != 0xFE) return (false);		// If invalid data token, return error

		do										// Receive the data block into buffer
		{
			MM_rcvr_spi_m (buff++);
			MM_rcvr_spi_m (buff++);
			MM_rcvr_spi_m (buff++);
			MM_rcvr_spi_m (buff++);
		}
		while (btr -= 4);

		MM_rcvr_spi();							// Discard CRC
		MM_rcvr_spi();

		return (true);							// Return with success
	}
} // Extern "C"


#if _READONLY == 0
extern "C"	// This function is compiled as C, not C++
{
	//---------------------------------------------------------------------------------
	/** This function sends a data packet to the SD card. 
	*  @param buff Pointer to a data buffer in which to store received data
	*  @param token Data/Stop token
	*  @return True if data was successfully transmitted, false if there were problems
	*/

	static bool MM_xmit_datablock (const BYTE *buff, BYTE token)
	{
		BYTE resp, wc = 0;

		if (MM_wait_ready () != 0xFF) return (false);

		MM_xmit_spi (token);					/* Xmit data token */
		if (token != 0xFD)						/* Is data token */
		{
			do									/* Xmit the 512 byte data block to MMC */
			{
				MM_xmit_spi (*buff++);
				MM_xmit_spi (*buff++);
			}
			while (--wc);

			MM_xmit_spi (0xFF);					/* CRC (Dummy) */
			MM_xmit_spi (0xFF);
			resp = MM_rcvr_spi ();				/* Reveive data response */
			if ((resp & 0x1F) != 0x05)			/* If not accepted, return with error */
				return (false);
		}

		return (true);
	}
} // Extern "C"
#endif /* _READONLY */


//-------------------------------------------------------------------------------------
/** This function sends a command packet to the SD card. 
 *  @param cmd The command byte to be sent
 *  @param arg The argument to be sent along with the command
 */

extern "C"	// This function is compiled as C, not C++
{
	static BYTE MM_send_cmd (BYTE cmd, DWORD arg)
	{
		BYTE n, res;

		if (cmd & 0x80)			/* ACMD<n> is the command sequence of CMD55-CMD<n> */
		{
			cmd &= 0x7F;
			res = MM_send_cmd (CMD55, 0);
			if (res > 1) return res;
		}

		/* Select the card and wait for ready */
		DESELECT ();
		SELECT ();
		n = MM_wait_ready ();
		if (n != 0xFF)
		{
// 			GLOB_DEBUG (PMS ("MM_send_cmd: timeout, got ") << n << endl);
			return 0xFF;
		}

		/* Send command packet */
		MM_xmit_spi (cmd);						/* Start + Command index */
		MM_xmit_spi ((BYTE)(arg >> 24));		/* Argument[31..24] */
		MM_xmit_spi ((BYTE)(arg >> 16));		/* Argument[23..16] */
		MM_xmit_spi ((BYTE)(arg >> 8));			/* Argument[15..8] */
		MM_xmit_spi ((BYTE)arg);				/* Argument[7..0] */
		n = 0x01;								/* Dummy CRC + Stop */
		if (cmd == CMD0) n = 0x95;				/* Valid CRC for CMD0(0) */
		if (cmd == CMD8) n = 0x87;				/* Valid CRC for CMD8(0x1AA) */
		MM_xmit_spi(n);

		/* Receive command response */
		if (cmd == CMD12) MM_rcvr_spi ();		/* Skip a stuff byte when stop reading */
		n = 10;									/* Wait for a valid response in timeout of 10 attempts */
		do
			res = MM_rcvr_spi();
		while ((res & 0x80) && --n);

		return res;			/* Return with the response value */
	}
} // Extern "C"


//-------------------------------------------------------------------------------------
/** This function initializes the SD card.
 *  @param drive_number The drive number; this function works with one SD card only, 
 *                      so the drive number is ignored
 */

extern "C"	// This function is compiled as C, not C++
{
	DSTATUS disk_initialize (BYTE drive_number)
	{
		BYTE n, ty, cmd, ocr[4];

		MM_power_on ();								// Force socket power ON
		FCLK_SLOW ();								// Slow the clock down

		DESELECT ();								// Make sure card is not selected
		for (n = 10; n; n--)						// 80 dummy clocks
			MM_rcvr_spi ();

		ty = 0;
		n = MM_send_cmd (CMD0, 0);
		if (n == 1)				// Enter Idle state
		{
			int32_t retry_cnt = 10000L;
			if (MM_send_cmd (CMD8, 0x1AA) == 1) 	// SDC ver 2.00
			{
				for (n = 0; n < 4; n++) 
					ocr[n] = MM_rcvr_spi ();
				if (ocr[2] == 0x01 && ocr[3] == 0xAA)	// Card can work at vdd in 2.7-3.6V
				{
					// ACMD41 with HCS bit
					while ((--retry_cnt > 0) && MM_send_cmd (ACMD41, 1UL << 30));
					if ((--retry_cnt > 0) && MM_send_cmd (CMD58, 0) == 0)
					{								/* Check CCS bit */
						for (n = 0; n < 4; n++) 
							ocr[n] = MM_rcvr_spi ();
						ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* SDv2 */
					}
				}
			}
			else
			{										/* SDv1 or MMCv3 */
				if (MM_send_cmd (ACMD41, 0) <= 1)
				{
					ty = CT_SD1; cmd = ACMD41;		/* SDv1 */
				}
				else
				{
					ty = CT_MMC; cmd = CMD1;		/* MMCv3 */
				}
				// Wait for leaving idle state
				while ((--retry_cnt > 0) && MM_send_cmd (cmd, 0));

				// Select R/W block length
				if ((retry_cnt <= 0) || MM_send_cmd (CMD16, 512) != 0)
					ty = 0;
			}
		}
// 		else
// 		{
// 			GLOB_DEBUG (PMS ("disk_initialize(): got ") << hex << n << dec 
// 				<< PMS (", not 1") << endl);
// 		}

		CardType = ty;
		MM_release_spi ();

		if (ty)									/* Initialization succeded */
		{
			Stat &= ~STA_NOINIT;				/* Clear STA_NOINIT */
			FCLK_FAST ();
		}
		else
		{										/* Initialization failed */
			MM_power_off ();
		}

		return (Stat);
	}
} // extern "C"


//-------------------------------------------------------------------------------------
/** This function returns the status of a given physical drive. The status is held in
 *  a byte of type DSTATUS which holds bits that indicate drive not initialized and
 *  other exception conditions. 
 *  @param drv The physical drive number. Since this implementation uses only one 
 *             drive, this parameter is ignored. 
 */

extern "C"	// This function is compiled as C, not C++
{
	DSTATUS disk_status (BYTE drv)
	{
		return (Stat);
	}
} // extern "C"


//-------------------------------------------------------------------------------------
/** This function reads one or more sector(s) from the SD card. 
 *  @param drive_number The drive number; this function works with one SD card only, 
 *                      so the drive number is ignored
 *  @param buff A pointer to the data buffer to store read data
 *  @param sector Starting sector number (LBA)
 *  @param count The sector count (1..255)
 */

extern "C"	// This function is compiled as C, not C++
{
	DRESULT disk_read (BYTE drive_number, BYTE *buff, DWORD sector, BYTE count)
	{
		if (!count) return RES_PARERR;
		if (Stat & STA_NOINIT) return RES_NOTRDY;

		if (!(CardType & CT_BLOCK)) sector *= 512;	/* Convert LBA to byte address if needed */

		if (count == 1) {	/* Single block read */
			if ((MM_send_cmd(CMD17, sector) == 0)	/* READ_SINGLE_BLOCK */
				&& MM_rcvr_datablock(buff, 512))
				count = 0;
		}
		else {				/* Multiple block read */
			if (MM_send_cmd(CMD18, sector) == 0) {	/* READ_MULTIPLE_BLOCK */
				do {
					if (!MM_rcvr_datablock(buff, 512)) break;
					buff += 512;
				} while (--count);
				MM_send_cmd(CMD12, 0);				/* STOP_TRANSMISSION */
			}
		}
		MM_release_spi();

		return count ? RES_ERROR : RES_OK;
	}
} // extern "C"


//-------------------------------------------------------------------------------------
/** This function writes one or more sector(s) to the SD card. 
 *  @param drive_number The drive number; this function works with one SD card only, 
 *                      so the drive number is ignored
 *  @param buff A pointer to the buffer holding the data to be written
 *  @param sector Starting sector number (LBA)
 *  @param count The sector count (1..255)
 */

extern "C"	// This function is compiled as C, not C++
{
	DRESULT disk_write (BYTE drive_number, const BYTE *buff, DWORD sector, 
								BYTE count)
	{
		if (!count) return RES_PARERR;
		if (Stat & STA_NOINIT) return RES_NOTRDY;
		if (Stat & STA_PROTECT) return RES_WRPRT;

		if (!(CardType & CT_BLOCK)) sector *= 512;	/* Convert LBA to byte address if needed */

		if (count == 1)								/* Single block write */
		{
			if ((MM_send_cmd(CMD24, sector) == 0)	/* WRITE_BLOCK */
				&& MM_xmit_datablock(buff, 0xFE))
				count = 0;
		}
		else									/* Multiple block write */
		{
			if (CardType & CT_SDC) {
				MM_send_cmd (CMD55, 0); MM_send_cmd (CMD23, count);	/* ACMD23 */
			}
			if (MM_send_cmd (CMD25, sector) == 0)	/* WRITE_MULTIPLE_BLOCK */
			{
				do 
				{
					if (!MM_xmit_datablock (buff, 0xFC)) break;
					buff += 512;
				}
				while (--count);
				if (!MM_xmit_datablock (0, 0xFD))	/* STOP_TRAN token */
					count = 1;
			}
		}
		MM_release_spi ();

		return count ? RES_ERROR : RES_OK;
	}
} // extern "C"


//-------------------------------------------------------------------------------------
/** This function is used to carry out various miscellaneous I/O functions with the
 *  SD card. The functions which are supported are the following: 
 *  \li CTRL_SYNC - Ensure that any pending write process has finished
 *  \li GET_SECTOR_COUNT - Get the number of sectors on the disk
 *  \li GET_SECTOR_SIZE - Get the sector size
 *  \li GET_BLOCK_SIZE - Get the block size (should always be 512 for SD cards)
 *  \li MMC_GET_CSD - 
 *  \li MMC_GET_CID - 
 *  \li MMC_GET_OCR - 
 *  \li MMC_GET_SDSTAT - 
 *
 *  @param drive_number The drive number; this function works with one SD card only, 
 *                      so the drive number is ignored
 *  @param ctrl The control code which tells which function is called for
 *  @param buff A pointer to a buffer used for sector I/O to and from the disk
 */

extern "C"	// This function is compiled as C, not C++
{
	DRESULT disk_ioctl (BYTE drive_number, BYTE ctrl, void *buff)
	{
		DRESULT res;
		BYTE n, csd[16], *ptr = (uint8_t*)buff;
		WORD csize;

		if (Stat & STA_NOINIT) return RES_NOTRDY;

		res = RES_ERROR;
		switch (ctrl) 
		{
			/* Make sure that pending write process has been finished */
			case CTRL_SYNC :
				SELECT ();
				if (MM_wait_ready() == 0xFF)
					res = RES_OK;
				break;

			/* Get number of sectors on the disk (DWORD) */
			case GET_SECTOR_COUNT :
				if ((MM_send_cmd (CMD9, 0) == 0) && MM_rcvr_datablock (csd, 16))
				{
					if ((csd[0] >> 6) == 1)							/* SDC ver 2.00 */
					{
						csize = csd[9] + ((WORD)csd[8] << 8) + 1;
						*(DWORD*)buff = (DWORD)csize << 10;
					}
					else									/* MMC or SDC ver 1.XX */
					{
						n = (csd[5] & 15) + ((csd[10] & 128) >> 7) 
							+ ((csd[9] & 3) << 1) + 2;
						csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) 
							+ ((WORD)(csd[6] & 3) << 10) + 1;
						*(DWORD*)buff = (DWORD)csize << (n - 9);
					}
					res = RES_OK;
				}
				break;

			/* Get sectors on the disk (WORD) */
			case GET_SECTOR_SIZE :
				*(WORD*)buff = 512;
				res = RES_OK;
				break;
 
			/* Get erase block size in unit of sectors (DWORD) */
			case GET_BLOCK_SIZE :
				if (CardType & CT_SD2)						/* SDC ver 2.00 */
				{
					if (MM_send_cmd(ACMD13, 0) == 0)
					{										/* Read SD status */
						MM_rcvr_spi ();
						if (MM_rcvr_datablock (csd, 16))	/* Read partial block */
						{
							/* Purge trailing data */
							for (n = 64 - 16; n; n--) MM_rcvr_spi ();
							*(DWORD*)buff = 16UL << (csd[10] >> 4);
							res = RES_OK;
						}
					}
				}
				else								/* SDC ver 1.XX or MMC */
				{
					if ((MM_send_cmd (CMD9, 0) == 0) && MM_rcvr_datablock(csd, 16))
					{
						if (CardType & CT_SD1)		/* SDC ver 1.XX */
						{
							*(DWORD*)buff = (((csd[10] & 63) << 1) 
								+ ((WORD)(csd[11] & 128) >> 7) 
								+ 1) << ((csd[13] >> 6) - 1);
						}
						else
						{					/* MMC */
							*(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) 
								* (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
						}
						res = RES_OK;
					}
				}
				break;

			/* Receive CSD as a data block (16 bytes) */
			case MMC_GET_CSD :
				if (MM_send_cmd (CMD9, 0) == 0 && MM_rcvr_datablock (ptr, 16))
					res = RES_OK;
				break;

			/* Receive CID as a data block (16 bytes) */
			case MMC_GET_CID :		
				if (MM_send_cmd (CMD10, 0) == 0
					&& MM_rcvr_datablock(ptr, 16))
					res = RES_OK;
				break;

			/* Receive OCR as an R3 resp (4 bytes) */
			case MMC_GET_OCR :
				if (MM_send_cmd (CMD58, 0) == 0)
				{
					for (n = 0; n < 4; n++)
						*ptr++ = MM_rcvr_spi();
					res = RES_OK;
				}
				break;

			/* Receive SD statsu as a data block (64 bytes) */
			case MMC_GET_SDSTAT :
				if (MM_send_cmd (ACMD13, 0) == 0)
				{
					MM_rcvr_spi ();
					if (MM_rcvr_datablock (ptr, 64))
						res = RES_OK;
				}
				break;

			default:
				res = RES_PARERR;
		}

		MM_release_spi();

		return res;
	}
} // extern "C"


//-------------------------------------------------------------------------------------
/** This is a real time clock service to be called from the FatFs module. Any valid 
 *  time must be returned even if the system does not support a real time clock. This 
 *  is not required in read-only configuration.
 */

extern "C"	// This function is compiled as C, not C++
{
	DWORD get_fattime (void)
	{
		/* Pack date and time into a DWORD variable */
		return ((DWORD)(2009 - 1980) << 25)			// Year
				| ((DWORD)(11) << 21)				// Month
				| ((DWORD)(26) << 16)				// Day
				| ((DWORD)(8) << 11)				// Hour
				| ((DWORD)(0) << 5)					// Minutes
				| ((DWORD)(0) >> 1);				// Seconds
	}
} // extern "C"


//-------------------------------------------------------------------------------------
/** This constructor sets up an SD card interface. It calls the constructor of its 
 *  base class base_text_serial and initializes the data buffer so that the buffer is 
 *  ready to accept some data. It also configures the SPI port pins so that they are
 *  ready to communicate with the card. The constructor does not initialize the card;
 *  initialization is performed in the mount() method. 
 */

sd_card::sd_card (void) : base_text_serial ()
{
	// Enable the correct port pins for the SPI port. The pins may vary by chip, so
	// the enables are defined on a chip by chip basis in sd_card.h 
	MM_SETUP_PORT ();

	// Set the slave select pin high. This is not necessarily the same pin as the one
	// SS pin provided with the AVR's SPI interface; any pin on any port can be used 
	// as the slave select (i.e. card enable or card select) pin
	SD_CS_PORT |= SD_CS_MASK;				// Set card select pin high
	SD_CS_DDR |= SD_CS_MASK;				// Set card select pin as an output

	#ifdef PRR
		PRR &= ~(1 << PRSPI);				// Make sure SPI power isn't cut off
	#endif
	SPCR = (1 << MSTR) | (1 << SPE);		// Enable SPI interface as Master
	SPCR |= (1 << SPR0);					// Set clock f_osc/16, 8 in double-speed
	SPSR = 0x01;							// Set SPI interface to double speed 

	// Get a pointer to the buffer for lines of text
	p_line_buffer = (uint8_t*)(line_buffer.get_p_buffer ());

	// Since no card has been mounted and no file opened, set error flags to prevent
	// any attempts at writing until a file has been opened
	Stat = STA_NOINIT;
	dir_file_result = FR_NOT_READY;
}


//-------------------------------------------------------------------------------------
/** This method checks if the card is ready, with a file opened, for some data to be 
 *  sent to it. If a file is opened, the status flags will have been set properly. 
 *  @return True if there's a card ready for data and false if not
 */

bool sd_card::ready_to_send (void)
{
	return ((Stat == 0) && (dir_file_result == 0));
}


//-------------------------------------------------------------------------------------
/** This method puts one character into the buffer of data to be written to the card. 
 *  If the buffer becomes full through this write, the buffer is automatically written
 *  to the card. This will take a long time, so it might be a good idea to control the
 *  times at which buffers full of data are written using the transmit_now() method to
 *  write data and chars_empty() to check how much space is left in the buffer. An 
 *  error occurs if the buffer is full and there's no card to which to store data.                          
 *  @param to_put The character to be written into the buffer
 *  @return True if everything was OK and false if data couldn't be stored
 */

bool sd_card::putchar (char to_put)
{
 	if (!ready_to_send ()) return (false);

	line_buffer.put (to_put);

	// If the buffer has become full or we have a return character, dump the buffer's
	// contents to the SD card right now
	#ifdef SD_SEND_NOW_CHAR
		if ((line_buffer.num_items () >= (SD_LINE_BUF_SIZE - 1))
			|| (to_put == SD_SEND_NOW_CHAR))
			transmit_now ();
	#else
		if (line_buffer.num_items () >= (SD_LINE_BUF_SIZE - 1))
			transmit_now ();
	#endif
	
	return (true);
}


//-------------------------------------------------------------------------------------
/** This method writes all the characters in a string to the card buffer until it gets 
 *  to the '\\0' at the end. Warning: This function blocks until it's finished. 
 *  @param str The string to be written 
 */

void sd_card::puts (char const* str)
{
	while (*str)
		putchar (*str++);
}


//-------------------------------------------------------------------------------------
/** This method writes the data from the buffer to the card. Because there's so much 
 *  data and flash cards are slow, it takes a comparatively long time to write data. 
 */

void sd_card::transmit_now (void)
{
	// If there's not a mounted card present, we can't transmit anything
	if (Stat || dir_file_result)
		return;

	uint16_t bytes_written = 0;				// Number of bytes acutally written

	// Write the data which is in the output buffer to the file
	f_write (&the_file, p_line_buffer, line_buffer.num_items (), &bytes_written);

	// The buffer's all empty, so prepare it for some all-new data
	line_buffer.flush ();
}


//-------------------------------------------------------------------------------------
/** This method performs an action equivalent to "mounting" the SD card (in POSIX 
 *  talk). This means that the card interface is initialized, then the card's boot 
 *  record and partition table and all that oddball stuff is scanned so that we will
 *  be able to find files on the card. Partition number 0 is always mounted. 
 *  @param part_num The number of the partition to mount -- ignored, we use 0 always
 *  @return The result code from attempting to open the root directory; 0 means OK
 */

FRESULT sd_card::mount (unsigned char part_num)
{
	// Call the disk initialization function
	disk_initialize (0);

	// This causes ELM-FAT-FS to use the given file system object for storing data
	// about the current file system; no disk access occurs yet. Because the only
	// possible error is "invalid drive number" and we don't use multiple drives, 
	// the return value is ignored. 
	f_mount (0, &the_fat_fs);

	// Open the root directory. This is done just to make sure it can be done
	dir_file_result = f_opendir (&the_dir, "/");

	return (dir_file_result);
}


//-------------------------------------------------------------------------------------
/** This method opens the given directory. The directory is identified by a path name
 *  which may go into several layers of subdirectories but must be less than 64
 *  characters long. A valid path name might be "/data/strain.dat/recent"; note that
 *  forward slashes are used (POSIX style), not backslashes in the Windows style, and
 *  that 8.3 names are required. 
 *  @param path A character string containing the full path name of the directory
 *  @return True if the directory was opened OK, or false if it couldn't be opened
 */

bool sd_card::open_directory (const char* path)
{
	return (false);
}


//-------------------------------------------------------------------------------------
/** This method opens the given file in read-only mode. The file must exist, and the
 *  volume in which the file exists must have already been mounted. 
 *  @param path_name A path to the file to be opened
 *  @return An FRESULT value (see ff.h) which tells if the file was opened successfully
 */

FRESULT sd_card::open_file_readonly (const char* path_name)
{
	// Check the status of mounting the card
	if (Stat)
	{
		GLOB_DEBUG (PMS ("Can't open: Not initialized") << endl);
		return (FR_NOT_READY);
	}

	// Call the file opening function to do the real work
	dir_file_result = f_open (&the_file, path_name, (FA_READ | FA_OPEN_EXISTING));

	// Check the result code
	if (dir_file_result)
	{
		GLOB_DEBUG (PMS ("Can't open file ") << path_name << PMS (", code ") 
			<< dir_file_result << endl);
	}

	// If we get here, the file was opened successfully
	return (dir_file_result);
}


//-------------------------------------------------------------------------------------
/** This method opens the given file for writing (the file can also be read).  It 
 *  leaves the read/write pointer at the beginning of the file so that old data will be
 *  overwritten.
 *  @param path_name The path name to the file, including the directory
 *  @return The result code from the f_open() call attempting to open the file
 */

FRESULT sd_card::open_file_overwrite (const char* path_name)
{
	// Check the status of mounting the card
	if (Stat)
	{
		GLOB_DEBUG (PMS ("Can't open: Not initialized") << endl);
		return (FR_NOT_READY);
	}

	// Call the file opening function to do the real work
	dir_file_result = f_open (&the_file, path_name, (FA_WRITE | FA_CREATE_ALWAYS));

	// Check the result code
	if (dir_file_result)
	{
		GLOB_DEBUG (PMS ("Can't open file ") << path_name << PMS (", code ") 
			<< dir_file_result << endl);
	}

	// If we get here, the file was opened successfully
	return (dir_file_result);
}


//-------------------------------------------------------------------------------------
/** This method opens the given file for writing (the file can also be read). If the 
 *  file doesn't yet exist, it is created; if it exists, it's opened in append mode.
 *
 *  WARNING, BUG, TODO: This method hasn't been finished yet and doesn't work. 
 * 
 *  @param path_name The path name to the file, including the directory
 *  @return The result code from the f_open() call attempting to open the file
 */

FRESULT sd_card::open_file_append (const char* path_name)
{
	//////////////////////// NOT WORKING YET //////////////////////////
	// Check the status of mounting the card
	if (Stat)
	{
		GLOB_DEBUG (PMS ("Can't open: Not initialized") << endl);
		return (FR_NOT_READY);
	}

	// Call the file opening function to do the real work
	dir_file_result = f_open (&the_file, path_name, (FA_WRITE | FA_OPEN_ALWAYS));

	// Check the result code
	if (dir_file_result)
	{
		GLOB_DEBUG (PMS ("Can't open file ") << path_name << PMS (", code ") 
			<< dir_file_result << endl);
	}

	// If we get here, the file was opened successfully
	return (dir_file_result);
	////////////////////////////////////////////////////////////////////
}


//-------------------------------------------------------------------------------------
/** This method closes a file. If there is any data left in the buffer, it is written
 *  to the card. Then further writing to this file is prevented unless the file is
 *  opened again.
 *  @return The result code from calling f_close() to close the file
 */

FRESULT sd_card::close_file (void)
{
	// If there isn't an open file, we can't really close a file
	if (dir_file_result != FR_OK)
	{
		GLOB_DEBUG (PMS ("Cannot close file; none open") << endl);
		return (FR_INT_ERR);
	}

	// Any data left in the buffer is written now
	if (line_buffer.num_items () > 0)
	{
		GLOB_DEBUG (PMS ("close_file(): ") << line_buffer.num_items () 
			<< PMS (" bytes") << endl);
		transmit_now ();
	}

	// No more file access is permitted until the file is reopened
	return (f_close (&the_file));
}


//--------------------------------------------------------------------------------------
/** This function opens a new data file, in the card's root directory, with a name 
 *  which hasn't yet been used. Names are made by putting together a path, base name, 
 *  number, and extension as in "/data/file_012.txt". The base name variable includes
 *  the path and the file name together, separated by forward slashes. 
 *  @param base_name The base name, including path, such as "/data/log_" where the
 *      file name should be at most 5 characters long because 3 digits will be added.
 *      The base name should begin with a slash to be an absolute path name on a card. 
 *  @param extension The 3-character file name extension, such as "txt" or "csv"
 *  @return The number used to make the new data file's name, or 0xFFFF for failure
 */

uint16_t sd_card::open_new_data_file (char const* base_name, char const* extension)
{
	// Check the status of mounting the card
	if (Stat)
	{
		GLOB_DEBUG (PMS ("Can't create: Not initialized") << endl);
		return (0xFFFF);
	}

	// Allocate a buffer for the file name
	uint8_t path_size = strlen (base_name);
	char buffer[path_size + 9];

	// Try lots of file names until an unused one is found
	for (uint16_t number = 0; number < 1000; number++)
	{
		// Make an extra pointer to the buffer
		char* pbuf = buffer;

		// First put the path and file name into the buffer
		strncpy (pbuf, base_name, path_size);
		pbuf += path_size;

		// Now add the digits of the number
		uint16_t temp_num = number;
		*pbuf++ = (temp_num / 100) + '0';
		temp_num %= 100;
		*pbuf++ = (temp_num / 10) + '0';
		temp_num %= 10;
		*pbuf++ = temp_num + '0';

		// Add a dot and the extension and a '\0' to terminate the string
		*pbuf++ = '.';
		strncpy (pbuf, extension, 3);
		pbuf += 3;
		*pbuf = '\0';

		// Now try to open a file with the given name for reading; if opening succeeds
		// then the file exists and we need to loop around and try another name
		dir_file_result = f_open (&the_file, buffer, (FA_READ | FA_OPEN_EXISTING));

		switch (dir_file_result)
		{
			// If the error is that the file doesn't exist, good! Use that file name
			case FR_NO_FILE:
				dir_file_result = open_file_overwrite (buffer);
				return (number);

			// If there's no error, the file exists, so don't use that name. Loop
			// again and try another name
			case FR_OK:
				break;

			// Any other error means trouble, so exit this function
			default:
				GLOB_DEBUG (PMS ("Error ") << dir_file_result 
					<< PMS (" opening data file") << endl);
				return (0xFFFF);
		}
	}

	// If we get here, no file name was usable (there were 1000 files?!?!)
	return (0xFFFF);
}
