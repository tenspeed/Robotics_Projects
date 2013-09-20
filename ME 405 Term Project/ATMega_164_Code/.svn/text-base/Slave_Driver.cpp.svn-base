//======================================================================================
/** \file  Slave_Driver.cpp
 *  Revisions:
 *            \li  04-26-11  Began tearing and hacking at our lab_4 code.
 *    
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

#include <stdlib.h>							//!< Include standard library header files
#include <avr/io.h>							//!< You'll need this for SFR and bit names
#include "rs232int.h"						//!< Include header for serial port class
#include "Slave_Driver.h"

bool busy_1 = 0;							//!< keep encoder 1 from updating during transmission
bool busy_2 = 0;							//!< keep encoder 2 from updating during transmission
uint8_t INTERRUPTING_COW = 0;				//!< state variable for transmission progress
uint8_t check_sum_1 = 0; 					//!< check sum for encoder 1
uint8_t check_sum_2 = 0; 					//!< check sum for encoder 2
int32_t* ptr_data_1;						//!< pointer to encoder 1
int32_t* ptr_data_2;						//!< pointer to encoder 2
uint8_t encoder_number = 0;					//!< helps determine where to save data
uint8_t command = 0;						//!< command is the first item sent by master.
/// Commands for slave are as follows:
/// Command 1: 		initiate transmission of encoder 1 data
/// Command 2: 		initiate transmission of encoder 2 data
/// Command 3: 		clear encoder data for encoder 1
/// Command 4: 		clear encoder data for encoder 2
/// Command 0xFF: 	continue transmission and checksum of previously started data transmission

union packet1								//!< union to assist in partitioning encoder 1 data
	{
		int32_t whole_1;					//!< whole_1 is a 32bit encoder count from motor 1
		int8_t parts_1[4];					//!< parts_1 is an array chars unioned with whole_1
	} encoder_buffer1;
union packet2								//!< union to assist in partitioning encoder 2 data
	{
		int32_t whole_2;					//!< whole_2 is a 32bit encoder count from motor 2
		int8_t parts_2[4];					//!< parts_2 is an array chars unioned with whole_2
	} encoder_buffer2;
//-------------------------------------------------------------------------------------
/** This constructor sets up a slave driver. SPI communication and interrupts are enabled.
*   @param p_serial_port A pointer to the serial port which writes debugging info. 
*	@param p_data_1 the address where the encoder counts for motor 1 live in main
*	@param p_data_2 the address where the encoder counts for motor 2 live in main
*/

Slave_Driver::Slave_Driver (base_text_serial* p_serial_port,int32_t* p_data_1, int32_t* p_data_2)
{
	ptr_to_serial = p_serial_port;          // Store the serial port pointer locally
	ptr_data_1 = p_data_1;
	ptr_data_2 = p_data_2;
	
	// Note that when the serial port pointer is used to send a message, it must be
	// dereferenced with a "*", meaning "what is pointed to by ptr_to_serial" 
	*ptr_to_serial << "Toby in the house" << endl;
	
	DDRB |= (1<<PIN6);							//MISO as output
	DDRB &= ~((1<< PIN5)|(1<<PIN7)|(1<<PIN4));	//MOSI as input
												//SCK as input
												//SS as input
	SPCR &= ~((1<<CPOL)|(1<<CPHA));				//Drive SCK low when idle
												//read on leading edge of SCK
	PORTB |= (1<<PIN4)|(1<<PIN5)|(1<<PIN7); 	//pull-up resistors
	SPCR |= (1<<SPE)|(1<<SPIE);					//enable SPI communication
	
}

/** Packet32 is a method which splits a 32 byte number into 4 packets stored in file scope 
*	variables parts 0, 1, 2, and 3. These variables are in union with 32 bit variable whole. 
*	One set of data exists for each encoder, and data is updated regularly, but is blocked from 
* 	updating when a transmission on the specified channel is underway.
*/
void Slave_Driver::Packet32()
{
	if (busy_1 == 0)
	{
		encoder_buffer1.whole_1 = *ptr_data_1;
	}
	if (busy_2 == 0)
	{
		encoder_buffer2.whole_2 = *ptr_data_2;
	}
}
	


