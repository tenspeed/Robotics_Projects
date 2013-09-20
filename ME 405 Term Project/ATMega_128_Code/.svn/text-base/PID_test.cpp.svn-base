	//======================================================================================
	/** \file PID_test.cpp
	
	*  Revisions:
	*    \li  05-10-11  Began tearing and hacking at our lab_4 code.				
	*	 \li  05-22-11  That was really an ordeal. PID finally working.
	*  License:
	*    This file released under the Lesser GNU Public License. The program is intended
	*    for educational use only, but its use is not restricted thereto. 
	*/
	//======================================================================================

	// System headers included with < >
	#include <stdlib.h>							//!< Standard C library
	#include <avr/io.h>							//!< Input-output ports, special registers
	#include <avr/interrupt.h>					//!< Interrupt handling functions
	#include "rs232int.h"						//!< Include header for serial port class
	#include "stl_timer.h"						//!< Microsecond-resolution timer
	#include "stl_task.h"						//!< Base class for all task classes

	// User written headers included with " "
	#include "Master.h"							//!< allows SPI communications to operate
	#include "da_motor.h"						//!< allows motor control to occur
	#include "task_PID.h"						//!< allow PID control
	#include "servo.h"
	#include "point.h"
	
	#include "task_lines.h"						//!< breaks up a line into small segments
	
	#include "Go_Home.h"
	
	#include "task_read.h"						//!< allow user input
	#include "task_print.h"						//!< allow messages and errors to print to screen
	

	//--------------------------------------------------------------------------------------
	/** The main function is the "entry point" of every C program, the one which runs first
	*  (after standard setup code has finished). For mechatronics programs, main() runs an
	*  infinite loop and never exits. 
	*/

	int main ()
	{
		volatile uint32_t dummy = 0;			//!< counter to control rate of SPI communication
		uint8_t print_mode = 0;					//!< controls what is printed to screen
		
		
		// Create a serial port object. Test voltages will be printed to this port, which
		// should be hooked up to a dumb terminal program like minicom or GTKterm
		rs232 the_serial_port (9600, 1);

		// Say hello
		the_serial_port << endl << "SPI: Master Driver test v0.001" << endl;

		// Create a MASTER object, "request". This object must be given a
		// pointer to the serial port object so that it can print debugging information
		// Pointers to encoder variables and transfer error variables are also given so
		// Master can store those values in main. Also, a encoder_select pointer is passed 
		// to help determine where to store data.
		Master request (&the_serial_port);
		
		// Create a motor. This object must be given a pointer to the serial port object so that 
		// it can print debugging and motor operation information.
		da_motor my_motor (&the_serial_port);
				
		// Create a time stamp which holds the interval between runs of the motor task
		// The time stamp is initialized with a number of seconds, then microseconds
		time_stamp interval_time_1 (0, 25000);		// set PIDS to run at 20 ms intervals
		
		// Create a microsecond-resolution timer
		task_timer the_timer;
		
		// Create a PID object for motor 1
		task_PID motor_1 (&the_serial_port, the_timer, interval_time_1, &request, &my_motor, 1);
		
		// Create a PID object for motor 2
		task_PID motor_2 (&the_serial_port, the_timer, interval_time_1, &request, &my_motor, 2); 
		
		// Create a servo object to control pen height
		servo Pen_and_Teller(&the_serial_port);
	
		// Create a point object. it is great
		point The_Dot_Maker(&the_serial_port, &motor_1, &motor_2, &Pen_and_Teller);
		
		// Create an object for breaking up line segments
		task_lines yayo(&the_serial_port, the_timer, interval_time_1, &motor_1, &motor_2, &The_Dot_Maker, &Pen_and_Teller);
		
		// Create a homing object so you can find home no matter how wasted you are
		Go_Home Find_Home(&the_serial_port, the_timer, interval_time_1, &my_motor, &request, &motor_1, &motor_2, &yayo); 
		
		// Create a user interface object which takes in input
		task_read keyboard(&the_serial_port, &motor_1, &motor_2, &print_mode, &request, &yayo,
						   &Pen_and_Teller, &Find_Home, &The_Dot_Maker); 
										
		// Create a user interface object which prints to screen
		task_print screen_print(&the_serial_port, &print_mode);
		
		
		
		
		
		// Say hello
		the_serial_port << endl << "PID Controller test v0.001" << endl;
		// enable interrupts
		sei();
		/// gains set up to run motors at 12 V
		// set initial gain values. (can be changed in program too)
		// Cart
		motor_1.set_kp(950); //1000
		motor_1.set_ki(450); //500
		motor_1.set_kd(0); //0
		
		// Arm
		motor_2.set_kp(3750); //4000
		motor_2.set_ki(3700); //4000
		motor_2.set_kd(1); //1
		
		// Enter infinite main loop
		while (true)
		{
			
			keyboard.run();						// Get user input if there is any
			if (print_mode == 0xFF)				// If the user entered a q, or Q print this stuff uncooperatively
			{	
				int32_t cart = motor_1.GET_setpoint();
				int32_t arm = motor_2.GET_setpoint();
				// current gain and set point values
				the_serial_port << "1)p: "<< motor_1.GET_Kp() <<"  i: "<< motor_1.GET_Ki() <<"   d: "<< motor_1.GET_Kd() <<endl;
				the_serial_port << "2)p: "<< motor_2.GET_Kp() <<"  i: "<< motor_2.GET_Ki() <<"   d: "<< motor_2.GET_Kd() <<endl;
				the_serial_port << "SETPOINTS:  cart="<< cart<<"   theta="<< arm<<endl;			
				the_serial_port << "enc1="<< motor_1.Get_Encoder()<<"  enc2="<< motor_2.Get_Encoder()<<endl;
				print_mode = 0;					// reset print_mode
			}
			motor_1.schedule ();				// run PID #1 as scheduled
			motor_2.schedule ();				// run PID #2 as scheduled
			screen_print.run();					// print any errors/propmpts/menus necessary
			yayo.schedule();					// break up some lines
			motor_1.schedule ();				// run PID #1 as scheduled
			motor_2.schedule ();				// run PID #2 as scheduled
			Find_Home.schedule();				// home if requested
			The_Dot_Maker.run();
			
		}
		return (0);
	
	}
	