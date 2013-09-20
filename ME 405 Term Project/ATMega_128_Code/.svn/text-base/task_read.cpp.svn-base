/** task_read is the input section of the user interface for the ME405 term project. It takes in the 
*	following commands:
*						[h,H,?] 	print a help screen 
*						[o,O]		home the machine
*						[g,G]		enable motor control
*						[s,S]		disable motor control (stop motors)
*                		[SPACE]		Emergency stop
*						[r,R]		raise pen
*						[l,L]		lower pen
*						[p,P]		initiate K_p GAIN ENTRY MODE
*						[i,I]		initiate K_i GAIN ENTRY MODE
*						[d,D]		initiate K_d GAIN ENTRY MODE
*						[c,C]		initiate COORDINATE INPUT MODE to make a line
*						[m,M]		initiate COORDINATE INPUT MODE to make a point
*						[q,Q]		print current values of gains, encoders and set points
*						[z,Z]		reset PID controller (on both boards)
*						[x,X]		Make signiture
*	COORDINATE INPUT MODES: Allows a user to enter values from 0 to 99.9 and accept one decimal place if desired
*	GAIN ENTRY MODE: Allows a user to enter a number up to 5 digits. User is then promted on which motor to update.
*					 no enter key is required on this last step.
*/

#include <stdlib.h>							//!< Include standard library header files
#include <avr/io.h>							//!< You'll need this for SFR and bit names
#include "rs232int.h"						//!< Include header for serial port class
#include "stl_timer.h"						//!< So this can become a schedualed task when it grows up
#include "stl_task.h"						//!< So this can become a schedualed task when it grows up
#include "Master.h"							//!< header file for SPI master class
#include "da_motor.h"						//!< header file for motor driver class
#include "task_PID.h"						//!< header file for PID controller class
#include "servo.h"							//!< allows pen actuation
#include "point.h"							//!< point is used to make dots
#include "task_lines.h"						//!< header file for task_lines
#include "Go_Home.h"						//!< so user can home the plotter
#include "task_read.h"						//!< header file for this class

//-------------------------------------------------------------------------------------
/**	The constructor creates a task_read object which handles user inputs as described above. It takes
*	in pointers to necessary objects and variables, saves them locally and initializes necessary variables.
*	Of note, read_state and print_mode are initialized to 3 and 6 respectively so that the help menu will 
*	print out as soon as all constructors run.
*	@param	p_serial_port:	A pointer to the serial port for printing purposes
*	@param	motor_1:		A pointer to a PID object
*	@param	motor_2:		A pointer to a second PID object
*	@param	p_print_mode:	A pointer to print_mode, which is a logic/state variable for print_state [TEMPORARY]
*	@param	Master_Object:	A pointer to an SPI master object so resets can be performed
*	@param	LINES 			A task_lines pointer so line endpoints can be set
*	@param	Tom_Servo		A servo object so pen can be raised/lowered
*	@param	Home_Slice		A Go_Home object so homing can be initiated
*	@param	POINTY			A point object so make a point command can be initiated
*/

task_read::task_read(base_text_serial* p_serial_port, task_PID* motor_1, task_PID* motor_2, uint8_t* p_print_mode,
						Master* Master_Object, task_lines* LINES, servo* Tom_Servo,
						Go_Home* Home_Slice, point* POINTY)
{
	// save pointers locally
	ptr_2_serial = p_serial_port;			// serial object to print to screen here
	MASTER = Master_Object;					// so methods for SPI communication can be utilized
	PID_1 = motor_1;						// so methods for PID #1 can be accessed
	PID_2 = motor_2;						// so methods for PID #2 can be accessed
	MAKE_PATH = LINES;						// so methods for task_lines can be accessed
	ptr_2_print_mode = p_print_mode;		// print_mode (in main) tells task_print what to print to screen
	You_Just_Got_Servoed = Tom_Servo;
	Plotter_Home = Home_Slice;
	Make_Point = POINTY;
	
	
	
		
	// initialize variables
	read_state = 3;							// initialize read state to 3 (print help menu state)
	*ptr_2_print_mode = 6;					// print out help menu at the start
	read_mode = 0;							// read_mode tells input states where to save to
	
	
	
	// print a hello message
	*ptr_2_serial << "task_read is online" << endl;
	
}

