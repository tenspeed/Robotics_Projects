//======================================================================================
/** \file blink.cpp
 
 *  Revisions:
 *    \li  03-12-14  Began working on blink.cpp
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
#include "blink.h"

//--------------------------------------------------------------------------------------
blink::blink(void)
{
	// set PortC Pin3 as output
	DDRC |= (1<<PIN3);\
	// initialize LED off
	PORTC |= (0<<PIN3);
}

void blink::delay_ms(uint8_t ms)
{
	// set a delay counter proportional to the mcu clock speed
	uint16_t delay_count = F_CPU/1000;
	volatile uint16_t i;
		
	while (ms != 0)
	{
		// this loop should eat up 1 ms of time
		// it runs until the ms counter is zero
		for(i = 0; i != delay_count; i++);
		ms--;
	}
}

void blink::blink_LED(uint8_t num_blink, uint8_t delay)
{
	uint8_t blink_count = num_blink;
	uint8_t the_delay = delay;

	// this code is poorly written and will block the mcu from doing anything else
	// but I don't care too much at this point.
	while (blink_count != 0)
	{
		// turn LED on
		PORTC |= (1<<PIN3);
		// wait
		delay_ms(the_delay);
		// turn LED off
		PORTC &= ~(1<<PIN3);
		// wait
		delay_ms(the_delay);
		// decrement the number of blinks remaining
		blink_count--;
	}
}