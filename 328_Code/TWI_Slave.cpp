//======================================================================================
/** \file TWI_Slave.cpp
 
 *  Revisions:
 *    \li  03-12-14  Began working on TWI_Slave.cpp
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

											//!< System headers included with < >
#include <stdlib.h>							//!< Standard C library
#include <avr/io.h>							//!< Input-output ports, special registers
#include <avr/interrupt.h>					//!< Interrupt handling functions

											// User written headers included with " "
#include "rs232int.h"						//!< Include header for serial port class
#include "blink.h"							// Include header for the blink class
#include "TWI_Slave.h"						// Include the TWI_Slave class's own header file

//--------------------------------------------------------------------------------------

TWI_Slave::TWI_Slave(blink* blink_object)
{
	blink_obj = blink_object;
	// the constructor for the TWI_Slave class.
}

void TWI_Slave::TWIInit(void)
{
	// hard coded slave address
	// Address: 0x04
	TWAR = (1<<TWA1);
	// confirm that the TWI slave has been initialized
	blink_obj->blink_LED(TWAR, 10);
}