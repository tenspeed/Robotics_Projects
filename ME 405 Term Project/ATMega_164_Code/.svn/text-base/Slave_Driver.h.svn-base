//======================================================================================
/** \file  Slave_Driver.h
 *   Slave_Driver.h contains specifications necessary for the methods of said driver to be 
 *   called from within any main program as long as this header file is included.
 *
 *  Revisions:
 *    \li  04-26-11  Began tearing and hacking at our lab_4 code.
 *	  \li  05-05-11  This file is done.
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

/// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _Slave_Driver_H_
#define _Slave_Driver_H_


//-------------------------------------------------------------------------------------
/**  Following are the headers of the constructor and methods contained in da_encoder.cpp.
*	 Also included is aheader for a serial port object which allows da_encoder's methods
*    to print to i/o devices.
*/
 

class Slave_Driver
{
	protected:
		/// The encoder driver class needs a pointer to the serial port used to output to the terminal.  
		base_text_serial* ptr_to_serial;

	public:
		/** The constructor says hello using the serial port which is specified in the pointer given to
		*	it. It is also passed pointers to two variables in main which count encoder ticks
		*/
		Slave_Driver(base_text_serial*, int32_t* p_encoder_1, int32_t* p_encoder_2);	
		/** Packet32 is a method which places encoder data into a buffer which is easily accesible
		*	for the ISR to pass the data in packets [via union] to the master via SPI
		*/
		void Packet32(void);
		
};

#endif // _Slave_Driver_H_