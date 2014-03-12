//======================================================================================
/** \file TWI_Master.cpp
 
 *  Revisions:
 *    \li  03-12-14  Began working on TWI_Master.cpp
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
#include "TWI_Master.h"

//--------------------------------------------------------------------------------------

TWI_Master::TWI_Master()
{
	// the constructor for the TWI_Master class.
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