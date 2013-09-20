//Copyright (C) 2009-2010  Patrick J. McCarty.
//Licensed under X11 License. See LICENSE.txt for details.

/*! \file LCD.cpp
    Implements support for a generic 16-column x 2-row LCD display compatible with
    the common HD44780 LCD driver chip. It is interfaced in 8-bit mode (8 data lines)
    and uses an additional 2 control lines: E (Enable) and RS (Register Select).
    The R/W (Read/Write) pin is hardwired to ground so the LCD is write-only. Therefore, this
    driver uses fixed delays following every command to the LCD instead of polling the LCD's
    status register. Consequently, it may delay longer than necessary, but it saves a pin.
 */
#include <stdlib.h>							//!< Include standard library header files
#include <avr/io.h>							//!< You'll need this for SFR and bit names

#include "rs232int.h"						//!< Include header for serial port class
#include "globals.h"
#include "utility.h"
#include "LCD.h"

//! LCD RAM address for the home position (row 0, col 0).
#define HOME         0x80
//! LCD RAM address for the second line (row 1, col 0).
#define SECOND_LINE  0XC0
#define FIRST_LINE_END 0x8F

/*! Macro function to reverse the bit order of an 8-bit variable as efficiently as possible.
    Should compile down to just 15 AVR assembly instructions, running in 15 clock cycles.
    Note the use of the swap assembly instruction to swap the two nibbles of a register.
    @param a The 8-bit variable to reverse.
 */
#define REVERSE(a) do                     \
{                                         \
  a=((a>>1)&0x55)|((a<<1)&0xAA);          \
  a=((a>>2)&0x33)|((a<<2)&0xCC);          \
  asm volatile("swap %0":"=r"(a):"0"(a)); \
} while (0)

LCD::LCD(base_text_serial* p_serial_port, utility* utility_object)  // Constructor for the LCD class
{
	ptr_to_serial = p_serial_port;          // Store the serial port pointer locally
	
	utility_function = utility_object;
}

//! Writes a byte of data to the LCD.
void LCD::writeLcd(uint8_t data)
{
	//Reverse the bit order of the data, due to LCD connections to the data bus being backwards.
	//This line doesn't affect the bus, so this can be done before the interrupts are disabled.
	REVERSE(data);
	//Disable interrupts to prevent the servo ISR (which shares the same data bus)
	//from interrupting in the middle of the sequence and messing with the bus.
	cli();
	//set the LCD's R/W line low to put the LCD in write mode.
	cbi(PORTA, PA7);
	//set the LCD's E (Enable) line high, so it can fall later
	sbi(PORTD, PD6);
	//write the data to the bus
	PORTC = data;
	//delay to allow the data to fully propagate to the LCD
	utility_function->delayUs(1);
	//set the LCD's E (Enable) line low to latch in the data
	cbi(PORTD, PD6);
	//re-enable interrupts
	sei();
}

//! Writes a command byte to the LCD.
void LCD::writeControl(const uint8_t data)
{
	//set RS (Register Select) line low to select command register
	cbi(PORTD, PD7);
	writeLcd(data);
	//wait for the instruction to be executed
	utility_function->delayUs(100);
}

uint8_t LCD::CursorAddress()
{

	//Disable interrupts to prevent the servo ISR (which shares the same data bus)
	//from interrupting in the middle of the sequence and messing with the bus.
	cli();
	// configure PORTC for input
	DDRC = 0X00;
	// enable pull-up resistors
	PORTC = 0XFF;
	//set RS (Register Select) line low to select command register
	cbi(PORTD, PD7);
	// set the LCD's R/W line to high to read from the data registers
	sbi(PORTA, PA7);
	//set the LCD's E (Enable) line high, so it can fall later
	sbi(PORTD, PD6);
	//wait for the instruction to be executed
	utility_function->delayUs(1);
	//store the cursor address
	address = PORTC;
	//set the LCD's E (Enable) line low
	cbi(PORTD, PD6);
	//Reverse the bit order of the data, due to LCD connections to the data bus being backwards.
	//This line doesn't affect the bus, so this can be done before the interrupts are disabled.
	REVERSE(address);
	// configure PORTC for output so we can print to the LCD again
	DDRC = 0xFF;
	// disable pull-up resistors
	PORTC = 0X00;
	//set the LCD's R/W line to low so the LCD returns to write mode.
	cbi(PORTA, PA7);
	//re-enable interrupts
	sei();
	return(address);
	
}

//! Clears all characters on the display and resets the cursor to the home position.
void LCD::clearScreen()
{
	writeControl(0x01);
	utility_function->delayUs(3300);
}

