//======================================================================================
/** \file  blink.h
 *	blink.h contains specifications necessary for blinking an LED connected to an output
 *  pin.
 * 
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================
// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _blink_H_
#define _blink_H_
//-------------------------------------------------------------------------------------
 /** blink.cpp is a class for blinking an LED connected to an output pin.
 */
class blink
{
	protected:

	public:
		/** The constructor blink creates a new blink object.
		*/
		blink (void);
		
		void delay_ms(uint8_t);

		void blink_LED(uint8_t, uint8_t);
};
	//-------------------------------------------------------------------------------------
#endif // _blink_H_