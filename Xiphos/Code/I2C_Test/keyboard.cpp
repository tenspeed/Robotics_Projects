//Copyright (C) 2009-2010  Patrick J. McCarty.
//Licensed under X11 License. See LICENSE.txt for details.

/*! \file keyboard.cpp
    keyboard_input.cpp handles keyboard input to display on the LCD screen in real time.
	
 */
#include <stdlib.h>							//!< Include standard library header files
#include <avr/io.h>							//!< You'll need this for SFR and bit names

#include "rs232int.h"						//!< Include header for serial port class
#include "globals.h"						//!< Include header for globals
#include "utility.h"						//!< Include header for utility
#include "LCD.h"							//!< Include header for LCD class
#include "keyboard.h"						//!< Include header for keyboard class

// Constructor for the keyboard class
keyboard::keyboard(LCD* LCD_object)
{
	LCD_function = LCD_object;				// Pointer to the LCD object.
	
	LCD_function->clearScreen();			// Clear the LCD when initializing.
	LCD_function->lcdCursorBlink();			// Make the cursor blink.
	uint8_t r1 = 0;							// Initialize cursor row counter.
	uint8_t c1 = 0;							// Initialize cursor collumn counter.
}
void keyboard::initialize()					// Method to reinitialize the LCD screen.
{
	LCD_function->upperLine();
	LCD_function->clearScreen();
	LCD_function->lcdCursorBlink();
	c1 = 0;
	r1 = 0;
}
void keyboard::print_a_char(char input) 	// Method to print a single user input char to the LCD screen.
{
	if (c1 == 16 && r1 == 1)				// Check to make sure the cursor is not at the end of the second row.
	{
		return;
	}
	else
	{
		c1++;
		LCD_function->printChar(input);		// Print the char
		if (c1 == 16 && r1 == 0)			// Check to see if the cursor is at the end of the first row; if so,
		{									// print the char in the first collumn of the second row.
			c1 = 0;
			r1 = 1;
			LCD_function->lowerLine();
			return;
		}
	}
}
void keyboard::delete_a_char()				// Method to delete a char from the LCD screen.
{
	if (c1 == 0 && r1 == 0)
	{
		return;
	}
	else if (c1 == 0 && r1 == 1)
	{
		c1 = 15;
		r1 = 0;
		LCD_function->upperLineEnd();
		LCD_function->printChar(0x20);
		LCD_function->upperLineEnd();
		return;
	}
	else
	{
		c1--;
		LCD_function->lcdCursor(r1, c1);
		LCD_function->printChar(0x20);
		LCD_function->lcdCursor(r1, c1);
		return;
	}
}	
void keyboard::clear_screen()				// Method to clear the LCD screen.
{
	LCD_function->clearScreen();
	LCD_function->upperLine();
	c1 = 0;
	r1 = 0;
	return;
}