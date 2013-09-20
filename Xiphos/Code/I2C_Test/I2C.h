//======================================================================================
/** \file  I2C.h
 *	
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

/// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _I2C_H_
#define _I2C_H_


//-------------------------------------------------------------------------------------

 

class I2C
{
	protected:
		// variable list goes here
		

	public:
	
		inline void i2cInit();
		
		u08 i2cStart();
		
		inline void i2cStop();
		
		u08 i2cByteTransmit(const u08);
		
		bool i2cByteReceiveACK(u08 *const);
		
		bool i2cByteReceiveNACK(u08 *const);
		
		bool i2cSendByteToDevice(const u08, const u08);
		
		bool i2cSend2BytesToDevice(const u08, const u16);
		
		bool i2cReadByteFromDevice(const u08, u08 *const);
		
		bool i2cSendDataToRegisters(const u08, const u08, const u08, const u08 *const);
		
		bool i2cReadDataFromRegisters(const u08, const u08, const u08, u08 *const);
		
		bool i2cSendDataToRegisters2(const u08, const u16, const u08, const u08 *const);
		
		bool i2cReadDataFromRegisters2(const u08, const u16, const u08, u08 *const);
		
};
	//-------------------------------------------------------------------------------------

#endif // _I2C_H_