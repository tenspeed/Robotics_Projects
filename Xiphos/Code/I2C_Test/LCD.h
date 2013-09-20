//======================================================================================
/** \file  LCD.h
 *   ADC.h contains specifications necessary for the methods within ADC.cpp to be 
 *   called from within any main program as long as this header file is included.
 *
 *  Revisions:
 *    \li  5-11-12	 Created new LCD header file for Xiphos LCD class.
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

/// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _LCD_H_
#define _LCD_H_


//-------------------------------------------------------------------------------------
/**  Following are the headers of the constructor and methods contained in da_motor.cpp.
*	 Also included is aheader for a serial port object which allows da_motor's methods
*    to print to i/o devices.
*/
 

class LCD
{
	protected:
		/// The ADC class needs a pointer to the serial port used to output to the terminal.  
		base_text_serial* ptr_to_serial;
		
		utility* utility_function;
		
		uint8_t address;

	public:
		/** The constructor initializes an ADC object.
		*/
		LCD(base_text_serial*, utility*);
		
		/** ADC.adcInit initializes the ADC converter.
		*   @param num The analog input to sample (0 to 7).
		*/
		void writeLcd(uint8_t);
		/** manually increase the duty cycle by 2%
		*/
		void writeControl(const uint8_t);
		/** manually decrease the duty cycle by 2%
		*/
		uint8_t CursorAddress();
		
		void clearScreen();

		void lcdOn();
		
		void lcdOff();
				
		void lcdCursorOn();
		
		void lcdCursorOff();
		
		void lcdCursorBlink();
		
		void lcdCursorIncrement();
		
		void lcdCursorDecrement();
		
		void lcdInit();
		
		void printChar(const uint8_t);
		
		void printString(const char *const);
		
		void print_uint8_t(const uint8_t);
		
		void printPlain_uint8_t(const uint8_t);
		
		void print_int8_t(const int8_t);
		
		void printPlain_int8_t(const int8_t);
		
		void print_uint16_t(const uint16_t);
		
		void printPlain_uint16_t(const uint16_t);
		
		void print_int16_t(const int16_t);
		
		void printPlain_int16_t(const int16_t);
		
		void printHexDigit(uint8_t);
		
		void printHex_uint8_t(const uint8_t);
		
		void printHex_uint16_t(const uint16_t);
		
		void printFloat(const float);
		
		void upperLine();
		
		void upperLineEnd();
		
		void lowerLine();
		
		void lcdCursor(const uint8_t, const uint8_t);
		
};
	//-------------------------------------------------------------------------------------

#endif // _LCD_H_
