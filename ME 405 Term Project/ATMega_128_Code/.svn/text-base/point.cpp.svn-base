	/** \file point.cpp is a task which moves a R-THETA plotter to a specific location.
	*	it can then make a dot or not as specified.
	
	*  Revisions:
	*    \li  05-10-11  Began tearing and hacking at our lab_4 code.				
	*	 \li  05-22-11	That was the hardest 12 days of our lives. It aint perfect, but it works
	*  License:
	*    This file released under the Lesser GNU Public License. The program is intended
	*    for educational use only, but its use is not restricted thereto. 
	*/
	//======================================================================================

	// System headers included with < >
	#include <stdlib.h>							//!< Standard C library
	#include <avr/io.h>							//!< Input-output ports, special registers
	#include <avr/interrupt.h>					//!< Interrupt handling functions
	#include <math.h>							//!< Contains Math Functions
	
	// User written headers included with " "
	#include "rs232int.h"						//!< Include header for serial port class
	#include "stl_timer.h"						//!< allows task_PID to be scheduled
	#include "stl_task.h"						//!< allows task_PID to be scheduled
	#include "Master.h"							//!< so da_motor doesn't get mad
	#include "da_motor.h"						//!< so PID objects don't get mad
	#include "task_PID.h"						//!< allows task_lines to call methods belonging to task_PID
	#include "servo.h"							//!< allows pen actuation
	#include "point.h"							//!< include own header file
	
	
	
	
	
	
	
//-----------------------------------------------------------------------------------------
/** The constructor saves object pointers locally and initializes necessary variables.
*	@param	p_serial_port:	A pointer to the serial port for printing purposes
*	@param	motor_1:	A pointer to a motor object
*	@param	motor_2:	A pointer to a second motor object
*	@param	Penny_Thingy: A servo object used to move a pen
*/
point::point(base_text_serial* p_serial_port, task_PID* motor_1, task_PID* motor_2, servo* Penny_Thingy) 
{
	// save object pointers locally
	ptr_2_serial = p_serial_port;
	PID_1 = motor_1;
	PID_2 = motor_2;
	Pen_Servo = Penny_Thingy;
	
	// initialize variables
	point_state = 0;
	Make_Point = false;
	Go_To = false;
	
	// Say hello
	*ptr_2_serial << "Welcome to the point of no return" << endl;
}

/** The run method moves the pen to a specific location. It has two modes. It can just move the pen somewhere
*	or it can move the pen somewhere and then make a dot.
*/
void point::run()
{
	switch (point_state)
	{
		/// wait state. Is a point or move requested?
		case 0:
			if (Make_Point || Go_To)
			{
				x_squared = pow((float)X, 2L);
				y_squared = pow((float)Y, 2L);
				encoder_goal_r  = (int32_t)sqrt(x_squared + y_squared);
				encoder_goal_r = encoder_goal_r*2350; //took this step out of the sqrt() function to minimize chance for math errors
				
				// calculate theta position, including oddballs THETA = 0 or 90 degrees
				if ((X == 0) && Y)
				{
					encoder_goal_theta = 925000;   
				}
				else if (Y == 0)
				{
					encoder_goal_theta = 0;
				}
				else
				{
					encoder_goal_theta = (int32_t)( atan(((float)Y)/((float)X)) * 588873);
				}
				
				// set setpoints and engage motors
				PID_1->set_setpoint(encoder_goal_r);
				PID_2->set_setpoint(encoder_goal_theta);
				PID_1->go();
				PID_2->go();
				// state transition
				point_state = 1;
			}
		break;
		
		/// state 1 checks if correct location is achieved yet, if so, make dot if necessary
		case 1:	
			// if both motors are done moving ...
			if ( (PID_1->At_Seg_End())   &&  	(PID_2->At_Seg_End()) )
			{
				// if in point mode lower pen, start counting from 0
				if (Make_Point)
				{
					counting = 0;
					Pen_Servo->Fine_Line();
					point_state = 2;
				}
				
				// must be in go-to mode, no further action necessary
				else
				{
					Go_To = false;
					point_state = 0;
					PID_1->stop();
					PID_2->stop();
				}
			}
				
		break;
		
		/// in the case of wanting to make a point, case 2 allows time for pen to get down
		case 2:
			if (counting >= 5000UL)
			{
				Pen_Servo->Pen_Up();
				Make_Point = false;
				counting = 0;
				point_state = 0;
			}
			counting++;
		break;
	}
}

/** This method initiates a cairrage go to. This is how lines moves the pen around.
*	@param Xcoord: An x coordinate to go to
*	@param Ycoord: An y coordinate to go to
*/
void point::Get_There(uint16_t Xcoord, uint16_t Ycoord)
{
	Go_To = true;
	X = Xcoord;
	Y = Ycoord;
}

/// this method allows someone to check if we're there yet. 
bool point::Are_We_There(void)
{
	// If Go_To is false after a go-to command was requested, we're there.
	return (!Go_To);
}

/** This method initiates a go to and then makes a point there.
*	@param Xcoord: An x coordinate to go to
*	@param Ycoord: An y coordinate to go to
*/
void point::Go_Make_A_Dot(uint16_t Xcoord, uint16_t Ycoord)
{
	Make_Point = true;
	X = Xcoord;
	Y = Ycoord;
}