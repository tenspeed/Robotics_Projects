//======================================================================================
/** \file  da_encoder.h
 *   da_encoder.h contains specifications necessary for the methods within da_encoder.cpp to be 
 *   called from within any main program as long as this header file is included.
 *
 *  Revisions:
 *    \li  04-18-11  Began tearing and hacking at our lab_3 code.
 *	  \li  04-21-11  Final iteration. Sweet.
 *	  \li  04-25-11  I lied. Doxygen comments added today.
 *	  \li  05-04-11  began integrating into slave draver
 *	  \li  05-05-11  this gutted version of encoder does all lab5 requires and nothing more
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

/// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _da_encoder_H_
#define _da_encoder_H_


//-------------------------------------------------------------------------------------
/**  Following are the headers of the constructor and methods contained in da_encoder.cpp.
*/
class da_encoder
{
	protected:
		/// The encoder driver class needs a pointer to the serial port used to output to the terminal.  
		base_text_serial* ptr_to_serial;

	public:
		/** The constructor says hello using the serial port which is specified in the pointer given to 
		*	it. It is also given pointers to two variables in main which count encoder ticks and two
		*	which count the number of encoder errors.
		*/
		da_encoder(base_text_serial*, int32_t* , int32_t* ,uint16_t* , uint16_t* );
};
	//-------------------------------------------------------------------------------------
#endif // _da_encoder_H_