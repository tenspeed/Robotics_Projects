//======================================================================================
/** \file  Go_Home.cpp is a task which finds home (R=0, THETA=0) for our plotter 
 
 *
 *  Revisions:
 *    \li  05-26-11	 Included specification for the method "read_a_few" within avr_adc.cpp
t.
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

/// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _Go_Home_H_
#define _Go_Home_H_


//-------------------------------------------------------------------------------------
/**  Following are the headers of the constructor and methods contained in Go_Home.
*/
 

class Go_Home : public stl_task
{
	protected:
		/// The homing class needs a pointer to the serial port used to output to the terminal.  
		base_text_serial* ptr_2_serial;
		/// a motor controller object allows Go_Home to initiate movement
		da_motor* Vtec_wins;
		/// a Master object allows the slave chip to be cleared upon homing
		Master* Wipe_Slave;
		/// PID object 1 allows errors and encoder counts on Master to be cleared
		task_PID* Wipe_Master1;
		/// PID object 2 allows errors and encoder counts on Master to be cleared
		task_PID* Wipe_Master2;
		/// task_lines object allows communication so lines can initiate homing after a line is made
		task_lines* LINES;
		
	public:
		/** This constructor enables the interrupts necessary for the bump stop switches on our
		*	plotter to operate, initializes control booleans, and saves object pointers locally.
		*/
		Go_Home(base_text_serial*, task_timer&, time_stamp&, da_motor*, Master*, task_PID*, task_PID*, task_lines*);
		
		/** run is the main method in Go_Home and it performs the actual homing operation of the two degree
		*	of freedom system.
		*	@param state: STL_task is used for scheduling
		*/
		char run(char state);
		
		/** Set home request allows homing to be initiated by other tasks
		*/
		void SET_Home_Request(void); 
		
		/** Is_home returns a flag which is true if machine is done homing.
		*/
		bool Is_Home(void);
  };


#endif // _Go_Home_H_