//! Shows the characters on the screen, if they were hidden with lcdOff().
void LCD::lcdOn()
{
	writeControl(0x0C);
}

//! Hides the characters on the screen. Can be unhidden again with lcdOn().
void LCD::lcdOff()
{
	writeControl(0x08);
}

void LCD::lcdCursorOn()
{
	writeControl(0x0E);
}

void LCD::lcdCursorOff()
{
	writeControl(0x0C);
}

void LCD::lcdCursorBlink()
{
	writeControl(0x0D);
}

void LCD::lcdCursorIncrement()
{
	writeControl(0x06);
}

void LCD::lcdCursorDecrement()
{
	writeControl(0x04);
}
/*! Initializes the LCD as described in the HD44780 datasheet.
    Normally called only by the initialize() function in utility.cpp
 */
void LCD::lcdInit()
{
	//configure LCD E (Enable) control pin as an output
	sbi(DDRD, DDD6);
	//configure LCD RS (Register Select) control pin as an output
	sbi(DDRD, DDD7);
	//configure LCD R/W (Read/Write) control pin as an output
	sbi(DDRA, DDA7);
	//set LCD R/W (Read/Write) control pin low initially
	cbi(PORTA, PA7);
	//set LCD E (Enable) line low inititally, so it can rise later
	cbi(PORTD, PD6);

	//wait 15ms after power on
	utility_function->delayMs(15);

	//Issue 'Function Set' commands to initialize LCD for 8-bit interface mode
	writeControl(0x38);
	utility_function->delayUs(4900); //+100us in writeControl = 5000us or 5ms total
	writeControl(0x38);
	utility_function->delayUs(50); //+100us in writeControl = 150us total
	writeControl(0x38);

	//Function Set command to specify 2 display lines and character font
	writeControl(0x38);

	//Display off
	lcdOff();

	//Clear display
	clearScreen();

	//Set entry mode
	writeControl(0x06);

	//Display on
	lcdOn();
}

/*! Prints a single character specified by its ASCII code to the display.
    Most LCDs can also print some special characters, such as those in LCDSpecialChars.h.
 */
void LCD::printChar(const uint8_t data)
{
	//set RS (Register Select) line high to select data register
	sbi(PORTD, PD7);
	writeLcd(data);
	utility_function->delayUs(50);
}

/*! Prints a null-terminated string starting at the current cursor position.
    Strings exceeding the length of the display go into the display's buffer.
    This is useful if you are using the scrolling option, but wastes write time otherwise.
    Once the buffer for the first line has been filled, the cursor wraps to the next line.
 */
void LCD::printString(const char *const string)
{
	uint8_t i = 0;
	//keep printing until we encounter a null terminator
	while (string[i] != 0)
	{
		printChar(string[i++]);
	}
}

//! Prints an 8-bit unsigned integer as 3 digits (with '0' padding in front). Example: "007"
void LCD::print_uint8_t(const uint8_t number)
{
	//print hundreds digit
	printChar((number / 100) + 48);
	//print tens digit
	printChar(((number % 100 ) / 10) + 48);
	//print ones digit
	printChar((number % 10) + 48);
}

//! Prints an 8-bit unsigned integer as 1-3 digits (no padding). Example: "7"
void LCD::printPlain_uint8_t(const uint8_t number)
{
	//if there is a digit in the tens place
	if (number >= 10)
	{
		//if there is a digit in the hundreds place
		if (number >= 100)
		{
			//print the hundreds digit
			printChar((number / 100) + 48);
		}
		//print the tens digit
		printChar(((number % 100 ) / 10) + 48);
	}
	//always print the ones digit
	printChar((number % 10) + 48);
}

//! Prints an 8-bit signed integer as +/- and 3 digits (with '0' padding in front). Example: "+007"
void LCD::print_int8_t(const int8_t number)
{
	if (number >= 0)
	{
		//print plus
		printChar('+');
		//cast to print the number as a u08
		print_uint8_t((uint8_t)number);
	}
	else
	{
		//print minus
		printChar('-');
		//make the number positive and print it as a u08
		print_uint8_t((uint8_t)(-number));
	}
}

//! Prints an 8-bit signed integer as +/- and 1-3 digits (no padding). Example: "+7"
void LCD::printPlain_int8_t(const int8_t number)
{
	if (number >= 0)
	{
		//print plus
		printChar('+');
		//cast to print the number as a u08
		printPlain_uint8_t((uint8_t)number);
	}
	else
	{
		//print minus
		printChar('-');
		//make the number positive and print it as a u08
		printPlain_uint8_t((uint8_t)(-number));
	}
}

