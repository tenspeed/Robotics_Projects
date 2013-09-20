/** \file point.cpp is a task which moves a R-THETA plotter to a specific location.
*	it can then make a dot or not as specified.
*/


// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _point_H_
#define _point_H_


//-------------------------------------------------------------------------------------

class point 
{
	protected:
		base_text_serial* ptr_2_serial;					
		task_PID* PID_1;								
		task_PID* PID_2;								
		servo* Pen_Servo;								
		/// puts point into go-to mode
		bool Go_To;
		/// puts point into make-point mode										
		bool Make_Point;	
		/// counting variable used to ensure pen gets down							
		uint16_t counting;	
		/// state variable							
		uint8_t point_state;	
		/// x coordinate desired						
		int16_t X;		
		/// y coordinate desired								
		int16_t Y;					
		/// current THETA value in ticks					
		int32_t encoder_now_theta;
		/// desired THETA value in ticks						
		int32_t encoder_goal_theta;
		/// current R value in ticks						
		int32_t encoder_now_r;	
		/// desired R value in ticks						
		int32_t encoder_goal_r;	
		/// allow pythagorean theorum without data loss						
		float x_squared;				
		/// allow pythagorean theorum without data loss				
		float y_squared;								
		
		
	public:

		point(base_text_serial* p_serial_port, task_PID* motor_1, task_PID* motor_2, servo* Penny_Thingy); 

		void run(void);

		void Get_There(uint16_t Xcoord, uint16_t Ycoord);

		bool Are_We_There(void);

		void Go_Make_A_Dot(uint16_t Xcoord, uint16_t Ycoord);
};

#endif // _point_H_
