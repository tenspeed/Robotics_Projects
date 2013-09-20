/** \file point.cpp is a task which moves a R-THETA plotter to a specific location.
*	it can then make a dot or not as specified.
*/


/// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _point_H_
#define _point_H_


//-------------------------------------------------------------------------------------

class point 
{
	protected:
		// object pointers
		base_text_serial* ptr_2_serial;					//!< serial object so tas_read can echo stuff to screen	
		task_PID* PID_1;								//!< allows point to initiate motion to a point
		task_PID* PID_2;								//!< allows point to initiate motion to a point
		servo* Pen_Servo;								//!< allows pen actuation
		
		//local variables
		bool Go_To;										//!< puts point into go-to mode
		bool Make_Point;								//!< puts point into make-point mode
		uint16_t counting;								//!< counting variable used to ensure pen gets down
		uint8_t point_state;							//!< state variable
		int16_t X;										//!< x coordinate desired
		int16_t Y;										//!< y coordinate desired
		int32_t encoder_now_theta;						//!< current THETA value in ticks
		int32_t encoder_goal_theta;						//!< desired THETA value in ticks
		int32_t encoder_now_r;							//!< current R value in ticks
		int32_t encoder_goal_r;							//!< desired R value in ticks
		float x_squared;								//!< allow pythagorean theorum without data loss
		float y_squared;								//!< allow pythagorean theorum without data loss
		
		
	public:
		/** The constructor saves object pointers locally and initializes necessary variables.
		*	@param	p_serial_port:	A pointer to the serial port for printing purposes
		*	@param	motor_1:	A pointer to a motor object
		*	@param	motor_2:	A pointer to a second motor object
		*	@param	Penny_Thingy: A servo object used to move a pen
		*/
		point(base_text_serial* p_serial_port, task_PID* motor_1, task_PID* motor_2, servo* Penny_Thingy); 
	
		/** The run method moves the pen to a specific location. It has two modes. It can just move the pen somewhere
		*	or it can move the pen somewhere and then make a dot.
		*/
		void run(void);
			
		/** This method initiates a cairrage go to. This is how lines moves the pen around.
		*	@param Xcoord: An x coordinate to go to
		*	@param Ycoord: An y coordinate to go to
		*/
		void Get_There(uint16_t Xcoord, uint16_t Ycoord);

		/// this method allows someone to check if we're there yet. 
		bool Are_We_There(void);

		/** This method initiates a go to and then makes a point there.
		*	@param Xcoord: An x coordinate to go to
		*	@param Ycoord: An y coordinate to go to
		*/
		void Go_Make_A_Dot(uint16_t Xcoord, uint16_t Ycoord);
};

#endif // _point_H_
