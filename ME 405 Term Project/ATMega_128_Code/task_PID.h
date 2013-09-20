//======================================================================================
/** \file  task_PID.h
 *	PID.h contains specifications necessary for the Proportional Integral Differential (PID)
 *	controller. Also file scope variables and simple in line methods are defined here.
 * 
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _task_PID_H_
#define _task_PID_H_
//-------------------------------------------------------------------------------------
 /** task_PID.cpp is a class for a PID controller. task_PID is able to read the current
 *	 encoder position on a motor, calculate the proportional, integral, and differential
 *	 gains, and update the motor's duty cycle based on the final gain value. It can also
 *	 manipulate certain flags via inline functions to trigger actions, such as telling a
 *	 motor to go or stop, in other objects.
 */
class task_PID : public stl_task
{
	protected:
		/// pointer to a serial port object for printing purposes
		base_text_serial* ptr_2_serial;
		/// pointer of type Master to an SPI object
		Master* func_READ_ENCODER;					
		/// pointer of type da_motor to a motor object
		da_motor* func_UPDATE_MOTOR;				
		/// tell motor to stop or go
		bool giddyup;			
		/// homing allows task Go_Home to take control of motors
		bool homing;
		/// a constant for each PID which controls which motor and encoder we use each time the PID is called
		uint8_t motor_num;							
		/// A flag which indicates an SPI transfer error				
		uint8_t Checksum_Error_flag;
		/// stupid counting variable
		volatile uint8_t dummy;								
		/// name pretty well says it
		uint8_t duty_cycle;							
		/// proportional gain
		uint16_t K_p;								
		/// integral gain
		uint16_t K_i;								
		/// differential gain
		uint16_t K_d;								
		/// used for differential feedback
		int32_t prev_error;							
		/// used for integral feedback
		int32_t E_sum_old;						
		/// encoder reading
		int32_t encoder;						
		/// the set point
		int32_t Set_Point;		
		/// boolean set to true when desired position is reached
		bool are_we_there_yet;
		
	public:
		/** The constructor task_PID creates a new PID object.
		*	@param p_serial_port	Allows screen printouts
		*	@param a_timer:			Assists in sceduling
		*	@param t_stamp:			Assists in sceduling
		*	@param master_object	An SPI master object
		*	@param motor_object:	A motor object
		*	@param Which_Channel: 	Gives the newborn PID its motor assignment. For life.
		*/
		task_PID (base_text_serial* p_serial_port, task_timer& a_timer, time_stamp& t_stamp, Master* master_object, 
				  da_motor* motor_object, uint8_t which_Channel);
		
		/** run is a 2 state PID controller. State 0 is an idle state which wits for the go bool giddyup to be true. State 1
		*	gets an encoder count, does necessary feedback calculations and sets motor duty cycle.
		*	@param state is a state variable controlled by STL_task
		*/
		char run(char);
		
		void go(void);
		
		void stop(void);
		
		void CLEAR(void);
		
		// These three in line methods allow a user to set gains from other tasks.
			
		/// set K_p @param kp_val: the value you wish to set the gain to
		void set_kp(uint16_t kp_val) { K_p = kp_val; }
		/// set K_i @param ki_val: the value you wish to set the gain to
		void set_ki(uint16_t ki_val) { K_i = ki_val; }
		/// set K_d @param kd_val: the value you wish to set the gain to
		void set_kd(uint16_t kd_val) { K_d = kd_val; }	
		
		// These three in line methods return K values so they can be displayed
		/// GET_Kp returns K_p 
		uint16_t GET_Kp(void) {return K_p;}
		/// GET_Ki returns K_i
		uint16_t GET_Ki(void) {return K_i;}
		/// GET_Kd returns K_d
		uint16_t GET_Kd(void) {return K_d;}
		
		/** set_setpoint sets the setpoint
		*	@param s_pt the set point you desire
		*/
		void set_setpoint(int32_t s_pt) {Set_Point = s_pt; are_we_there_yet=false;}
		
		/** GET_setpoint gets the current setpoint so it can be printed
		*	@param Set_Point the set point you desire
		*/
		int32_t GET_setpoint(void) {return(Set_Point);}
		
		/** Get_Encoder gets an encoder reading
		*/
		int32_t Get_Encoder(void){ return(encoder);	}
		
		/** check if we're at the end of the segment
		*/
		bool At_Seg_End(void) { return(are_we_there_yet);}
		
		/** disable PID so homing can occur
		*	@param Nice_Shoes set this true to disable PID for homing
		*/
		void Request_Home(bool Nice_Shoes) {homing = Nice_Shoes;}
};
	//-------------------------------------------------------------------------------------
#endif // _PID_H_