//======================================================================================
/** \file TWI_Master.h
 *	TWI_Master.h contains specifications necessary for a Master MCU to control the flow
 *	of two-wire communication between itself and any number of Slave MCU's.
 * 
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================
// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _TWI_Master_H_
#define _TWI_Master_H_
//-------------------------------------------------------------------------------------
 /** blink.cpp is a class for blinking an LED connected to an output pin.
 */
class TWI_Master
{
	protected:

	public:
		/** The constructor blink creates a new blink object.
		*/
		TWI_Master (void);
		
		void delay_ms(uint8_t);

		void blink_LED(uint8_t, uint8_t);
};
	//-------------------------------------------------------------------------------------
#endif // _TWI_Master_H_