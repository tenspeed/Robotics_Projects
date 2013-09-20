	//======================================================================================
	/** \file doubler.cpp
	
	*  Revisions:
	*    \li  08-29-13  doubler program created.				
	*  License:
	*    This file released under the Lesser GNU Public License. The program is intended
	*    for educational use only, but its use is not restricted thereto. 
	*/
	//======================================================================================

	// System headers included with < >
	// User written headers included with " "
	#include <stdlib.h>							//!< Standard C library
	#include <avr/io.h>							//!< Input-output ports, special registers
	#include <avr/interrupt.h>					//!< Interrupt handling functions
	#include "rs232int.h"						//!< Include header for serial port class
	#include "stl_timer.h"						//!< Microsecond-resolution timer
	#include "stl_task.h"						//!< Base class for all task classes
	#include "globals.h"						//!< Include header for global definitions
	#include "utility.h"
	#include "LCD.h"							//!< Include header for the LCD class
	#include "keyboard.h"						//!< Include header for the display class
	
	
	//======================================================================================
	
	int main()
	{
		char temp_var;
		rs232 the_serial_port (9600, 0); 					// Create a serial port object to communicate via serial.					
															// If serial communication doesn't work, check the port number.
		
		time_stamp interval_time (0, 25000);  			// Create a time stamp which holds the interval that each task is called.
															// The time stamp is initialized with a number of seconds, then microseconds.
		
		task_timer the_timer;								// Create a microsecond-resolution timer.
		
		utility utility_OBJ(&the_serial_port);				// Create a utility object to use handy pre-built methods like micro and
															// milli second delays.

		LCD LCD_OBJ(&the_serial_port, &utility_OBJ);		// Create an LCD object for handling output to the LCD screen.
		keyboard user_input(&LCD_OBJ);
		
		LCD_OBJ.lcdInit();									// Initialize the LCD screen.
		LCD_OBJ.printString("input:");							// print msg1 on first row
		LCD_OBJ.lcdCursor(1,0);								// move cursor to start of second row
		LCD_OBJ.printString("output:");							// print msg2 on second row
		LCD_OBJ.lcdCursor(0,8);
		LCD_OBJ.lcdCursorOn();								// Turn cursor on
		LCD_OBJ.lcdCursorBlink();							// Blink cursor
		
		while(1) 
		{
			if(the_serial_port.check_for_char())
			{
				temp_var = the_serial_port.getchar();
				if(temp_var == 0x7F) //check if backspace is pressed
				{
					user_input.delete_a_char();
				}
				else if(temp_var == 0x0D) //check if enter is pressed
				{
					// initiate data send over I2C
				}
				else
				{
					user_input.print_a_char(temp_var); //print everything else
				}
			}
		};										
	}