//======================================================================================
/** \file  ADC.h
 *   ADC.h contains specifications necessary for the methods within ADC.cpp to be 
 *   called from within any main program as long as this header file is included.
 *
 *  Revisions:
 *    \li  5-11-12	 Created new ADC header file for Xiphos ADC class.
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

/// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _ADC_H_
#define _ADC_H_


//-------------------------------------------------------------------------------------
/**  Following are the headers of the constructor and methods contained in da_motor.cpp.
*	 Also included is aheader for a serial port object which allows da_motor's methods
*    to print to i/o devices.
*/
 

class ADC
{
	protected:
		/// The ADC class needs a pointer to the serial port used to output to the terminal.  
		base_text_serial* ptr_to_serial;

	public:
		/** The constructor initializes an ADC object.
		*/
		ADC(base_text_serial*);
		
		/** ADC.adcInit initializes the ADC converter.
		*   @param num The analog input to sample (0 to 7).
		*/
		void adcInit ();
		/** manually increase the duty cycle by 2%
		*/
		uint8_t analog (const uint8_t);
		/** manually decrease the duty cycle by 2%
		*/
		uint16_t analog10 (const uint8_t);		
    };
	//-------------------------------------------------------------------------------------

#endif // _ADC_H_
