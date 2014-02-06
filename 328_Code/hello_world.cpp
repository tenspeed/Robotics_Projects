//======================================================================================
/** \file program.cpp
 
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

//--------------------------------------------------------------------------------------
/** The main function is the "entry point" of every C program, the one which runs first
 *  (after standard setup code has finished). For mechatronics programs, main() runs an
 *  infinite loop and never exits. You must be really sick of reading that.
 */
void delay_ms(uint8_t ms)
{
	uint16_t delay_count = F_CPU/1000;
	volatile uint16_t i;
		
	while (ms != 0)
	{
		for(i = 0; i != delay_count; i++);
		ms--;
	}
}

int main ()
{
	
	DDRC |= (1<<PIN3); //set PortC Pin3 as output
	PORTC |= (0<<PIN3); //initialize LED off
	int i = 0;
	
	sei();
	rs232 the_serial_port(9600, 6);
	bool blink = true;
	

	while (blink == true)
	{
		
		PORTC &= ~(1<<PIN3); //turn LED off
		delay_ms(10);
		PORTC |= (1<<PIN3); //turn LED on
		
		delay_ms(20);
		//the_serial_port << endl << "Hello World!" << endl;
		
	}

	return(0);
}

