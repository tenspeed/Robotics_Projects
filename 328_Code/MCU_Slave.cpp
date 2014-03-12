//======================================================================================
/** \file MCU_Slave.cpp
 
 *  Revisions:
 *    \li  03-12-14 Created MCU_Slave.cpp for testing TWI communications between
 					Master and Slave MCU's
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
#include "blink.h"							// Include header for blink class
#include "TWI_Slave.h"						// Include header for TWI_Slave class

//--------------------------------------------------------------------------------------
/** The main function is the "entry point" of every C program, the one which runs first
 *  (after standard setup code has finished). For mechatronics programs, main() runs an
 *  infinite loop and never exits. You must be really sick of reading that.
 */

// create a blink object so we can blink our LED
int main ()
{

	blink blink_obj;
	TWI_Slave slave_mcu(&blink_obj);
	slave_mcu.TWIInit();
	bool go = true;

	while(1)
	{	
		
	}
	return(0);
}