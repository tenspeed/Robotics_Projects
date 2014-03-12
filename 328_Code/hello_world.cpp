//======================================================================================
/** \file hello_world.cpp
 
 *  Revisions:
 *    \li  04-26-11  Began tearing and hacking at our lab_4 code.
 *	  \li  05-05-11  Finished modifying for lab5 
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

//--------------------------------------------------------------------------------------
/** The main function is the "entry point" of every C program, the one which runs first
 *  (after standard setup code has finished). For mechatronics programs, main() runs an
 *  infinite loop and never exits. You must be really sick of reading that.
 */

// create a blink object so we can blink our LED
int main ()
{

	blink blink_obj;
	bool go = true;

	while(1)
	{	
		if (go)
		{
			blink_obj.blink_LED(5, 300);
			go = false;
		}
		else
		{}
	}
	return(0);
}

