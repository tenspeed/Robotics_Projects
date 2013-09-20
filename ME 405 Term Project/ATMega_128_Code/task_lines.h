//======================================================================================
/** \file  task_lines.h
 *
 *    \li  05-10-11  Began tearing and hacking at our lab_5 code.
 *	  \li  05-22-11	 That was the hardest 12 days of our lives. It aint perfect, but it works
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _task_lines_H_
#define _task_lines_H_


//-------------------------------------------------------------------------------------

 /**task_lines.cpp is a task which cuts lines into bitty pieces and moves sequentially 
 *	through the line by moving the setpoint and feeding it to the point task.
 */
class task_lines : public stl_task
{
	protected:
		
		// object pointers
		base_text_serial* ptr_2_serial;				//!< pointer to a serial port object for printing purposes
		point* Initial_Point;						//!< point object used to move machine
		servo* operate_pen;							//!< servo object used to raise lower pen
		task_PID* PID_1;							//!< PID object used to control motor 1
		task_PID* PID_2;							//!< PID object used to control motor 2

		// variables
		bool send_home;								//!< boolean used to request homing
		bool divide_lines;							//!< boolean which allows lines to begin
		bool moving;								//!< boolean used to wait while movement occurs
		bool signature;								//!< boolean used to initiate and continue signiture process
		uint8_t sig_count;							//!< sub state variable keeps track of where in signiture we are
		uint8_t line_type;							//!< sub state variable used to transition between line types
		int8_t dx;									//!< incremental x distance in tenths of an inch
		int8_t dy;									//!< incremental y distance in tenths of an inch
		uint16_t current_segment;					//!< keeps track of when a line is done
		uint16_t Seg_Total;							//!< total number of segments in a line
		uint16_t counter;							//!< dumb counter used to let pen get down befor next step happens
		int32_t SUMdx;								//!< total change in x
		int32_t SUMdy;								//!< total change in y
		int16_t x0;									//!< initial x coordinate of a line
		int16_t y0; 								//!< initial y coordinate of a line
		int16_t xf;									//!< final x coordinate of a line
		int16_t yf;									//!< final y coordinate of a line
		float slope;								//!< to detect lines we can do really well! (through origin)
		float SUMx_squared;							//!< partitioning pythagorean theorum helped math errors stop
		float SUMy_squared;							//!< partitioning pythagorean theorum helped math errors stop
		uint16_t SUMdx_16;							//!< extra variable to help avoid math errors
		uint16_t SUMdy_16;							//!< extra variable to help avoid math errors

		
	public:

		task_lines (base_text_serial* p_serial_port, task_timer& a_timer, time_stamp& t_stamp, task_PID* PID_1, task_PID* PID_2, 
					point* get_there, servo* PEN);
		
		char run(char);
		
		/// This method sets a bool allowing task_lines to operate.
		void go(void){divide_lines = true;}
		
		/// Set_coords updates the initial and final x&y coordinates when a new line needs to be drawn.
		void set_coords(int16_t X0, int16_t Y0, int16_t X_f, int16_t Y_f){x0=X0; y0=Y0; xf=X_f; yf=Y_f;}
		
		/// This method sets up lines to draw our signature.
		void draw_signature(void) {divide_lines = true; signature = true;}
		
		/// This method allows Go_Home to see if we want to home (after each line).
		bool wanna_go_home(void){return(send_home);}
		
		/// This method allows Go_Home to lower the homing flag once home is reached.
		void ok_were_home(void) {send_home = false;}
};

	//-------------------------------------------------------------------------------------

#endif // _task_lines_H_