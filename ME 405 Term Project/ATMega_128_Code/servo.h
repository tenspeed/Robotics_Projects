// This define prevents this .h file from being included more than once in a .cpp file
#ifndef _servo_H_
#define _servo_H_
//-------------------------------------------------------------------------------------
/** Servo.cpp is a class with methods and inline methods for controlling a hobby servo
*	to raise and lower the pen for making points and lines. The servo is connected to
*	a PWM output pin on the microcontroller and the angle is related to the duty cycle
*	of the PWM signal.
*/
class servo
{
	protected:
		/// This servo driver class needs a pointer to the serial port so it can talk 
		base_text_serial* ptr_2_serial;	
		
		/// buffer for converting angle to PWM duty cycle
		uint8_t ServoSet;
		
	public:
		/** CONSTRUCTOR initializes PWM output for a RC servo motor. and sets pen height to a safe
		*	value. Servo output pins are A0 & A1
		*	@param p_serial_port Serial object so we can write to screen
		*/
		servo(base_text_serial* p_serial_port);
			   
		/** Set_Angle sets the angle of the servo.
		*	@param angle: an angle from 0 to 360 degrees is accepted
		*/
		void Set_Angle(uint16_t angle);
		
		/// Pen_Up raises the pen
		void Pen_Up(void) { OCR3AL = 7; }
		
		/// Fine_Line touches pen to paper softly
		void Fine_Line(void) { OCR3AL = 20; }
		
		/// Heavy_Line plants the pen hard
		void Heavy_Line(void) { OCR3AL = 11; }
		
		/// Change_Pen raises the pen extra high to change it easier
		void Change_Pen(void) { OCR3AL = 5; }
};


#endif // _servo_H_

