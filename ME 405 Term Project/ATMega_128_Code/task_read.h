/** task_read.h is the header file for task_read and includes a number of file scoped variables
*	and a single method, run.
*/



//-------------------------------------------------------------------------------------

/// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _task_read_H_
#define _task_read_H_


//-------------------------------------------------------------------------------------

class task_read
{
	protected:
		/// This input driver class needs a pointer to the serial port used to echo inputs to the terminal.  
		base_text_serial* ptr_2_serial;			
		/// this input driver class needs pointers to each PID object it is expected to interface with
		task_PID* PID_1;						
		/// this input driver class needs pointers to each PID object it is expected to interface with
		task_PID* PID_2;							
		/// this input driver class needs a pointer to the SPI master object for reset purposes
		Master* MASTER;								
		/// this input driver class needs a pointer to task_lines to initiate the coordinate conversion process
		task_lines* MAKE_PATH;							
		/// print_mode is used in task_print to tell what to print and task_read to tell when printing is done
		uint8_t* ptr_2_print_mode;
		/// servo object allowing pen to be actuated
		servo* You_Just_Got_Servoed;
		/// Go_Home object allows task_read to initiate homing procedure
		Go_Home* Plotter_Home;
		/// point object so make point operation can be initialized
		point* Make_Point;
		
		
		/// sets coordinate inpute mode to take 4 coords
		bool coord_request;
		/// sets coordinate inpute mode to take 2 coords
		bool point_request;
		/// buffer to store inputted char
		char input_char;								
		/// state variable for task_read
		uint8_t read_state;		
		/// read_mode tells input states where to save to
		uint8_t read_mode;								
		/// temporary storage of a coordinate during conversion
		uint16_t coordinate;							
		/// temporary storage of a gain value during conversion. (this gets typecasted to 16 bit at save)
		uint32_t K;										
		/// counts number of inputted digits and helps with decimal point logic for coordinates.					
		uint8_t index;									
		/// helps in coordinate entry
		uint8_t dec_flag;
		/// initial x coordinate
		uint16_t X0;
		/// initial y coordinate
		uint16_t Y0;
		/// final x coordinate or point x coordinate
		uint16_t X_f;
		/// final y coordinate or point y coordinate
		uint16_t Y_f;		
		/// keeps track of where to save what coordianate as they're entered
		uint8_t coord_num;

	public:
		
	
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
		task_read (base_text_serial* p_serial_port, task_PID* motor_1, task_PID* motor_2, uint8_t* p_print_mode,
					 Master* SPI, task_lines* LINES, servo* Tom_Servo, Go_Home* Home_Slice, point* POINTY);
			   
		/**	the run method handles all user inputs. In easy cases, actions are taken here. in longer cases, states are used
		*	and flags indicate to another task, task_print, that a message should be printed to screen. 
		*/
		void run(void);
};


#endif // _task_read_H_

