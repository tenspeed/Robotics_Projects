//======================================================================================
/** \file  Go_Home.cpp is a task which finds home (R=0, THETA=0) for our plotter
 
 *
 *  Revisions:
 *            \li  05-26-11  Began tearing and hacking at our lab_5 code.
 *	       
 *    
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

#include <stdlib.h>							// Include standard library header files
#include <avr/io.h>							// You'll need this for SFR and bit names
#include <avr/interrupt.h>					// Interrupt handling functions
#include <math.h>							// so task_lines doesn't get mad
#include "rs232int.h"						// Include header for serial port class
#include "stl_timer.h"						// allows task_PID to be scheduled
#include "stl_task.h"						// allows task_PID to be scheduled
#include "Master.h"							// allows data clearing on slave chip
#include "da_motor.h"						// Include header for the A/D class
#include "task_PID.h"						// allows clearing of data on master chip
#include "servo.h"							// allows pen actuation
#include "point.h"							// used to move to specific points
#include "task_lines.h"						// allows lines to request homing
#include "Go_Home.h"						// include own header file

/// externally settable bool triggers homing operation
bool Home_Request;
/// interrupt driven switch tells whether cart is at r=0
bool cart_switch = false;
/// interrupt driven switch tells whether arm is at Theta=0. (note: r must be zero also!)
bool arm_switch = false;
//-------------------------------------------------------------------------------------
/** This constructor enables the interrupts necessary for the bump stop switches on our
*	plotter to operate, initializes control booleans, and saves object pointers locally.
*	@param p_serial_port	Allows screen printouts
*	@param a_timer:			Assists in sceduling
*	@param t_stamp:			Assists in sceduling
*	@param Vortec:			A motor object
*	@param Clear_Slave		An SPI master object
*	@param Clear_Massah1 	PID object for motor 1
*	@param Clear_Massah2 	PID object for motor 2
*	@param liner			task lines object so it can call home
*/

Go_Home::Go_Home (base_text_serial* p_serial_port, task_timer& a_timer, time_stamp& t_stamp,
				  da_motor* Vortec, Master* Clear_Slave, task_PID* Clear_Massah1, task_PID* 
				  Clear_Massah2, task_lines* liner) : stl_task (a_timer, t_stamp)
{
	// save object pointers locally
	ptr_2_serial = p_serial_port;
	Vtec_wins = Vortec;
	Wipe_Slave = Clear_Slave;
	Wipe_Master1 = Clear_Massah1;
	Wipe_Master2 = Clear_Massah2;
	LINES = liner;
	
	//enable pins E4 and E5 as inputs with pull ups on. (mommy wow!)
	DDRE &= ~( (1<<PIN4)|(1<<PIN5) );
	PORTE |= (1<<PIN4)|(1<<PIN5);
	
	// Enable external interrupts on Pins E4 and E5 trigger on falling edge
	EIMSK |= (1<<PIN4)|(1<<PIN5);
	EICRB |= (1<<PIN1)|(1<<PIN3);
	
	// say hello
	*ptr_2_serial << "Bad dog. Go Home!"<<endl;
}
/** run is the main method in Go_Home and it performs the actual homing operation of the two degree
*	of freedom system.
*	@param state: STL_task is used for scheduling
*/
char Go_Home::run(char state)
{
	switch (state)
	{
		/// state 0 does nothing unless homing is requested by user key press or task_lines
		case 0:
			if ((Home_Request) || (LINES->wanna_go_home()))
			{
				// if home requested by lines, enable interrupts
				Home_Request = true;
				
				//if cart not already homed, engage cart motor towards home, disable cart's PID
				if (PORTE && PIN4)
				{
					// set motor direction towards r=0 and engage motor.
					Vtec_wins->set_mode (1, 2);
					Wipe_Master1->Request_Home(true);
					Vtec_wins->update_duty_cycle (1, 150);
					
				}	
				return(1);
			}
			else
				return(STL_NO_TRANSITION);
		break;
		
		/// state 1 initiates movement of the arm towards home if cart has reached r=0
		case 1:
			// when cart is home, shut off cart motor, engage arm motor towards home, and disable arm's PID
			if (cart_switch)
			{
				// Shut off cart motor
				Vtec_wins->update_duty_cycle (1, 0);
				
				// if arm not already homed, send it home.
				if (PORTE && PIN5)
				{
					// set motor direction towards theta=0 and engage motor.
					Vtec_wins->set_mode (2, 2);
					Wipe_Master2->Request_Home(true);
					Vtec_wins->update_duty_cycle (2, 255);	
				}
				return(2);
			}
			return(STL_NO_TRANSITION);
		break;
		
		/// state 2 concludes homing once arm reaches THETA=0.
		case 2:
			if (arm_switch)
			{
				// shut off cart motor
				Vtec_wins->update_duty_cycle (2, 0);
				
				//clear all homing bools, and encoder and error counts
				Wipe_Slave->CLEAR(1);
				Wipe_Slave->CLEAR(2);
				Wipe_Master1->CLEAR();
				Wipe_Master2->CLEAR();
				arm_switch = false;
				cart_switch = false;
				Home_Request = false;
				
				// Tell PIDs that they have control of the motors again 
				Wipe_Master1->Request_Home(false);
				Wipe_Master2->Request_Home(false);
				LINES->ok_were_home();
				return(0);
			}
			return(STL_NO_TRANSITION);
		break;	
	}	
}

/** Set home request allows homing to be initiated by other tasks
*/
void Go_Home::SET_Home_Request(void)
{
	Home_Request = true;
}

/** Is_home returns a flag which is true if machine is done homing.
*/
bool Go_Home::Is_Home(void)
{
	return(!Home_Request);
}
//------------------------------------------------------------------------------------
// INTERRUPT ROUTINES
/// Cart switch is attached to pin E4
ISR (INT4_vect)
{
	// alert Go_Home to shut off cart motor 
	if(Home_Request)
		cart_switch = true;
}

/// Arm switch is attached to pin E5
ISR (INT5_vect)
{
	// alert Go_Home to shut off arm motor 
	if(Home_Request)
		arm_switch = true;
}


