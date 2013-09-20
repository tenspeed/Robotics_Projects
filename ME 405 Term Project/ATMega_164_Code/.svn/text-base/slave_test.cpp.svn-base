//======================================================================================
/** \file slave_test.cpp
 
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
#include "Slave_Driver.h"					//!< Include header for encoder class
#include "da_encoder.h"		

//--------------------------------------------------------------------------------------
/** The main function is the "entry point" of every C program, the one which runs first
 *  (after standard setup code has finished). For mechatronics programs, main() runs an
 *  infinite loop and never exits. You must be really sick of reading that.
 */

int main ()
{
	int32_t encoder_1 = 0;					//!< count of encoder ticks seen by encoder 1
	int32_t encoder_2 = 0;					//!< count of encoder ticks seen by encoder 2
	uint16_t error_encoder_1 = 0;			//!< counts encoder errors on channel 1
	uint16_t error_encoder_2 = 0;			//!< counts encoder errors on channel 2
	volatile unsigned long dummy;       	//!< counter which controls screen print out rate
	
	// Create a serial port object. Test voltages will be printed to this port, which
	// should be hooked up to a dumb terminal program like minicom or GTKterm
	rs232 the_serial_port (9600, 0);

	// Say hello
	the_serial_port << endl << "(SPI) Slave Driver with encoder test v0.001" << endl;

	// Create a Slave_Driver object, "transmit". This object must be given a
	// pointer to the serial port object so that it can print debugging information.
	// Pointers to encoder_1 and encoder_2 are also given so that the slave_driver object can 
	// pass those values to Master.
	Slave_Driver transmit(&the_serial_port, &encoder_1, &encoder_2);
	
	// Create an encoder object, "read_encoder". This object must be given a
	// pointer to the serial port object so that it can print debugging information
	// Pointers to encoder and encoder_error variables are also given so that the 
	// encoder object can update those values.
	da_encoder read_encoder (&the_serial_port, &encoder_1, &encoder_2, &error_encoder_1, &error_encoder_2);
	
	sei();
	// Enter infinite main loop
	while (true)
	{	// Print statements unnecessary once the 164 code was fully functional, but useful for debugging so retained.
		/*
		/// Here a dummy loop is used to slow down data print out on terminal (which occurs via position_display).
		if (++dummy >= 100000UL)
		{
			dummy = 0;
			the_serial_port << "SLAVE"<<endl;
			the_serial_port << dec << "encoder 1 reads "<<encoder_1<<" ticks"<<endl;
			the_serial_port << dec << "encoder 2 reads "<<encoder_2<<" ticks"<<endl;
			the_serial_port << dec << "encoder 1 errors "<<error_encoder_1<<endl;
			the_serial_port << dec << "encoder 2 errors "<<error_encoder_2<<endl<<endl;
		}
		*/
		transmit.Packet32();
	
	}
	return (0);
}
