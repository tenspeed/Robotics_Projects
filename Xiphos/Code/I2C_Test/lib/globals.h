//Copyright (C) 2009-2010  Patrick J. McCarty.
//Licensed under X11 License. See LICENSE.txt for details.

/*! \file globals.h
    Global defines.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <avr/io.h>          //I/O definitions (port/pin names, register names, named register bits, etc.)
#include <avr/interrupt.h>   //interrupt support
#include "projectGlobals.h"  //user-configured project-specific global #defines. To work, this requires "-I ." in the avr-gcc arguments.

#if USE_LCD == 1
	#include "LCDSpecialChars.h" //defines friendly names for various LCD special character codes.
#endif

//! CPU speed (16MHz) for timer and delay loop calculations.
#define F_CPU 16000000UL


//! Specifies the valid values that can be passed to the digitalDirection() function.
typedef enum
{
	INPUT,
	OUTPUT,
	INPUT_PULLUP
} DigitalDirection;

//! Specifies the valid values that can be passed to the servoRange() function.
typedef enum
{
	SERVO_RANGE_STANDARD = 7,
	SERVO_RANGE_DEFAULT = 8,
	SERVO_RANGE_EXTENDED1 = 9,
	SERVO_RANGE_EXTENDED2 = 10,
	SERVO_RANGE_EXTENDED3 = 11,
	SERVO_RANGE_EXTENDED4 = 12,
} ServoRange;


//Bit manipulation macros
#define sbi(a, b) ((a) |= 1 << (b))       //!< Sets bit b in variable a.
#define cbi(a, b) ((a) &= ~(1 << (b)))    //!< Clears bit b in variable a.
#define tbi(a, b) ((a) ^= 1 << (b))       //!< Toggles bit b in variable a.
#define gbi(a, b) ((a) & (1 << (b)))      //!< Gets bit b in variable a (masks out everything else).
#define gbis(a, b) (gbi((a), (b)) >> (b)) //!< Gets bit b in variable a and shifts it to the LSB.

#endif //ifndef GLOBALS_H
