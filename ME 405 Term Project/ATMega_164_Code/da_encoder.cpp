//======================================================================================
/** \file  da_encoder.cpp
 *    da_encoder.cpp is an object class that contains a constructor which initializes interrupts 
 *	  and configures necessary i/o ports. It also is passed a serial pointer which allows encoder 
 *	  objects to print to the terminal window. All other operations handled by da_encoder are 
 *	  contained in the ISR. The constructor is passed pointers to two variables in main which
 *	  keep count of encoder ticks, where those values are inc/decremented here in the ISR.
 *
 *  Revisions:
 *            \li  04-18-11  Began tearing and hacking at our lab_3 code.
 *	          \li  04-25-11  Final iteration. (for now)
 *			  \li  05-05-11  Gutted version of encoder to be paired with slave_driver    
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

#include <stdlib.h>							//!< Include standard library header files
#include <avr/io.h>							//!< You'll need this for SFR and bit names
#include "rs232int.h"						//!< Include header for serial port class
#include "da_encoder.h"						//!< Include header for the A/D class
/// total # of ticks seen in a revolution by encoder 1
#define QUAD1  4000		
/// total # of ticks seen in a revolution by encoder 2
#define QUAD2  4000	

/** This object utilizes 5 file scope variables for each encoder. This allows motor movement to 
*   be determined using interrupts. Also, errors and revolutions are counted.
*/
uint8_t new_1;								//!< Stores channel A & B status for motor 1
uint8_t old_1 = 0;							//!< Archives channel A & B status for comparison
uint8_t new_2;								//!< Stores channel A & B status for motor 2
uint8_t old_2 = 0;							//!< Archives channel A & B status for comparison
uint16_t* ptr_to_error_1 = 0;				//!< Counts number of errors encountered on encoder 1
uint16_t* ptr_to_error_2 = 0;				//!< Counts number of errors encountered on encoder 2
int32_t* ptr_encoder_1;						//!< counts encoder ticks on motor 1
int32_t* ptr_encoder_2;						//!< counts encoder ticks on motor 2

//-------------------------------------------------------------------------------------
/** This constructor sets up an encoder driver. Pin Change Interrupts are enabled on ports A & C
*	and utilize PCINT0 and 1 for motor 1 and PCINT16 & 17 for motor 2. These mentioned ports are also 
*	verified to be outputs in the DDR and pull-up resitors are enabled.
*   @param p_serial_port A pointer to the serial port which writes debugging info. 
*	@param p_encoder_1 A pointer to a variable in main which tabulates encoder ticks for channel 1
*	@param p_encoder_2 A pointer to a variable in main which tabulates encoder ticks for channel 2
*	@param p_error_1 A pointer to a variable in main which tabulates encoder errors for channel 1
*	@param p_error_2 A pointer to a variable in main which tabulates encoder errors for channel 2
*/

da_encoder::da_encoder (base_text_serial* p_serial_port, int32_t* p_encoder_1, int32_t* p_encoder_2,
						uint16_t* p_error_1, uint16_t* p_error_2)
{
	ptr_to_serial = p_serial_port;          // Store the serial port pointer locally
	ptr_encoder_1 = p_encoder_1;			// Store the other pointers locally too
	ptr_encoder_2 = p_encoder_2;
	ptr_to_error_1 = p_error_1;
	ptr_to_error_2 = p_error_2;
	
	// Note that when the serial port pointer is used to send a message, it must be
	// dereferenced with a "*", meaning "what is pointed to by ptr_to_serial" 
	*ptr_to_serial << "Encoder in the house" << endl;
	
	DDRA &= ~(1<<PIN0); 					// configure PORTA PIN0 as an input
	DDRA &= ~(1<<PIN1); 					// configure PORTA PIN1 as an input
	PORTA |= (1<<PIN0)|(1<<PIN1);			// turn on necessary pull-up resistors [PORTA]
	
	DDRC &= ~(1<<PIN0); 					// configure PORTD PIN0 as an input
	DDRC &= ~(1<<PIN1); 					// configure PORTD PIN1 as an input
	PORTC |= (1<<PIN0)|(1<<PIN1);			// turn on necessary pull-up resistors [PORTD]
	
	PCICR |= (1<<PCIE0)|(1<<PCIE2);			// Enable PC interrupts on PORTS A & D
	PCMSK0 |= (1<<PCINT0)|(1<<PCINT1); 		// Enable interrupts: PORTA pins 0 & 1
	PCMSK2 |= (1<<PCINT16)|(1<<PCINT17);		// Enable interrupts: PORTD pins 24 & 25
	sei();									// Enable interrupts
}

//-------------------------------------------------------------------------------------
/**	Interrupt Service for Motor 1 encoder on PORT A pins 0 and 1. Comparitive statements
*	determine whether motor has rotated forward, backward, or has errored. Any time the
*	ISR has been called and encoder ticks have been missed by either encoder channel an error
* 	is logged. Lastly, current encoder status is archived for comparison on next interrupt.
*/
ISR(PCINT0_vect)
{
	
	new_1 = ((PINA & 0b00000001)|(PINA & 0b00000010));
	uint8_t crap_var=new_1+old_1+old_1;
	if (new_1 + old_1 ==3)
	{
		(*ptr_encoder_1)++;
	}
	else if ((crap_var == 2)||(crap_var == 7))
	{
		(*ptr_encoder_1)++;
	}
	else
	{
		(*ptr_encoder_1)--;
	}
	old_1 = new_1;
}
//-------------------------------------------------------------------------------------
/**	Interrupt Service for Motor 2 encoder on PORT C pins 0 and 1. Comparitive statements
*	determine whether motor has rotated forward, backward, or has errored. Any time the
*	ISR has been called and encoder ticks have been missed by either encoder channel an error
* 	is logged. Lastly, current encoder status is archived for comparison on next interrupt.
*/
ISR(PCINT2_vect)
{
	
	new_2 = ((PINC & 0b00000001)|(PINC & 0b00000010));
	uint8_t crap_var=new_2+old_2+old_2;
	if (new_2 + old_2 ==3)
	{
		(*ptr_encoder_2)++;
	}
	else if ((crap_var == 2)||(crap_var == 7))
	{
		(*ptr_encoder_2)++;
	}
	else
	{
		(*ptr_encoder_2)--;
	}
	old_2 = new_2;
}