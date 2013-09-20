//======================================================================================
/** \file  Go_Home.cpp is a task which finds home (R=0, THETA=0) for our plotter 
 *
 *  License:
 *    This file released under the Lesser GNU Public License. The program is intended
 *    for educational use only, but its use is not restricted thereto. 
 */
//======================================================================================

// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _Go_Home_H_
#define _Go_Home_H_


//-------------------------------------------------------------------------------------
/**  Go_Home.cpp is a class with methods to send the carriage and arm to their home positions
*	 and reset both motor encoders. Two bump-stop switches connected to pin interrupts tell
*	 the microcontroller to stop the motors when the carriage and arm reach their respective
*	 home positions.
*/
 

class Go_Home : public stl_task
{
	protected:
		  
		base_text_serial* ptr_2_serial;
		
		da_motor* Vtec_wins;
		
		Master* Wipe_Slave;
		
		task_PID* Wipe_Master1;
		
		task_PID* Wipe_Master2;
		
		task_lines* LINES;
		
	public:

		Go_Home(base_text_serial*, task_timer&, time_stamp&, da_motor*, Master*, task_PID*, task_PID*, task_lines*);
		
		char run(char state);
		
		void SET_Home_Request(void); 
		
		bool Is_Home(void);
  };


#endif // _Go_Home_H_
