//======================================================================================
	/** \file task_PID.cpp is a Proportional Integral Differential (PID) controller. It can
	* 	control two motors currently, but is best used to control one motor per PID object.
	
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
	
	// User written headers included with " "
	#include "rs232int.h"						//!< Include header for serial port class
	#include "stl_timer.h"						//!< allows task_PID to be scheduled
	#include "stl_task.h"						//!< allows task_PID to be scheduled
	#include "Master.h"							//!< allows SPI communications to operate
	#include "da_motor.h"						//!< allows motor control to occur
	#include "task_PID.h"						//!< include own header file
	
	// Define tuning values here for ease fo adjustment, names explain all.
	#define K_P_DIVISOR 10000 					//!< Just like it sounds			
	#define K_I_DIVISOR 1000000  				//!< Just like it sounds
	#define K_D_DIVISOR 100 					//!< Just like it sounds
	#define INTEGRAL_SATURATE 1000				//!< Saturate integral error
	#define DUTY_CYCLE_SATURATE 255				//!< Saturate duty cycle
//-----------------------------------------------------------------------------------------
/** The constructor task_PID creates a new Proportional Integral Differential (PID) controller object.
		*	@param p_serial_port	Allows screen printouts
		*	@param a_timer:			Assists in sceduling
		*	@param t_stamp:			Assists in sceduling
		*	@param master_object	An SPI master object
		*	@param motor_object:	A motor object
		*	@param Which_Channel: 	Gives the newborn PID its motor assignment. For life.
		*/

task_PID::task_PID (base_text_serial* p_serial_port, task_timer& a_timer, time_stamp& t_stamp, Master* master_object, 
					da_motor* motor_object, uint8_t Which_Channel)  :   stl_task (a_timer, t_stamp)
{
	
	ptr_2_serial = p_serial_port;				// pointer to a serial port
	func_READ_ENCODER = master_object;			// pointer to encoder object (used to call encoder methods)
	func_UPDATE_MOTOR = motor_object;			// pointer to motor object (used to call motor methods)
	
	
	motor_num = Which_Channel;					// When PID object is created, this constant is set, this tells PID object which 
												// motor & encoder to control.
	K_p = 0;									// initialize K_p
	K_i = 0;									// initialize K_i
	K_d = 0;									// initialize K_d
	Set_Point = 0;								// initialize set point
	E_sum_old = 0;								// initialize error sum
	prev_error = 0;								// initialize error archive
	duty_cycle = 0;								// initialize duty cycle
	encoder = 0;								// initialize encoder count variable
	giddyup = false; 							// initialize giddyup, this makes the motors start stopped
	are_we_there_yet = false;
	homing = false;
	
	// Say hello
	*ptr_2_serial << "Hi. I'm PID #"<< motor_num << endl;
		
	
}
/** run is a two state PID controller. The first state is simply a non moving state which checks to see if 
*	movement has been requested. State two incorporates proportional, integral and differential feedback 
*	into a control loop which updates the duty cycle of a motor (one motor per PID object).
*/
char task_PID::run(char state)
{
	switch (state)
	{
		/** State 0 is a non moving state. It doesn't move.
		*/
		case 0: 
			// if homing underway, leave and do nothing
			if (homing)
			{
				return (STL_NO_TRANSITION);	
			}
			
			func_UPDATE_MOTOR->update_duty_cycle(motor_num, 0);
			if (giddyup == true)
			{
				return (1); 							//transition to state 1 to do PID
			}
			else
				return (STL_NO_TRANSITION);				// continue to be off
		break;
		/** State 1 is the PID task. It gets a current encoder reading (ensuring no transfer error) and then 
		*	calculates new duty and sets the duty cycle.
		*/
		case 1: 
			bool Checksum_Error_flag = 1;				// mock an encoder checksum fault
			while (Checksum_Error_flag)					// get encoder readings until no checksum fault
			{
				func_READ_ENCODER -> Initiate(motor_num);		
				Checksum_Error_flag = func_READ_ENCODER -> Get_Checksum_flag();
			}
			encoder = func_READ_ENCODER -> Get_Encoder();
			
			// calculate error and saturate
			int32_t error_now = (Set_Point - encoder);
			
			int32_t proportional_error = (int32_t)(((int64_t)error_now * (int64_t)K_p)/K_P_DIVISOR);
			
			int32_t E_sum_now = error_now + E_sum_old;

			// ensure integral portion of feedback loop is within reason
			if (E_sum_now > INTEGRAL_SATURATE)
				E_sum_now = INTEGRAL_SATURATE;
			else if (E_sum_now < (-INTEGRAL_SATURATE))
				E_sum_now = (-INTEGRAL_SATURATE);
						
			int32_t integral_error = (int32_t)(((int64_t)E_sum_now * (int64_t)K_i)/K_I_DIVISOR);
			
			
			//int32_t differential_error = ((error_now - prev_error) * (K_d))/K_D_DIVISOR;
			
			//archive data
			E_sum_old = E_sum_now;
			prev_error = error_now;
			
			int32_t OUTPUT = (proportional_error + integral_error);// + differential_error);
			
			// If error_now yields a negative value, set direction as reverse
			if (OUTPUT < 0)
			{
				func_UPDATE_MOTOR->set_mode(motor_num, 2);	// make motor go CCW
			}
			// If error_now is positive, make motor go forward 
			else 
				func_UPDATE_MOTOR->set_mode(motor_num, 1);
			
			// Ensure OUTPUT is positive before updating the duty cycle
			if (OUTPUT < 0)
			{
				OUTPUT *= (-1);
			}
			else 
			{}
			
			// Saturate duty cycle
			if (OUTPUT > DUTY_CYCLE_SATURATE)
			{
				OUTPUT = DUTY_CYCLE_SATURATE;
			}
			duty_cycle = (uint8_t) OUTPUT;
			
			// update speed of motor
			func_UPDATE_MOTOR->update_duty_cycle(motor_num, duty_cycle);
		
			// If somebody pressed the stop button, transition to the stopped state
			if (giddyup == false)
			{
				return (0);
			}
			// transition to new set point if appropriate
			if ( (error_now <= 400) && (error_now >= -400) )
			{
				dummy++;
				if (dummy >= 10L)
				{
					are_we_there_yet = true;
					dummy = 0;
				}
			}
			return (STL_NO_TRANSITION);
	}
}

/** go enables motor control
*/
void task_PID::go(void)
{
	giddyup = true;
	are_we_there_yet = false;
}

/** stop disables motor control (stops the motors)
*/
void task_PID::stop(void)
{
	giddyup = false;
	are_we_there_yet = true;
}
/** CLEAR resets PID control variables
*/
void task_PID::CLEAR(void)
{
	E_sum_old = 0;
	prev_error = 0;
	encoder = 0;
	duty_cycle = 0;
}