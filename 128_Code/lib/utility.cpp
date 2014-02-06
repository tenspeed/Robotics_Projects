//Copyright (C) 2009-2010  Patrick J. McCarty.
//Licensed under X11 License. See LICENSE.txt for details.

/*! @file
    Various utility functions for the Xiphos 1.0 Board.
 */
#include <stdlib.h>							//!< Include standard library header files
#include <avr/io.h>							//!< You'll need this for SFR and bit names

#include "rs232int.h"						//!< Include header for serial port class
#include "globals.h"
#include <util/delay.h>
#include <avr/wdt.h>
#include "utility.h"

/*
Key of pin operations:
DDR:           0=input, 1=output
input PORT:    0=disable pullup, 1=enable pullup
output PORT:   0=drive low, 1=drive high
write to PIN:  1=toggle value of PORT
read from PIN: value on the pin
*/

utility::utility (base_text_serial* p_serial_port)  // Constructor for the LCD class
{
	ptr_to_serial = p_serial_port;          // Store the serial port pointer locally
}

//! Initializes all enabled board features. Must be called in your program before using other library functions.
void utility::initialize()
{
	//configure BTN1 as an input
	cbi(DDRD, DDD4);
	//enable pullup for BTN1
	sbi(PORTD, PD4);

	//configure LED as an output
	sbi(DDRG, DDG2);

	//configure 74LS374 (D Flip-Flop) clock pin as an output
	sbi(DDRD, DDD5);

	//configure LCD/Servo bus on port C as an output
	DDRC = 0xFF;

	#if USE_LCD == 1
		//initialize LCD
		lcdInit();
	#endif

	#if USE_I2C == 1
		//configure I2C clock rate
		i2cInit();
	#endif

	#if USE_MOTOR0 == 1 || USE_MOTOR1 == 1
		//initialize enabled motors
		motorInit();
	#endif

	#if NUM_SERVOS > 0
		//initialize servos
		servoInit();
	#endif

	#if USE_ADC == 1
		//initialize ADC
		adcInit();
	#endif
}

//! Provides a busy wait loop for an approximate number of milliseconds.
void utility::delayMs(uint16_t num)
{
	for (; num > 0; num--)
	{
		_delay_loop_2(4000);
	}
}

//! Provides a busy wait loop for an approximate number of microseconds.
void utility::delayUs(uint16_t num)
{
	for (; num > 0; num--)
	{
		
		_delay_loop_2(4);
	}
}

/*! Reads the status of the BTN1 button.
    @return 0 when the button is not pressed and 1 when the button is pressed.
    @see Use buttonWait() if you want to wait for a button press and release.
 */
uint8_t utility::getButton1()
{
	return (gbi(PIND, PIND4) == 0);
}

/*! Waits for a complete BTN1 button press and release, with button debouncing.
    @see Use getButton1() for simply checking the button state without waiting or debouncing.
 */
void utility::buttonWait()
{
	//wait for button to be pushed down
	while (!getButton1());
	//delay 30 ms for button debouncing
	delayMs(30);
	//wait for button to be released, if it is still down
	while (getButton1());
	//delay 30 ms for button debouncing
	delayMs(30);
}

//! Turns the LED on
void utility::ledOn()
{
	sbi(PORTG, PG2);
}

//! Turns the LED off
void utility::ledOff()
{
	cbi(PORTG, PG2);
}

//! Toggles the LED on/off
void utility::ledToggle()
{
	tbi(PORTG, PG2);
}

/*! Sets the direction and pullup resistor option for a digital pin.
    @param num Selects the digital pin (0 to 9).
    @param direction Specifies the direction and pullup for the digital pin.
    Valid values specified by the ::DigitalDirection enumeration (::INPUT, ::INPUT_PULLUP, ::OUTPUT).
    @see Use digitalDirections() and digitalPullups() if you want to configure all 10 digital pins at once.
 */
void utility::digitalDirection(uint8_t num, const DigitalDirection direction)
{
	if (num > 1)
	{
		//adjust num offset here to simplify the code below
		num -= 2;
		switch (direction)
		{
		case INPUT:
			cbi(DDRA, num);  //input direction
			cbi(PORTA, num); //disable pullup
			break;
		case INPUT_PULLUP:
			cbi(DDRA, num);  //input direction
			sbi(PORTA, num); //enable pullup
			break;
		case OUTPUT:
			sbi(DDRA, num);  //output direction
			break;
		}
	}
	else if (num == 1)
	{
		switch (direction)
		{
		case INPUT:
			cbi(DDRB, DDB7);  //input direction
			cbi(PORTB, PB7); //disable pullup
			break;
		case INPUT_PULLUP:
			cbi(DDRB, DDB7);  //input direction
			sbi(PORTB, PB7); //enable pullup
			break;
		case OUTPUT:
			sbi(DDRB, DDB7);  //output direction
			break;
		}
	}
	else if (num == 0)
	{
		switch (direction)
		{
		case INPUT:
			cbi(DDRB, DDB4);  //input direction
			cbi(PORTB, PB4); //disable pullup
			break;
		case INPUT_PULLUP:
			cbi(DDRB, DDB4);  //input direction
			sbi(PORTB, PB4); //enable pullup
			break;
		case OUTPUT:
			sbi(DDRB, DDB4);  //output direction
			break;
		}
	}
}

/*! Sets the data direction for all 10 digital pins, using the lower 10 bits of the argument.
    @param directions Bit 0 (LSB) matches digital0 ... bit 9 matches digital9.
    A high (1) sets the pin as an output. A low (0) sets the pin as an input.
    @see Use digitalDirection() if you only want to configure a single digital pin.
 */