/**	the run method handles all user inputs. In easy cases, actions are taken here. in longer cases, states are used
*	and flags indicate to another task, task_print, that a message should be printed to screen. 
*	States are as follows:
*							0) Hub state: get char if input, check for valid command, initiate action
*							1) Coordinate entry mode
*							2) Gain value entry mode
*							3) Help Menu (wait for it to print)
*							4) Reset                        
*							5) Wait for coordinate value prompt
*							6) Wait for gain value prompt
*							7) Wait for prompt asking which motor to apply gain to
*							8) Apply entered to gain to correct memory location
*/
void task_read::run(void)
{	
	
	// enter switch statement where state variable read_state is controller
	switch (read_state)
	{
		/** state 0 is a hub state
		*/
		case 0:
			// has a char been entered?
			if (ptr_2_serial->check_for_char ())
			{
				input_char = ptr_2_serial->getchar ();
				// enter "is it a valid command?" switch
				switch (input_char)
				{					
					case ' ':
						PID_1->stop();
						PID_2->stop();
						PID_1->Request_Home(true);
						PID_2->Request_Home(true);
						*ptr_2_serial << "Emergency stop Yo" << endl;
					break;
					
					// K_i entry command
					case 'i':
					case 'I':
						*ptr_2_print_mode = 3;								// print prompt
						read_state = 6;										// change state
						read_mode = 3;										// save to correct spot
						K = 0;
						index = 0;
					break;
					
					// K_d entry command
					case 'd':
					case 'D':
						*ptr_2_print_mode = 5;								// print prompt
						read_state = 6;										// change state
						read_mode = 5;										// save to correct spot
						K = 0;
						index = 0;
					break;
					
					// K_p entry command
					case 'p':
					case 'P':
						*ptr_2_print_mode = 4;								// print prompt
						read_state = 6;										// change state
						read_mode = 4;										// save to correct spot
						K = 0;
						index = 0;
					break;
					
					// raise pen command
					case 'r':
					case 'R':
						You_Just_Got_Servoed->Pen_Up();
					break;
					
					// lower pen command
					case 'l':
					case 'L':
						You_Just_Got_Servoed->Fine_Line();
						//You_Just_Got_Servoed->Set_Angle(80);
					break;
					
					// go command
					case 'g':
					case 'G':
						MAKE_PATH->go();
					break;
					
					// stop command
					case 's':
					case 'S':
						PID_1->stop();
						PID_2->stop();
					break;
					
					// reset command
					case 'z':
					case 'Z':
							read_state = 4;
					break;
					
					// help command
					case 'h':
					case 'H':
					case '?':
						*ptr_2_print_mode = 6;								// print help menu
						read_state = 3;												
					break;
					
					// Home Plotter
					case 'o':
					case 'O':
						// set PIDs to allow homing
						PID_1->Request_Home(true);
						PID_2->Request_Home(true);
						// initiate homing task
						Plotter_Home->SET_Home_Request();
					break;
					
					// de-bug printouts
					case 'q':
					case 'Q':
						*ptr_2_print_mode = 0xFF;
					break;
					
					//make a point
					case 'M':
					case 'm':
						coord_num = 2;
						read_state = 11;
						point_request = true;
						index = 1; 
						*ptr_2_print_mode = 10;							// print prompt
						coordinate = 0;										// re-initialize temp coordinate memory
					break;
					
					// input a line
					case 'C':
					case 'c':
						coord_num = 4;
						read_state = 11;
						coord_request = true;
						index = 1; 
						*ptr_2_print_mode = 12;							// print prompt
						coordinate = 0;										// re-initialize temp coordinate memory
					break;	
					
					case 'x':
					case 'X':
						read_state = 12;
						*ptr_2_print_mode = 14;
				}
			}
		break;
		
		/** state 1 is a coordinate entry mode. It expects a up to two digit integer with a tenths place 
		*	decimal [ie 23.1 or 3.2]. it is always stored as number of tenths [ie 23.4 --> 234 here]
		*/
		case 1:
			// has a key been pressed?
			if (ptr_2_serial->check_for_char ())
			{
				input_char = ptr_2_serial->getchar ();  // get input 
				
				if (input_char == '.') 
				{
					//raise decimal flag
					dec_flag = index;
					// echo decimal to screen
					*ptr_2_serial << ".";										//echo to screen
				}
				// is character an 'enter'?
				else if (input_char == 0x0D)
				{
					switch (index)
					{
						// CASE 1, no numbers entered, don't save anything
						case 1:
							read_mode = 0;
							*ptr_2_print_mode = 7;								// invalid input error
						break;
						
						// CASE 2, one digit entered
						case 2:
							// .# or #. ?
							if (dec_flag == 0)
							{
								coordinate = (coordinate*10);
							}
						break;
						
						// CASE 3, two digits entered
						case 3:
							// ## or #.# ?
							if (dec_flag == 0 )
							{
								coordinate = (coordinate*10);
							}
						break;
						
						// CASE 4, 3 digits entered, ensure decimal pressed
						case 4:
							// if 3 digits and no decimal, error
							if (dec_flag == 0)
							{
								read_mode = 0;
								*ptr_2_print_mode = 7;							// invalid input error
							}
						break;
					}
						
						*ptr_2_serial << endl;
						dec_flag = 0;
						switch (coord_num)
					{
						// save last value and transition to proper mode
						case 1:
							Y_f = coordinate;
							if (point_request)
							{
								read_state = 9;
							}
		
							else if (coord_request)
							{
								read_state = 10;
							}
						break;
		
							
						break;
						
						case 2:
							X_f = coordinate;
							coord_num--;
							read_state = 11;
							*ptr_2_print_mode = 11;							// print prompt
							index = 1;
							coordinate = 0;										// re-initialize temp coordinate memory
						break;
						
						case 3:
							Y0 = coordinate;
							coord_num--;
							read_state = 11;
							*ptr_2_print_mode = 10;							// print prompt
							index = 1;
							coordinate = 0;										// re-initialize temp coordinate memory
						break;
						
						case 4:
							X0 = coordinate;
							coord_num--;
							read_state = 11;
							*ptr_2_print_mode = 13;							// print prompt
							index = 1;
							coordinate = 0;										// re-initialize temp coordinate memory
						break;
					}
					
					
				}
				
				// is (0 < input_char < 9) ?  [ie something we dont care about]
				else if ((input_char > '9') || (input_char < '0'))
				{
					// perhaps set an error here, for now- ignore
				}
				
				// input_char must be a number
				else
				{
					// only take up to three digits
					if (index < 4)
					{
						coordinate = (coordinate*10)+(input_char - '0');
						index++;
						*ptr_2_serial << ascii << input_char;								//echo to screen
					}
				}
			}
		break;
		
		/**	state 2 is an entry mode for gains. It expects an integer value up to 5 digits. Values are saturated to 
		*	16 bit maximum if entry is too large .
		*/
		case 2:
			// has a key been pressed?
			if (ptr_2_serial->check_for_char ())
			{
				input_char = ptr_2_serial->getchar ();  // get input 
				
				// is character an 'enter'?
				if (input_char == 0x0D)
				{
					// Saturate K to 16 bit if too big
					if (K > 0xFFFF)
					{
						K = 0xFFFF;
					}
					// new line
					*ptr_2_serial << endl;
					read_state = 7;
					*ptr_2_print_mode = 8;				// ask which motor
					
				}
				// is (0 > input_char > 9) ? [ie something we dont care about] 
				else if ((input_char > '9') || (input_char < '0'))
				{
					// perhaps set an error here, for now- ignore
				}
				// input_char must be a number
				else
				{
					// only take up to five digits
					if (index <= 5)
					{
						K = (K*10)+(input_char - '0');
						index++;
						*ptr_2_serial << ascii<< input_char;								//echo to screen
					}
				}	
			}
		break;
	
		/**	State 3 prints the help menu. Since that takes a bit the task waits until print out is done
		*/
		case 3:
			if (*ptr_2_print_mode == 0)
			{
				read_state = 0;
			}
		break;
		
		/** State 4 is a user defined reset. Necessary motor control values are cleared on both microcontrollers
		*/
		case 4:
			// reset encoder ticks, transfer errors on 164
			MASTER -> CLEAR(1);
			MASTER -> CLEAR(2);
			
			// reset encoder value, duty cycle and gain calculation archive values
			PID_1-> CLEAR();
			PID_2-> CLEAR();
			
			// return the state back to the hub
			read_state = 0;
		break;
		
		/** State 5 allows task_print enough time to get a prompt up before coordinate entry task is entered
		*/
		case 5:
			if (*ptr_2_print_mode == 0)
			{
				read_state = 1;
			}
		break;
		
		/** State 6 allows task_print enough time to get a prompt up before gain entry task is entered
		*/
		case 6:
			if (*ptr_2_print_mode == 0)
			{
				read_state = 2;
			}
		break;
		
		/** State 7 allows task_print enough time to ask which motor needs its gains adjusted before data is saved
		*/
		case 7:
			if (*ptr_2_print_mode == 0)
			{
				read_state = 8;
			}
		break;
		
		/** State 8 saves gain information to the proper K location
		*/
		case 8:
			// has a key been pressed?
			if (ptr_2_serial->check_for_char ())
			{
				input_char = ptr_2_serial->getchar ();  // get input 
				switch (input_char)
				{
					case '1':
						*ptr_2_serial << ascii<< "1"<<endl;
						// save to correct location (or leave memory unchanged)
						switch (read_mode)
						{
							// CASE 0 means don't overwrite any data
							// CASE 3 means write to K_i
							case 3:
								PID_1 -> set_ki((uint16_t) K);
							break;
							// CASE 4 means write to K_p
							case 4:
								PID_1 -> set_kp((uint16_t) K);
							break;
							// CASE 5 means write to K_d
							case 5:
								PID_1 -> set_kd((uint16_t) K);
							break;
						}
					break;
					
					case '2':
						*ptr_2_serial << ascii<< "2"<<endl;
						// save to correct location (or leave memory unchanged)
						switch (read_mode)
						{
							// CASE 0 means don't overwrite any data
							// CASE 3 means write to K_i
							case 3:
								PID_2 -> set_ki((uint16_t) K);
							break;
							// CASE 4 means write to K_p
							case 4:
								PID_2 -> set_kp((uint16_t) K);
							break;
							// CASE 5 means write to K_d
							case 5:
								PID_2 -> set_kd((uint16_t) K);
							break;
						}
					break;
				}
			read_mode = 0;
			read_state = 0;
			}
			
		break;
		
		/// state 9 is the beginning of a point entry command
		case 9:
			read_state = 0;
//*ptr_2_serial << X_f << "  " << Y_f <<endl; //FOR DEBUGGING
			Make_Point->Go_Make_A_Dot( X_f, Y_f);
			point_request = false;
	
		break;
		
		case 10:
			read_state = 0;
//*ptr_2_serial << "Read  " << X0 <<"  "<<Y0 <<"  "<< X_f << "  " << Y_f <<endl; //FOR DEBUGGING
			MAKE_PATH->set_coords(X0, Y0, X_f, Y_f);
			coord_request = false;
		break;

		/// state 11 handles prompt printing wait times for coord entry
		case 11:
			if (*ptr_2_print_mode == 0)
			{
				read_state = 1;
			}
		break;
		
		case 12:
			if (*ptr_2_print_mode == 0)
			{
				MAKE_PATH->draw_signature();
				read_state = 0;
			}
		break;
	}
	
}

