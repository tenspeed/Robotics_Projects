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

// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _Master_H_
#define _Master_H_


//-------------------------------------------------------------------------------------
/**  Master.cpp is a class with methods for orchestrating and controlling the flow of
*	 data transfers on the SPI bus between a master device and a slave device. The data
*	 being transfered are new encoder positions from the two motors so that the task_PID
*	 class can keep track of where the motors are. The encoder values used in the PID
*	 calculations are stored in the main program and are updated in Master via pointers
*	 so that other objects can use them as well.
*/
 

class Master
{
	protected:
		/// The encoder driver class needs a pointer to the serial port used to output to the terminal.  
		base_text_serial* ptr_to_serial;
		/// flag indicating transfer error on encoder 1
		bool error_flag;
		/// encoder value received from Slave_Driver				
		int32_t encoder;	
		/// encoder transfer error 1					
		uint8_t Checksum_Error;	
		/// check sum computed here in master for encoder 1		
		uint8_t Check_Sum_Master;
		/// check sum computed in slave for encoder 1		
		uint8_t Slave_Sum;	
		/// counter used to delay  so SPI works			
		uint8_t dummy;		
		/// integer to tell Master which encoder reading to get from Slave_Driver			
		uint8_t encoder_num;			
		/// encoder_data allows easy partitioning of encoder data
		union packet1					
		{
			/// whole is a 32 bit encoder count
			int32_t whole;	
			/// parts is an array unioned with whole			
			int8_t parts[4];			
		} encoder_data;
		

	public:

		Master(base_text_serial*);
			   
		void CLEAR (uint8_t);

		void Initiate (uint8_t);
		
		bool Get_Checksum_flag(void);

		uint8_t Get_Checksum_Error(void);

		int32_t Get_Encoder(void); 
		
};


#endif // _Master_H_