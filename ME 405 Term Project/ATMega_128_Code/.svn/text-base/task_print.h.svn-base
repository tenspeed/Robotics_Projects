/** task_print is a state machine which handles the screen printout portion of the user 
*	interface for a ME405 term project. State 0 checks to see if any prompts, errors or
*	menus need to be printed. If so, the strings are copied to a buffer. State 1 prints the 
*	buffer one character at a time so as to be cooperative. When the string end is reached,
*	an endline is printed and State is returned to 0. The exception to this rule is the case 
*	of the help menu. Because the menu is so long, it is divided into two strings. State two
*	detects the end of the first half of the menu, copies the second half to the buffer and 
*	then continues to print one character at a time. Normal string end procedures are then followed.
*/
//-------------------------------------------------------------------------------------

/// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _task_print_H_
#define _task_print_H_


//-------------------------------------------------------------------------------------
/**  Following are the headers of the constructor and methods contained in task_print.cpp.
*/
 

class task_print
{
	protected:
		/// The encoder driver class needs a pointer to the serial port used to output to the terminal.  
		base_text_serial* ptr_2_serial;			
		/// pointer to variable indicating what to print
		uint8_t* ptr_2_Which_Msg;				
		/// state variable for task_print
		uint8_t print_state;					
		/// buffer for currently printing message
		char buffer[100];						
		/// keeps track of location in a string
		uint8_t index;							

	public:
		/** The constructor creates a task_print object.
		*	@param p_serial_port:	A serial port object allows printing to the screen
		*	@param p_print_mode:	A variable which tells task_print which message to print and also
		*							tells task_read when the printing has been completed.
		*/
		task_print(base_text_serial* p_serial_port, uint8_t* p_print_mode);
		
		/**	run is task_print's only method. It performs all screen printing of menus, prompts
		*	and errors. Printing is done one character at a time using a two state format. State 
		*	0 checks to see if any prompts, errors or menus need to be printed. If so, the strings are 
		*	copied to a buffer. State 1 prints the buffer one character at a time so as to be
		*	cooperative. When the string end is reached, an endline is printed and State is returned
		*	to 0. The exception to this rule is the case of the help menu. Because the menu is so long,
		*	it is divided into two strings. State two detects the end of the first half of the menu,
		*	copies the second half to the buffer and then continues to print one character at a time.
		*	Normal string end procedures are then followed.
		*/
		void run(void);
			   
		
		
};


#endif // _task_print_H_