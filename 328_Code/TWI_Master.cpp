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
#include "blink.h"							// Include header for the blink class
#include "TWI_Master.h"						// Include the TWI_Master class's own header file

//--------------------------------------------------------------------------------------

TWI_Master::TWI_Master(blink* blink_object)
{
	blink_obj = blink_object;
	// the constructor for the TWI_Master class.
}

void TWI_Master::TWIInit(void)
{
	//set SCL to 500kHz
	TWSR = 0X00;
	TWBR = 0X0C;
	//enable TWI
	TWCR = (1<<TWEN);
	// confirm that the TWI bus has been initialized
	blink_obj->blink_LED(3, 10);
}

void TWI_Master::TWIStart(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}

void TWI_Master::TWIStop(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void TWI_Master::TWIWrite(uint8_t data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}

uint8_t TWI_Master::TWIReadACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

uint8_t TWI_Master::TWIReadNACK(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}