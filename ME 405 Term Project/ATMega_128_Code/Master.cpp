	//======================================================================================
	/** \file  Master.cpp
	*  Revisions:
	*            \li  04-26-11  Began tearing and hacking at our lab_4 code.
	*	         \li  04-29-11  Writing this code feels funny. I'm dubious about what I've written
	*    		 \li  05-05-11	Finalize code for lab5	
	*
	*  License:
	*    This file released under the Lesser GNU Public License. The program is intended
	*    for educational use only, but its use is not restricted thereto. 
	*/
	//======================================================================================

	#include <stdlib.h>							// Include standard library header files
	#include <avr/io.h>							// You'll need this for SFR and bit names
	#include "rs232int.h"						// Include header for serial port class
	#include "Master.h"							// Include header for SPI driver class
	
	#define WAIT 150							/// WAIT is used as a delay to allow proper SPI communication
	//-------------------------------------------------------------------------------------
	/** The constructor takes in a serial pointer and saves it locally and constructs a
	*	Master object. The object sets up SPI communications as a master.
	*	@param p_serial_port serial object to allow printing
	*/
	Master::Master (base_text_serial* p_serial_port)
	{
		ptr_to_serial = p_serial_port;          // Store the serial port pointer locally
		
		// say hello
		*ptr_to_serial << "The Master Cometh" << endl;
		
		
		DDRA |= (1<<PIN7); 						// configure PORTA PIN7 as Slave Select line
		PORTA |= (1<<PIN7);						//Drive SS high
		DDRB |= (1<<DDB2)|(1<<DDB1)|(1<<DDB0);	// set MOSI and SCK as outputs
		SPCR |= (1<< SPE)|(1<<MSTR);  			//SPI enable, Data order [LSB first], Master select
		SPCR &= ~(1<<CPOL);						//Drive SCK low when idle
		SPCR &= ~(1<<CPHA);						//read on leading edge of SCK
		SPCR |= (1<<SPR1);						//SPR1, SPR0 set the frequency of SCK (prescale by 64)
		SPCR &= ~(1<<SPR0);

		//DDRA  |= (1<<PIN6)|(1<<PIN5);         // configure PORTA PIN5 and PIN6 as outputs for LED debugging
		
		// reset encoder ticks on 164
		CLEAR (1);
		CLEAR (2);
	}
	
	/** Clear is a method to clear a pair of counters located on another chip. Associated values in this class are also cleared
	*	@param motor_num pass in the value of which motor's encoder you would like to reset. motor_num=1 
	*	corresponds to command 3 and motor_num=2  sends command 4.
	*/
	void Master::CLEAR (uint8_t motor_num)
	{
		uint8_t confirm = 0;
		PORTA &= ~(1<<PIN7);									// Activate SS
		if (motor_num == 1)
		{
			while (confirm != 0x03)
			{
				PORTA &= ~(1<<PIN7);							// Activate SS
				SPDR = 0x03;									// send reset command
				while(!(SPCR && (1<<SPIF)))						//wait for transfer
				{
				}
				while (++dummy < WAIT)		{}	dummy=0;		// hold it up a sec
				SPDR = 0xFE;
				while(!(SPCR && (1<<SPIF)))						//wait for transfer
				{
				}
				while (++dummy < WAIT)		{}	dummy=0;		// hold it up a sec
				confirm = SPDR;
				encoder = 0; 
				Checksum_Error = 0;
				*ptr_to_serial << "reset channel 1" << endl;
			}
		}
		else if (motor_num == 2)
		{
			while (confirm != 0x04)
			{
				PORTA &= ~(1<<PIN7);							// Activate SS
				SPDR = 0x04;									// send reset command
				while(!(SPCR && (1<<SPIF)))						//wait for transfer
				{
				}
				while (++dummy < WAIT)		{}	dummy=0;		// hold it up a sec
				SPDR = 0xFE;
				while(!(SPCR && (1<<SPIF)))						//wait for transfer
				{
				}
				while (++dummy < WAIT)		{}	dummy=0;		// hold it up a sec
				confirm = SPDR;
				encoder= 0; 
				Checksum_Error = 0;
				*ptr_to_serial << "reset channel 2" << endl;
			}
		}
	}
	
	/** Initiate communicates with another SPI device. Each encoder value is 32 bits or 4 bytes. The data
	*	transfer across the SPI bus happens one byte at a time. Each byte is stored in the union encoder_data.
	*	After the complete encoder value has been recieved, one more data transfer occurs to get the checksum
	*	from the slave device. The checksum for the received data is calculated and then compared to the checksum
	*	sent by the slave. If the checksums don't match, there has been a transfer error and the pointer to the
	*	encoder variable in PID_test is not updated. If the checksums match, then the pointer to the encoder
	*	variable gets updated with the new encoder position.
	*	@param encoder_select Initiate must be fed which channel (motor) you desire encoder ticks from
	*/
	void Master::Initiate (uint8_t encoder_select)
	{	
		while (++dummy < WAIT)		{}	dummy=0;		// hold it up a sec
		encoder_num = encoder_select;
		PORTA &= ~(1<<PIN7);							// Activate SS
		if (encoder_num == 1)							// send 01 command if encoder_num = 1
		{
			Check_Sum_Master = 0x01;					// put command into check sum 1
			SPDR = 0x01;								// send transfer request command 1
			while(!(SPCR && (1<<SPIF)))	{}				// wait for transfer
			while (++dummy < WAIT)		{}	dummy=0;	// hold it up a sec
		}
		else if (encoder_num == 2)						// send 02 command if encoder_num = 2
		{
			Check_Sum_Master = 0x02;					// put command into check sum 2
			SPDR = 0x02;								// send transfer request command 2
			while(!(SPCR && (1<<SPIF)))	{}				// wait for transfer
			while (++dummy < WAIT)		{}	dummy=0;	// hold it up a sec
		}
		
		while (++dummy < WAIT)		{}	dummy=0;		// hold it up a sec
		SPDR = 0xFF;									// send junk, get first byte
		while(!(SPCR && (1<<SPIF)))	{}					// wait for transfer
		while (++dummy < WAIT)		{}	dummy=0;		// hold it up a sec
		encoder_data.parts[0] = SPDR;
		Check_Sum_Master += encoder_data.parts[0];
		
		while (++dummy < WAIT)		{}	dummy=0;		// hold it up a sec
		SPDR = 0xFF;									// send junk, get second byte
		while(!(SPCR && (1<<SPIF)))	{}					// wait for transfer
		while (++dummy < WAIT)		{}	dummy=0;		// hold it up a sec
		encoder_data.parts[1] = SPDR;
		Check_Sum_Master += encoder_data.parts[1];
		
		while (++dummy < WAIT)		{}	dummy=0;		// hold it up a sec
		SPDR = 0xFF;									// send junk, get third byte
		while(!(SPCR && (1<<SPIF)))	{}					// wait for transfer
		while (++dummy < WAIT)		{}	dummy=0;		// hold it up a sec
		encoder_data.parts[2] = SPDR;
		Check_Sum_Master += encoder_data.parts[2];
		
		while (++dummy < WAIT)		{}	dummy=0;		// hold it up a sec
		SPDR = 0xFF;									// send junk, get fourth byte
		while(!(SPCR && (1<<SPIF)))	{}					// wait for transfer
		while (++dummy < WAIT)		{}	dummy=0;		// hold it up a sec
		encoder_data.parts[3] = SPDR;
		Check_Sum_Master += encoder_data.parts[3];
		
		while (++dummy < WAIT)		{}	dummy=0;		// hold it up a sec
		SPDR = 0xFF;									// send junk, get check sum
		while(!(SPCR && (1<<SPIF)))	{}					// wait for transfer
		while (++dummy < WAIT)		{}	dummy=0;		// hold it up a sec
		Slave_Sum = SPDR;									
		if (Check_Sum_Master == Slave_Sum)
		{
			error_flag = false;							// no error occurred
			encoder = encoder_data.whole;
		}
		else
		{
			error_flag = true; 							// transmission error occurred!
			Checksum_Error++;
		}
		PORTA |= (1<<PIN7);								// Drive SS high, disableing 
		Check_Sum_Master = 0;							// Reset Check_Sum_Master before exiting
		Slave_Sum = 0;									// Reset Slave_Sum before exiting
		encoder_data.whole = 0;							// Reset the union before exiting
	}	
	
	/** Get_Encoder gets an encoder reading
	*/
	int32_t Master::Get_Encoder(void)
	{
			return(encoder);
	}
	
	/** Get_Checksum_Error gets the number of encoder transfer errors experienced
	*/
	uint8_t Master::Get_Checksum_Error(void)
	{
			return(Checksum_Error);
	}
	
	/**	Get_Checksum_flag is a method which returns true if the specified channel has experienced SPI transfer error
	*	and false if the channel's transfer went well.
	*/
	bool Master::Get_Checksum_flag(void)
	{
		if (error_flag)
			return(true);
		else
			return(false);
	}
	//-------------------------------------------------------------------------------------

