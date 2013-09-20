// System headers included with < >
	#include <stdlib.h>							// Standard C library
	#include <avr/io.h>							// Input-output ports, special registers
	#include <avr/interrupt.h>					// Interrupt handling functions
	#include "rs232int.h"						// Include header for serial port class
	#include "servo.h"							// include own header file


//-------------------------------------------------------------------------------------

/** CONSTRUCTOR initializes PWM output for a RC servo motor. and sets pen height to a safe
*	value. Servo output pins are A0 & A1
*	@param p_serial_port Serial object so we can write to screen
*/
servo::servo(base_text_serial* p_serial_port)
{
	// save object pointers locally
	ptr_2_serial = p_serial_port;
	
	// say hello
	*ptr_2_serial << "servo is servo-ing"<<endl;
	
	// set PWM output pin
	DDRE |= (1<<PIN3);
	
	// Set timer3 to operate in non-inverting fast PWM mode 
	TCCR3A |= (1<<COM3A1)|(1<<COM3B1);
	TCCR3A &= ~(1<<COM3A0); // Set to 0 to operate in non-inverting mode
	TCCR3A &= ~(1<<COM3B0); // Set to 0 to operate in non-inverting mode
	
	// set up fast PWM and initialize timer
	//8 bit
	TCCR3A |= (1<<WGM30);  // set timer1 to 8 bit fast PWM [step 1]
	TCCR3A &= ~(1<<WGM31); // set timer1 to 8 bit fast PWM [step 2]
	TCCR3B |= (1<<WGM32);
	TCCR3B &= ~(1<<WGM33); // set timer1 for 8 bit fast PWM 
	
	// prescalar settings clk/64
	TCCR3B |= (1<<CS30); // controls the prescaler bit 0
	TCCR3B &= ~(1<<CS31); // controls the prescaler bit 1
	TCCR3B |= (1<<CS32); // controls the prescaler bit 2
	
	// initialize variables
	OCR3AL = 5;			// initialize servo duty cycle
}

void servo::Set_Angle(uint16_t angle)
{
	if (angle <= 360)
	{
		ServoSet = (uint8_t)angle*255/720;
		OCR3AL = ServoSet;
	}
}


	
	
	
	
	
	