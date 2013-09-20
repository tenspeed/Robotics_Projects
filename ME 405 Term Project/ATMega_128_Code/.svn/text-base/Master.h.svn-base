//======================================================================================
/** \file  Master.h
 *   Master.h contains specifications necessary for the methods herein to be 
 *   called from within any main program as long as this header file is included.
 *
 *  Revisions:
 *    \li  04-26-11  Began tearing and hacking at our lab_4 code.
 *	  \li  05-05-11  Finalized for lab5
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

/// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _Master_H_
#define _Master_H_


//-------------------------------------------------------------------------------------
/**  Following are the headers of the constructor and methods contained in da_encoder.cpp.
*	 Also included is aheader for a serial port object which allows da_encoder's methods
*    to print to i/o devices.
*/
 

class Master
{
	protected:
		/// The encoder driver class needs a pointer to the serial port used to output to the terminal.  
		base_text_serial* ptr_to_serial;
		bool error_flag;				//!< flag indicating transfer error on encoder 1
		int32_t encoder;				//!< encoder value received from Slave_Driver		
		uint8_t Checksum_Error;			//!< encoder transfer error 1
		uint8_t Check_Sum_Master;		//!< check sum computed here in master for encoder 1
		uint8_t Slave_Sum;				//!< check sum computed in slave for encoder 1
		uint8_t dummy;					//!< counter used to delay  so SPI works
		uint8_t encoder_num;			//!< integer to tell Master which encoder reading to get from Slave_Driver
		
		union packet1					//!< encoder_data allows easy partitioning of encoder data
		{
			int32_t whole;				//!< whole is a 32 bit encoder count
			int8_t parts[4];			//!< parts is an array unioned with whole
		} encoder_data;
		

	public:
		/** The constructor takes in a serial pointer and saves it locally and constructs a
		*	Master object. The object sets up SPI communications as a master.
		*	@param p_serial_port serial object to allow printing
		*/
		Master(base_text_serial*);
			   
		/** Clear is a method to clear a pair of counters located on another chip.
		*	@param motor_num pass in the value of which motor's encoder you would like to reset. motor_num=1 
		*	corresponds to command 3 and motor_num=2  sends command 4.
		*/
		void CLEAR (uint8_t);
		
		/** Initiate communicates with another SPI device using a state machine format. State
		*	variable Initiate_State determines position in switch case structure where the state variable 
		*	here and the encoder channel is alternated via pointer ptr_encoder_select.
		*/
		void Initiate (uint8_t);
		
		/**	Get_Checksum_flag is a method which returns true if the specified channel has experienced SPI transfer error
		*	and false if the channel's transfer went well.
		*/
		bool Get_Checksum_flag(void);
		/**  Get_Checksum_Error is a method which returns the number of SPI transfer errors encountered
		*/
		uint8_t Get_Checksum_Error(void);
		/**	 Get_Encoder returns the current value of the encoder,
		*/
		int32_t Get_Encoder(void); 
		
};


#endif // _Master_H_