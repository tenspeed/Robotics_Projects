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
		blink* blink_obj;
	public:
		/** The constructor blink creates a new blink object.
		*/
		TWI_Master(blink* blink_object);
		
		void TWIInit(void);

		void TWIStart(void);

		void TWIStop(void);

		void TWIWrite(uint8_t data);

		uint8_t TWIReadACK(void);

		uint8_t TWIReadNACK(void);
};
	//-------------------------------------------------------------------------------------
#endif // _TWI_Master_H_