void utility::digitalDirections(const uint16_t directions)
{
	const uint8_t lower = (uint8_t)directions;
	//set digital0 as output
	if (gbi(lower, 0))
		sbi(DDRB, DDB4);
	//else set digital0 as input
	else
		cbi(DDRB, DDB4);

	//set digital1 as output
	if (gbi(lower, 1))
		sbi(DDRB, DDB7);
	//else set digital1 as input
	else
		cbi(DDRB, DDB7);

	//set directions for digital2-digital9
	DDRA = (uint8_t)(directions >> 2);
}

/*! Sets the pullup resistor options for all 10 digital pins, using the lower 10 bits of the argument.
    Pullup option will only be set for a pin if the pin is currently configured as an input.
    @param pullups Bit 0 (LSB) matches digital0 ... bit 9 matches digital9.
    A high (1) will enable the pullup. A low (0) will disable the pullup.
    @see Use digitalDirection() if you only want to configure a single digital pin.
*/
void utility::digitalPullups(const uint16_t pullups)
{
	//digital0
	if (gbi(DDRB, DDB4) == 0)
	{
		if (gbi(pullups, 0))
			sbi(PORTB, PB4); //enable pullup
		else
			cbi(PORTB, PB4); //disable pullup
	}
	//digital1
	if (gbi(DDRB, DDB7) == 0)
	{
		if (gbi(pullups, 1))
			sbi(PORTB, PB7); //enable pullup
		else
			cbi(PORTB, PB7); //disable pullup
	}
	//digital2-digital9
	uint8_t i;
	const uint8_t pullups2 = (uint8_t)(pullups >> 2);
	for (i = 0; i < 8; i++)
	{
		if (gbi(DDRA, i) == 0)
		{
			if (gbi(pullups2, i))
				sbi(PORTA, i); //enable pullup
			else
				cbi(PORTA, i); //disable pullup
		}
	}
}

/*! Reads the value of a digital input.
    @param num Selects the digital input (0 to 9).
    @return 1 if the input is high, 0 if the input is low.
    @see Use digitalInputs() if you want to read all 10 digital pins at once.
 */
uint8_t utility::digitalInput(const uint8_t num)
{
	if (num > 1)
		return gbis(PINA, num - 2);
	else if (num == 1)
		return gbis(PINB, PINB7);
	else if (num == 0)
		return gbis(PINB, PINB4);
}

/*! Sets the value of a digital output.
    This function does not validate that the specified pin is an output, so it may be setting the pullup instead.
    @param num Selects the digital output (0 to 9).
    @param value 0 turns the output off, anything else turns it on.
    @see Use digitalOutputs() if you want to set all 10 digital pins at once.
 */
void utility::digitalOutput(const uint8_t num, const uint8_t value)
{
	//if the output should be set
	if (value > 0)
	{
		if (num > 1)
			sbi(PORTA, num - 2);
		else if (num == 1)
			sbi(PORTB, PB7);
		else if (num == 0)
			sbi(PORTB, PB4);
	}
	//else the output should be cleared
	else
	{
		if (num > 1)
			cbi(PORTA, num - 2);
		else if (num == 1)
			cbi(PORTB, PB7);
		else if (num == 0)
			cbi(PORTB, PB4);
	}
}

/*! Sets the values of all 10 digital outputs, by reading the lower 10 bits of the argument.
    If setting a constant value, it is recommended to express the argument in hex notation.
    @param outputs Set bits to 1 to turn on corresponding outputs, or 0 to turn off corresponding outputs.
    @see Use digitalOutput() if you only want to set a single digital pin.
 */
void utility::digitalOutputs(const uint16_t outputs)
{
	PORTA = (uint8_t)(outputs>>2);
	PORTB = (PORTB & 0x6F) | ((gbis((uint8_t)outputs, 1) << PB7) | (gbi((uint8_t)outputs, 0) << PB4));
}

/*! Returns the 10 digital inputs packed into the lower 10 bits.
    This number will be easier to read if displayed in hex, for example by printHex_u16().
    @return 1 bits for corresponding high inputs, 0 bits for corresponding low inputs.
    Bit 0 (LSB) matches digital0 ... bit 9 matches digital9.
    @see Use digitalInput() if you only want to read a single digital pin.
 */
uint16_t utility::digitalInputs()
{
	return (uint16_t)((PINA<<2) | (gbis(PINB, PINB7)<<1) | gbis(PINB, PINB4));
}

/*! Toggles the value of a digital pin from 0 to 1 or vice versa.
    Only makes sense to use this with a pin that is already set as an output.
    This function does not validate that the specified pin is an output, so it may be toggling the pullup instead.
    @param num Selects the digital output (0 to 9).
 */
void utility::digitalOutputToggle(const uint8_t num)
{
	//write a 1 to PIN to toggle value of PORT
	if (num > 1)
		PINA = _BV(num - 2);
	else if (num == 1)
		PINB = _BV(PINB7);
	else if (num == 0)
		PINB = _BV(PINB4);
}

/*! Performs a software reset by setting the shortest watchdog timeout possible and infinite looping.
    This is quite similar to pushing the reset button. The chip will reset and run the bootloader again.
    Therefore if the USB cable is providing power, the bootloader will do a 10 second countdown.
	@bug This function is correct but is not working properly, probably an issue with the current bootloader or fuse settings.
 */
void utility::softReset()
{
	wdt_enable(WDTO_15MS);
	while (1)
	{
	}
}

