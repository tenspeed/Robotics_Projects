//======================================================================================
/** \file TWI_Slave.h
 *	TWI_Slave.h contains specifications necessary for a Slave MCU to communicate with
 *	 a Master MCU on a two-wire serial bus.
 * 
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================
// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _TWI_Slave_H_
#define _TWI_Slave_H_
//-------------------------------------------------------------------------------------
 /** blink.cpp is a class for blinking an LED connected to an output pin.
 */
class TWI_Slave
{
	protected:
		blink* blink_obj;
	public:
		/** The constructor blink creates a new blink object.
		*/
		TWI_Slave(blink* blink_object);

		void TWIInit(void);
};
	//-------------------------------------------------------------------------------------
#endif // _TWI_Slave_H_