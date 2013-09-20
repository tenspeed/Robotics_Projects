//======================================================================================
/** \file  da_motor.cpp
 *  
 *  Revisions:
 *            \li  04-12-11  Began tearing and hacking at our lab_2 code.
 *	          \li  04-18-11  Final iteration. This code is the bestest.
 *    
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

#include <stdlib.h>							// Include standard library header files
#include <avr/io.h>							// You'll need this for SFR and bit names

#include "rs232int.h"						// Include header for serial port class
#include "da_motor.h"						// Include header for the A/D class


//-------------------------------------------------------------------------------------
/** This constructor configures the PWM timer on the ATmega128 and the H-bridges on the VNH motor driver chips
*	set up a motor driver.
*   @param p_serial_port A pointer to the serial port which writes debugging info. 
*/
da_motor::da_motor (base_text_serial* p_serial_port)
{
	ptr_to_serial = p_serial_port;          // Store the serial port pointer locally.
	
	// Set timer1 to operate in non-inverting fast PWM mode. 
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1);
	TCCR1A &= ~(1<<COM1A0); // Set to 0 to operate in non-inverting mode.
	TCCR1A &= ~(1<<COM1B0); // Set to 0 to operate in non-inverting mode.
	
	// Set up fast PWM and initialize timer

	//8 bit
	TCCR1A |= (1<<WGM10);  // Set timer1 to 8 bit fast PWM mode [step 1]
	TCCR1A &= ~(1<<WGM11); // Set timer1 to 8 bit fast PWM mode [step 2]
	TCCR1B |= (1<<WGM12);  // Set timer1 to 8 bit fast PWM mode [step 3]
	TCCR1B &= ~(1<<WGM13); // Set timer1 to 8 bit fast PWM mode [step 4]
	
	// 10 bit
	// TCCR1A |= (1<<WGM10)|(1<<WGM11);   // Set timer1 to 10 bit fast PWM mode [step 1].
	// TCCR1B |= (1<<WGM12);			  // Set timer1 to 10 bit fast PWM mode [step 1].
	// TCCR1B &= ~(1<<WGM13);			  // Set timer1 to 10 bit fast PWM mode [step 1].
	
	// prescalar settings clk/256
	TCCR1B |= (1<<CS10);  // Controls the prescaler bit 0
	TCCR1B |= (1<<CS11);  // Controls the prescaler bit 1
	TCCR1B &= ~(1<<CS12); // Controls the prescaler bit 2
	
	// miscellaneous timer options
	TCCR1B &= ~(0b00100000); // Bit # 5 is reserved and must be set to zero.
	TCCR1B &= ~(1<<ICNC1);   // This bit is for the input capture noise canceler. We don't care about it.
	TCCR1B &= ~(1<<ICES1);   // This bit is for the input capture edge select. We also don't care about it.
	
	// Set up atmega ports to input for diagnostic purposes while using pull-up resistors
	// to enable motor control chip on.
 	DDRC &= ~(1<<PIN2); // Sets Port C Pin 2 as input
 	DDRD &= ~(1<<PIN7); // Sets Port D Pin 7 as input
 	PORTC |= (1<<PIN2); // Sets Port C Pin 2 pull-up resistor to ON
 	PORTD |= (1<<PIN7); // Sets Port D Pin 7 pull-up resistor to ON
	// set PWM out put ports
	DDRB |= (1<<PIN5)|(1<<PIN6);
	// Set outputs for H-bridges
 	DDRC |= (1<<PIN0)|(1<<PIN1);
 	DDRD |= (1<<PIN5)|(1<<PIN6);
}


