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
#ifndef _utility_H_
#define _utility_H_


//-------------------------------------------------------------------------------------
/**  Following are the headers of the constructor and methods contained in da_motor.cpp.
*	 Also included is aheader for a serial port object which allows da_motor's methods
*    to print to i/o devices.
*/
 

class utility
{
	protected:
		/// The ADC class needs a pointer to the serial port used to output to the terminal.  
		base_text_serial* ptr_to_serial;

	public:
		/** The constructor initializes an ADC object.
		*/
		utility(base_text_serial*);
		
		void initialize();
		
		void delayMs(uint16_t);
		
		void delayUs(uint16_t);
		
		uint8_t getButton1();
		
		void buttonWait();
		
		void ledOn();
		
		void ledOff();
		
		void ledToggle();
		
		void digitalDirection(uint8_t, const DigitalDirection);
		
		void digitalDirections(const uint16_t);
		
		void digitalPullups(const uint16_t);
		
		uint8_t digitalInput(const uint8_t);
		
		void digitalOutput(const uint8_t, const uint8_t);
		
		void digitalOutputs(const uint16_t);
		
		uint16_t digitalInputs();
		
		void digitalOutputToggle(const uint8_t);
		
		void softReset();
		
		/*! Since the watchdog timer remains enabled after a resetting, this function is added
		to a section of code that runs very early during startup to disable the watchdog.
		It should not be called from user code.
		
		void wdt_init() __attribute__((naked)) __attribute__((section(".init3")));
		void wdt_init()
		{
		MCUSR = 0;
		wdt_disable();
		return;
		}*/
		
		
};
	//-------------------------------------------------------------------------------------

#endif // _utility_H_
