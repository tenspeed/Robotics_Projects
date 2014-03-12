//======================================================================================
/** \file MCU_Master.cpp
 
 *  Revisions:
 *    \li  03-12-14 Created MCU_Master.cpp for testing TWI communications between
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
#include "TWI_Master.h"						// Include header for TWI_Master class

//--------------------------------------------------------------------------------------
/** The main function is the "entry point" of every C program, the one which runs first
 *  (after standard setup code has finished). For mechatronics programs, main() runs an
 *  infinite loop and never exits. You must be really sick of reading that.
 */

// create a blink object so we can blink our LED
int main ()
{
	// create a blink object
	blink blink_obj;
	// create a TWI_Master object
	TWI_Master master_mcu(&blink_obj);
	// initialize the TWI bus on the master mcu
	// if successfully initialized, the LED should blink rapidly 3 times
	master_mcu.TWIInit();
	bool go = true;

	while(1)
	{	
		
	}
	return(0);
}