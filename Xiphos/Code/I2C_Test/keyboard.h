//======================================================================================
/** \file  keyboard.h
 *	
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

/// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _keyboard_H_
#define _keyboard_H_


//-------------------------------------------------------------------------------------

 

class keyboard
{
	protected:
		/// pointer of type LCD
		LCD* LCD_function;

		uint8_t r1;
		
		uint8_t c1;

	public:

		keyboard(LCD*);
		
		void initialize();
		
		void print_a_char(char);
		
		void delete_a_char();
		
		void clear_screen();
		
};
	//-------------------------------------------------------------------------------------

#endif // _keyboard_H_