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

void TWIInit(void)
{
	//set SCL to 500kHz
	TWSR = 0X00;
	TWBR = 0X0C;
	//enable TWI
	TWCR = (1<<TWEN);
}

void TWIStart(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}

void TWIStop(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void TWIWrite(uint8_t data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}

uint8_t TWIReadACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

uint8_t TWIReadNACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

int main ()
{
	
	DDRC |= (1<<PIN3); //set PortC Pin3 as output
	PORTC |= (0<<PIN3); //initialize LED off
	int i = 0;
	
	sei();
	rs232 the_serial_port(9600, 6);
	uint8_t blink = 10;
	

	while (blink != 0)
	{
		
		PORTC &= ~(1<<PIN3); //turn LED off
		delay_ms(10);
		PORTC |= (1<<PIN3); //turn LED on
		delay_ms(20);
		//the_serial_port << endl << "Hello World!" << endl;
		blink--;

		if (blink == 0)	//Take a small breather, then lets keep this party rockin.
		{
			PORTC &= ~(1<<PIN3);
			delay_ms(100);
			blink = 10;
		}
		
	}

	return(0);
}

