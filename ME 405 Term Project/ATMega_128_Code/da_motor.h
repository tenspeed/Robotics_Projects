//======================================================================================
/** \file  da_motor.h
 *   da_motor.h contains specifications necessary for the methods within da_motor.cpp to be 
 *   called from within any main program as long as this header file is included.
 *
 *  Revisions:
 *    \li  11-04-11	 Included specification for the method "read_a_few" within avr_adc.cpp
 *    \li  04-12-11  Began tearing and hacking at our lab_2 code.
 *	  \li  04-18-11  Final iteration. This code is the bestest.
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _da_motor_H_
#define _da_motor_H_


//-------------------------------------------------------------------------------------
/**   da_motor.cpp is an object class that contains a constructor and three methods which 
 *	  control a motor(s). The constructor initializes timers, prescalars and configures 
 *    necessary i/o ports. Two of the three methods are mirror images of each other and  
 *	  are used to increase/decrease rotational speed. The final method sets the H bridge 
 *    to achieve proper rotational direction and braking.
*/
 

class da_motor
{
	protected:
		// The motor driver class needs a pointer to the serial port used to output to the terminal.  
		base_text_serial* ptr_to_serial;

	public:
		da_motor(base_text_serial*);
		
		void set_mode (uint8_t, uint8_t);

		void increase_duty_cycle (uint8_t, uint8_t*);

		void decrease_duty_cycle (uint8_t, uint8_t*);

		void update_duty_cycle (uint8_t, uint8_t);
		
			
    };
	//-------------------------------------------------------------------------------------
/** This operator prints some BS on a text serial device. We didn't have the heart
*   to erase the cute 'lil fella.
*/

base_text_serial& operator<< (base_text_serial&, da_motor&);

#endif // _da_motor_H_