//! Prints a 16-bit unsigned integer as 5 digits (with '0' padding in front). Example: "00042"
void LCD::print_uint16_t(const uint16_t number)
{
	//print the ten-thousands digit
	printChar((number / 10000) + 48);
	//print the thousands digit
	printChar(((number % 10000) / 1000) + 48);
	//print the hundreds digit
	printChar(((number % 1000) / 100) + 48);
	//print the tens digit
	printChar(((number % 100 ) / 10) + 48);
	//print the ones digit
	printChar((number % 10) + 48);
}

//! Prints a 16-bit unsigned integer as 1-5 digits (no padding). Example: "42"
void LCD::printPlain_uint16_t(const uint16_t number)
{
	//if there is a digit in the tens place
	if (number >= 10)
	{
		//if there is a digit in the hundreds place
		if (number >= 100)
		{
			//if there is a digit in the thousands place
			if (number >= 1000)
			{
				//if there is a digit in the ten-thousands place
				if (number >= 10000)
				{
					//print the ten-thousands digit
					printChar((number / 10000) + 48);
				}
				//print the thousands digit
				printChar(((number % 10000) / 1000) + 48);
			}
			//print the hundreds digit
			printChar(((number % 1000) / 100) + 48);
		}
		//print the tens digit
		printChar(((number % 100 ) / 10) + 48);
	}
	//always print the ones digit
	printChar((number % 10) + 48);
}

//! Prints a 16-bit signed integer as +/- and 5 digits (with '0' padding in front). Example: "+00042"
void LCD::print_int16_t(const int16_t number)
{
	if (number >= 0)
	{
		//print plus
		printChar('+');
		//cast to print the number as a u16
		print_uint16_t((uint16_t)number);
	}
	else
	{
		//print minus
		printChar('-');
		//make the number positive and print it as a u16
		print_uint16_t((uint16_t)(-number));
	}
}

//! Prints a 16-bit signed integer as +/- and 1-5 digits (no padding). Example: "+42"
void LCD::printPlain_int16_t(const int16_t number)
{
	if (number >= 0)
	{
		//print plus
		printChar('+');
		//cast to print the number as a u16
		printPlain_uint16_t((uint16_t)number);
	}
	else
	{
		//print minus
		printChar('-');
		//make the number positive and print it as a u16
		printPlain_uint16_t((uint16_t)(-number));
	}
}

//! Prints a single uppercase hex digit representing the lower 4 bits of the argument.
void LCD::printHexDigit(uint8_t nibble)
{
	//strip off everything except the lower nibble (4 bits)
	nibble &= 0x0F;
	//check what ASCII character offset we need to add
	if (nibble < 10)
	{
		//print 0 through 9. ASCII char '0' evaluates to 48.
		printChar(nibble + '0');
	}
	else
	{
		//print A through F. You can change to lowercase if you want by using 'a' instead.
		//'A' (ASCII char 65) - 10 = 55
		//'a' (ASCII char 97) - 10 = 87
		printChar(nibble + ('A' - 10));
	}
}

//! Prints an 8-bit unsigned integer as 2 hex digits. Example(255): "FF"
void LCD::printHex_uint8_t(const uint8_t number)
{
	//print the upper nibble
	printHexDigit(number >> 4);
	//print the lower nibble
	printHexDigit(number);
}

//! Prints a 16-bit unsigned integer as 4 hex digits. Example(65535): "FFFF"
void LCD::printHex_uint16_t(const uint16_t number)
{
	//print the high nibble
	printHexDigit((uint8_t)(number >> 12));
	//print the upper middle nibble
	printHexDigit((uint8_t)(number >> 8));
	//print the lower middle nibble
	printHexDigit(((uint8_t)number) >> 4);
	//print the low nibble
	printHexDigit((uint8_t)number);
}

//! Prints a floating point number.
void LCD::printFloat(const float number)
{
	char s[10];
	dtostre(number, s, 3, 0);
	printString(s);
}

//! Moves the LCD cursor to the beginning of the first line of the display (row 0, col 0).
void LCD::upperLine()
{
	writeControl(HOME);
}

void LCD::upperLineEnd()
{
	writeControl(FIRST_LINE_END);
}

//! Moves the LCD cursor to the beginning of the second line of the display (row 1, col 0).
void LCD::lowerLine()
{
	writeControl(SECOND_LINE);
}

/*! Moves the LCD cursor position directly to the specified row and column.
    @param row Valid row values are 0 to 1.
    @param column Valid column values are 0 to 15.
 */
void LCD::lcdCursor(const uint8_t row, const uint8_t column)
{
	writeControl(HOME | (row << 6) | (column % 17));
}
