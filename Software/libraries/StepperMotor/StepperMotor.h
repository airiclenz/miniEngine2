/*

    (c) 2013 Airic Lenz 
	
	See www.airiclenz.com for more information
    		    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#ifndef StepperMotor_h
#define StepperMotor_h


#include "MotorBezier.h"


#define		TYPE_LINEAR		0
#define		TYPE_RADIAL		1


/*
// callback function
extern "C" {
    typedef void (*CallbackFunction)(void);
}
*/


// ============================================================================
// ============================================================================
// ============================================================================
class StepperMotor {
	
	///////////////////////////////////////////
	public:
	///////////////////////////////////////////
	
		//StepperMotor();
		StepperMotor(uint8_t, uint8_t);
			
		void		setPinEnable(uint8_t, bool);
		void		setType(uint8_t);
		void		setKeepPowered(bool);
		void		setCalibration(float);
		void 		setDirection(bool);
		void 		setMotorPosition(int32_t);
		void		setPostDelay(uint32_t);
		void		setDirectionFlipped(bool);
		void		setMaxSpeed(float);
		void		setLimitRetractionDistance(float);
		
		
		float		getCalibration(void);
		bool		getDirection(void);
		uint8_t		getType(void);
		float		getMotorPosition();
		int32_t		getMotorRawPosition();
		uint32_t	getPostDelay(void);
		float		getMaxSpeed(void);
		float		getLimitRetractionDistance(void);
		
		
		void 		enable(void);
		void		disable(void);
		
		
		bool		isKeepPowered(void);
		bool		isEnabled(void);
		bool 		isLimitSwitch(void);
		bool		isDirectionFlipped();
		
				
		void   		limitSwitchEvent();
	//	void 		registerLimitSwitchEvent(CallbackFunction newFunction);
		void		resetLimitSwitch(void);
	//	bool		toggleDirectionFlipped(void);
		bool		step(void);
					
		
		void 		startJog(void);
		void 		stopJog(void); 
		bool		setJogSpeed(float);
		float		getJogSpeed(void);
		void 		processJog(void);
	
	
	///////////////////////////////////////////
	private:
	///////////////////////////////////////////
		
	
		uint8_t 			_pin_step;
		volatile uint32_t 	*_port_step;
		uint32_t			_bit_step;
		
		uint8_t 			_pin_dir;
		volatile uint32_t 	*_port_dir;
		uint32_t			_bit_dir;
		
		uint8_t 			_pin_enable;
		uint8_t				_pin_ls1;
		uint8_t				_pin_ls2;
				
	//	CallbackFunction	_functionLimitSwitchEvent;		// the function to be called
		bool				_limitActive;					// limit switch limit
		bool				_dir;							// motor direction
		bool				_dir_flipped;					// invert the direction internally
		bool				_enable_state;					// the state in which the motor is enabled
		bool				_keep_powered;					// keep the motor powered all the time
		bool				_enabled;						// a var for avoiding digital reads
		
		int32_t				_motor_position;				// the position of the motor in steps
		uint32_t			_post_delay;					// the delay in ms after the motor moved
		uint8_t				_type;							// the motor type (radial, linear) 
		float				_calibration;					// the calibration factor (steps/cm; steps/°)
		float    			_max_speed;						// the max speed the motor can do
		float				_limitEventRetractionDist;		// the dustance the motor moves backwards after a limit switch event


		bool 				_jog;
		bool				_jog_speed_change;				// flag used for speed changes
		LinearFunction 		_jog_func;
		uint32_t			_jog_time;
		
};


#endif	