//-------------------------------------------------------------------------------------

ISR(SPI_STC_vect)
{
	command = SPDR;
	switch (command)
	{
		/// Case 1 initiates data transfer from encoder 1
		case 0x01: //initiate packet sending, load first packet into SPDR
			INTERRUPTING_COW = 0; 												//resets state variable INTERRUPTING_COW
			busy_1 = 1; 														//Prevents Slave_Driver from over-writing packet 
			busy_2 = 0;
			encoder_number = command;											//buffers while encoder data is being transmitted.
			SPDR = encoder_buffer1.parts_1[INTERRUPTING_COW]; 					//Loads the first packet into SPDR
			check_sum_1 = command + encoder_buffer1.parts_1[INTERRUPTING_COW]; 	//stores the original command and the first
																				// packet of transmitted data into a checksum.
			INTERRUPTING_COW++; 												//state variable INTERRUPTING_COW --> 1
		break;
		
		/// Case 2 initiates data transfer from encoder 2
		case 0x02: //initiate packet sending, load first packet into SPDR
			INTERRUPTING_COW = 0; 												//resets state variable INTERRUPTING_COW
			busy_1 = 0;
			busy_2 = 1; 														//Prevents Slave_Driver from over-writing packet 
			encoder_number = command;											//buffers while encoder data is being transmitted.
			SPDR = encoder_buffer2.parts_2[INTERRUPTING_COW]; 					//Loads the first packet into SPDR
			check_sum_2 = command + encoder_buffer2.parts_2[INTERRUPTING_COW]; 	//stores the original command and the first
																				// packet of transmitted data into a checksum.
			INTERRUPTING_COW++; 												//state variable INTERRUPTING_COW --> 1
		break;
		
		/// Case 3 resets encoder 1
		case 0x03:  
			SPDR = 0x03;														//command is returned as confirmation
			*ptr_data_1 = 0;													// clear encoder 1 data
		break;
		
		/// Case 4 resets encoder 2
		case 0x04:
			SPDR = 0x04;														//command is returned as confirmation
			*ptr_data_2 = 0;													// clear encoder 2 data
		break;
		
		/// case 0xFF continues data transfer already in progress
		case 0xFF: 
			if(INTERRUPTING_COW <= 3)
			{
				if (encoder_number == 0x01)
				{
					SPDR = encoder_buffer1.parts_1[INTERRUPTING_COW]; 			//Load the 2nd through 4th packets into SPDR
					check_sum_1 += encoder_buffer1.parts_1[INTERRUPTING_COW];	//add each packet to the checksum
					INTERRUPTING_COW++;											//state variable INTERRUPTING_COW --> 2, 3, then 4
				}
				if (encoder_number == 0x02)
				{
					SPDR = encoder_buffer2.parts_2[INTERRUPTING_COW]; 			//Load the 2nd through 4th packets into SPDR
					check_sum_2 += encoder_buffer2.parts_2[INTERRUPTING_COW];	//add each packet to the checksum
					INTERRUPTING_COW++;											//state variable INTERRUPTING_COW --> 2, 3, then 4
				}
			}		
			else if(INTERRUPTING_COW == 4)
			{
				if (encoder_number == 0x01)
				{
					SPDR = check_sum_1; 										//Load the checksum into SPDR
					INTERRUPTING_COW ++; 										//state variable INTERRUPTING_COW --> 5
				}
				if (encoder_number == 0x02)
				{
					SPDR = check_sum_2; 										//Load the checksum into SPDR
					INTERRUPTING_COW ++; 										//state variable INTERRUPTING_COW --> 5
				}
			}
			else
			{
				if (encoder_number == 0x01)
				{
					busy_1 = 0;													//allow encoder to update
				}
				if (encoder_number == 0x02)
				{
					busy_2 = 0;													//allow encoder to update
				}
				encoder_number = 0;
			}
		break;
		
	}

}
