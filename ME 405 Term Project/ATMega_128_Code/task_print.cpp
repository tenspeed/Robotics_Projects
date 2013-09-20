/** task_print is a state machine which handles the screen printout portion of the user 
*	interface for the polar plotter. State 0 checks to see if any prompts, errors or
*	menus need to be printed. If so, the strings are copied to a buffer. State 1 prints the 
*	buffer one character at a time so as to be cooperative. When the string end is reached,
*	an endline is printed and State is returned to 0. The exception to this rule is the case 
*	of the help menu. Because the menu is so long, it is divided into two strings. State two
*	detects the end of the first half of the menu, copies the second half to the buffer and 
*	then continues to print one character at a time. Normal string end procedures are then followed.
*/

#include <stdlib.h>							// Include standard library header files
#include <avr/io.h>							// You'll need this for SFR and bit names
#include <avr/pgmspace.h>					// allows strings to be stored in flash memory
#include "rs232int.h"						// Include header for serial port class
#include "da_motor.h"						// Include header for the da_motor class
#include "task_print.h"						// include own header file


//-------------------------------------------------------------------------------------
// Messages and prompts to display
char Msg3[] = "Please enter 10^6*K_i [0 to 32667]";
char Msg4[] = "Please enter 10^4*K_p [0 to 32667]";
char Msg5[] = "Please enter 100*K_d [0 to 32667]";
char Msg6[] = "\nCommands:\nSPACE: Emergency Stop\nM: Point Entry Mode\nC: Coordinate Entry Mode\nG: Go\nO: Home\nR: ";
char Msg7[] = "Invalid entry";
char Msg8[] = "Apply gain to which motor? [1 or 2]";
char Msg9[] = "Raise pen\nL: Lower pen\nX: signature\nZ: Reset\nP: Set K_p\nI: Set K_i\nD: Set K_d\nH,?: Display help";
char Msg10[] = "enter X final [0 - 34.0]\n";
char Msg11[] = "enter Y final [0 - 22.0]\n";
char Msg12[] = "enter X initial [0 - 34.0]\n";
char Msg13[] = "enter Y initial [0 - 22.0]\n";
char Msg14[] = "Drawing Signature\n";
//-------------------------------------------------------------------------------------
/** The constructor creates a task_print object.
*	@param p_serial_port:	A serial port object allows printing to the screen
*	@param p_print_mode:	A variable which tells task_print which message to print and also
*							tells task_read when the printing has been completed.
*/
task_print::task_print(base_text_serial* p_serial_port, uint8_t* p_print_mode)
{
	ptr_2_serial = p_serial_port;			// serial object to print to screen here
	ptr_2_Which_Msg = p_print_mode;			//print_mode (in main) tells task_print what to print to screen
											//print_mode also allows task_read to continue in user input operations
	
	// say hello
	*ptr_2_serial << "task_print is online" << endl;
	// initialize variables
	print_state = 0;
}

/** the run task handles all printing to screen. If another task requests a print by changing print_mode in
*	main, run moves the appropriate message from program memory to a buffer in data memory and then prints
*	the message one character per task call.
*/
void task_print::run(void)
{
	switch (print_state)
	{
		/** State 0 is a hub state. It checks to see if interface wants anything printed to screen. If so,
		*	the appropriate message is moved to a local buffer.
		*/
		case 0:
			switch (*ptr_2_Which_Msg)
			{	
				// CASE 0 means do nothing
				// case 1 was an x entry prompt - no longer used
				// case 2 was a y entry prompt - no longer used
 				// CASES 3 - 14 each initialize a message
				
				/// Message 3 is a K_i entry prompt
				case 3:
					print_state = 1;
					index = 0;
					while (Msg3[index])
					{
						buffer[index] = Msg3[index];
						index++;
					}
					buffer[index] = 0;
					index = 0;
				break;
				/// Message 3 is a K_p entry prompt
				case 4:
				print_state = 1;
					index = 0;
					while (Msg4[index])
					{
						buffer[index] = Msg4[index];
						index++;
					}
					buffer[index] = 0;
					index = 0;
				break;
				/// Message 3 is a K_d entry prompt
				case 5:
				print_state = 1;
					index = 0;
					while (Msg5[index])
					{
						buffer[index] = Msg5[index];
						index++;
					}
					buffer[index] = 0;
					index = 0;
				break;
				/// Message 6 is the first half of the help menu
				case 6:
					print_state = 1;
					index = 0;
					while (Msg6[index])
					{
						buffer[index] = Msg6[index];
						index++;
					}
					buffer[index] = 0;
					index = 0;
					
				break;
				/// Message 7 is an invalid input error
				case 7:
					print_state = 1;
					index = 0;
					while (Msg7[index])
					{
						buffer[index] = Msg7[index];
						index++;
					}
					buffer[index] = 0;
					index = 0;
				break;
				/// Message 8 is a prompt asking which motor to apply an entered gain to
				case 8:
					print_state = 1;
					index = 0;
					while (Msg8[index])
					{
						buffer[index] = Msg8[index];
						index++;
					}
					buffer[index] = 0;
					index = 0;
				break;
				
				/// case 9 is left out as a reminder that msg9 is the second part of help
				
				// case 10 -13 coordinate prompts
				/// Message 10
				case 10:
					print_state = 1;
					index = 0;
					while (Msg10[index])
					{
						buffer[index] = Msg10[index];
						index++;
					}
					buffer[index] = 0;
					index = 0;
				break;
				
				/// Message 11
				case 11:
					print_state = 1;
					index = 0;
					while (Msg11[index])
					{
						buffer[index] = Msg11[index];
						index++;
					}
					buffer[index] = 0;
					index = 0;
				break;
				
				/// Message 12
				case 12:
					print_state = 1;
					index = 0;
					while (Msg12[index])
					{
						buffer[index] = Msg12[index];
						index++;
					}
					buffer[index] = 0;
					index = 0;
				break;
				
				/// Message 13
				case 13:
					print_state = 1;
					index = 0;
					while (Msg13[index])
					{
						buffer[index] = Msg13[index];
						index++;
					}
					buffer[index] = 0;
					index = 0;
				break;
				
				case 14:
					print_state = 1;
					index = 0;
					while (Msg14[index])
					{
						buffer[index] = Msg14[index];
						index++;
					}
					buffer[index] = 0;
					index = 0;
				break;
 			}
		break;
		
		/** State 1 is the actual print state. it keeps track of its progress via index and prints until a nul
		*	character is encountered. The exception to this rule is in printing the help menu. Because of it's 
		*	length, the menu is divided into halves. When the first string ends, this state automatically 
		*	transitions to the second half of the menu. At the end of any message or menu an endline is printed 
		*	before returning to state 0.
		*/
		case 1:
		
			// if next char is not nul, continue
			if (buffer[index] != 0)
			{
				*ptr_2_serial << ascii<< buffer[index];			// print next char
				index++;										// increment index
			}
			// if finishing printing first half of help menu, transition to second half
			else if (*ptr_2_Which_Msg == 6)
			{
				index = 0;
				while (Msg9[index])
					{
						buffer[index] = Msg9[index];
						index++;
					}
				buffer[index] = 0;
				index = 0;
				*ptr_2_Which_Msg = 9;
			}
			// printing done
			else if (buffer[index] == 0)
			{
				*ptr_2_serial << endl;							// end line
				*ptr_2_Which_Msg = 0;							// lower print flag
				print_state = 0;
			}
		break;
	}
}