//-------------------------------------------------------------------------------------
/** da_motor.set_mode allows a user to choose a motor's rotational direction or apply 
*   unmodulated braking. These tasks are performed by passing motor_num a 1 or a 2 to
*	select which motor's state to change and mode a 1, 2, or 3.
*   @param motor_num: selects motor number 1 or 2.
*   @param mode: mode 1 = spin CW    mode 2 = spin CCW   mode 3 = unmodulated braking.
*/
void da_motor::set_mode (uint8_t motor_num, uint8_t mode)
{
	// determine which motor to set
	if (motor_num == 1)
	{
		// set direction
		if (mode == 1)
		{
			PORTC |= (1<<PIN0);
			PORTC &= ~(1<<PIN1);
		}
		// set direction
		else if (mode == 2)
		{
			PORTC |= (1<<PIN1);
			PORTC &= ~(1<<PIN0);
		}
		// set braking
		else if (mode == 3)
		{
			PORTC |= (1<<PIN0)|(1<<PIN1);
		}
	}
	// determine which motor to set
	if (motor_num == 2)
	{
		// set direction
		if (mode == 1)
		{
			PORTD |= (1<<PIN5);
			PORTD &= ~(1<<PIN6);
		}
		// set direction
		else if (mode == 2)
		{
			PORTD |= (1<<PIN6);
			PORTD &= ~(1<<PIN5);
		}
		// set braking
		else if (mode == 3)
		{
			PORTD |= (1<<PIN5)|(1<<PIN6);
		}
	}
	
}
//-------------------------------------------------------------------------------------
/** manually increase the duty cycle by 2%
*	da_motor.increase_duty_cycle allows a user to increase the rotational
*	speed of a motor. The motor who's speed is adjusted is whichever motor (1 or 2)
*	was previously selected.
*   @param motor_num: determines which motor is adjusted
*	@param duty_cycle: a pointer to duty_cycle allows this method to alter
*                               values in duty cycle (a variable in main)
*/
void da_motor::increase_duty_cycle (uint8_t motor_num, uint8_t* duty_cycle)
{
	if (*duty_cycle == 255)
	{
		//*ptr_to_serial<< "Duty cycle at maximum "<< *duty_cycle << endl; // debug statement
	}
	else
	{
		*duty_cycle += 5;
	}
	
	
	//*ptr_to_serial<< "THIS IS THE NEW DUTY CYCLE: "<< *duty_cycle << endl; // debug statement
	
	if (motor_num == 1)
	{
	OCR1BL = *duty_cycle;
	}
	else if (motor_num == 2)
	{
	OCR1AL = *duty_cycle;
	}
	
}
//-------------------------------------------------------------------------------------
/** manually decrease the duty cycle by 2%
*	da_motor.decrease_duty_cycle allows a user to decrease the rotational
*	speed of a motor. The motor who's speed is adjusted is whichever motor (1 or 2)
*	was previously selected.
*   @param motor_num: determines which motor is adjusted
*	@param duty_cycle: a pointer to duty_cycle allows this method to alter
*                               values in duty cycle (a variable in main)
*/
void da_motor::decrease_duty_cycle (uint8_t motor_num, uint8_t* duty_cycle)
{
	if (*duty_cycle == 0)
	{
		*duty_cycle = 0;
	}
	else
	{
		*duty_cycle -= 5;
	}
	//*ptr_to_serial<< "THIS IS THE NEW DUTY CYCLE: "<< *duty_cycle << endl; // debug statement
	
	if (motor_num == 1)
	{
	OCR1BL = *duty_cycle;
	}
	else if (motor_num == 2)
	{
	OCR1AL = *duty_cycle;
	}
	
}
	
//-------------------------------------------------------------------------------------
/**	update_duty_cycle is a method which allows you to update the a motor's duty cycle
*	with any 8-bit value you want instead of incrementally.
*/
void da_motor::update_duty_cycle (uint8_t motor_num, uint8_t duty_cycle)
{

	if (motor_num == 1)
	{
		OCR1BL = duty_cycle;
	}
	else if (motor_num == 2)
	{
		OCR1AL = duty_cycle;
	}
		
}

//-------------------------------------------------------------------------------------
/** This operator prints a humorous message on a text serial device. We enjoyed it. 
*   @param serial: A pointer to a serial object which allows us to print
*   @param my_motor: a command which prints our joke. We could have put something valuable
*					 here though if we had something we wanted to shortcut to.
*/
base_text_serial& operator<< (base_text_serial& serial, da_motor& my_motor)
{
	serial << "Hi, Could you call me a cab? I'm overloaded." << endl; // lol
	

	// This statement should be left here; you must return a reference to the serial
	// device so that the "<<" operator can be used again and again on the same line
	return (serial);
}