//Copyright (C) 2009-2010  Darron Baida and Patrick J. McCarty.
//Licensed under X11 License. See LICENSE.txt for details.

/*! \file ADC.cpp
    Implements support for the Analog to Digital Converter (ADC).
    Provides functions for reading the 8 single-ended analog inputs with both 8-bit and 10-bit resolution.
    Functions are implemented synchronously, so the code will block while waiting for the conversion to complete.
    With a prescaler of 128, each conversion takes:
    62.5 ns/cpucycle * 128 cpucycle/aclock * 13 aclock/conversion = 104 us/conversion.
 */

#include <stdlib.h>							//!< Include standard library header files
#include <avr/io.h>							//!< You'll need this for SFR and bit names

#include "rs232int.h"						//!< Include header for serial port class
#include "globals.h"						//!< Include header for global definitions
#include "ADC.h"						    //!< Include header for the A/D class

ADC::ADC (base_text_serial* p_serial_port)
{
	ptr_to_serial = p_serial_port;          // Store the serial port pointer locally
	
}

/*! Initialize ADC.
    Normally called only by the initialize() function in utility.cpp
 */
 void ADC::adcInit() //initializes the ADC converter
{
	//Use AVCC as the ADC voltage reference, with external capacitor at AREF pin. Left shift data.
	ADMUX = _BV(REFS0) | _BV(ADLAR);

	//Enable ADC and set prescaler to /128 to yield 125kHz ADC clock.
	ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1)| _BV(ADPS0);
}

/*! Returns an 8-bit resolution reading of the specified analog input.
    @param num The analog input to sample (0 to 7).
    @return The 8-bit reading or 0xBD if an invalid input number was passed.
 */
uint8_t ADC::analog(const uint8_t num)
{
	//validate num parameter and return early if num is out of range
	if (num > 7)
	{
		return 0xBD;
	}

	//clear lower 5 bits and set left shifting
	ADMUX = _BV(REFS0) | _BV(ADLAR);
	//select the analog input to read
	ADMUX |= num;

	//start conversion
	ADCSRA |= _BV(ADSC);
	//wait for conversion to complete
	loop_until_bit_is_clear(ADCSRA, ADSC);

	//return 8-bit result
	return ADCH;
}

/*! Returns a 10-bit resolution reading of the specified analog input.
    @param num The analog input to sample (0 to 7).
    @return The 10-bit reading or 0xBAD if an invalid input number was passed.
 */
uint16_t ADC::analog10(const uint8_t num)
{
	//validate num parameter and return early if num is out of range
	if (num > 7)
	{
		return 0x0BAD;
	}

	//clear lower 5 bits and set right shifting
	ADMUX = _BV(REFS0);
	//select the analog input to read
	ADMUX |= num;

	//start conversion
	ADCSRA |= _BV(ADSC);
	//wait for conversion to complete
	loop_until_bit_is_clear(ADCSRA, ADSC);

	//lower 8 bits of result must be read first
	const uint8_t temp = ADCL;
	//combine the high and low bits and return the result as a 16-bit number.
	return ((uint16_t)ADCH << 8) | temp;
}
