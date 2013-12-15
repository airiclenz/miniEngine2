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


#include "StepperMotor.h"
#include "bitOps.h"


// ============================================================================
StepperMotor::StepperMotor(uint8_t pin_step, uint8_t pin_dir) {
	
	// store the pin numbers
	_pin_step = pin_step;
	_pin_dir = pin_dir;
	
	// retreive the port-mapping info
	_port_step = portOutputRegister(digitalPinToPort(_pin_step));
	_bit_step  = digitalPinToBitMask(_pin_step);
	
	_port_dir = portOutputRegister(digitalPinToPort(_pin_dir));
	_bit_dir  = digitalPinToBitMask(_pin_dir);
	
	
    // set the pin states
    pinMode(_pin_step, OUTPUT);
    pinMode(_pin_dir, OUTPUT);
        
    // and set the pin to reasonable start states
    digitalWrite(_pin_step, LOW);    
    digitalWrite(_pin_dir, LOW);
	
	// and also set the direction bit to 0    
    _dir = false;
	// reset the motor position	
	_motor_position = 0;
	
	// set the motor to a linear one
	_type = TYPE_LINEAR;
	// set the calibration to 100 steps / cm
	_calibration = 100;
	
	// clear the limit-event flag
	_limitActive = false;
	
	// insitialize the jog
	_jog 		= false;
	_jog_speed_change = false;
	_jog_func 	= LinearFunction();
	_jog_time	= 0;
	
	// keep the motor powered
	_keep_powered = false;
	
	// set the post delay to a standard value of 200ms
	_post_delay = 200;
	
	// set the max speed to 1000 steps / second
	_max_speed = (1000/_calibration);
	
	// this means that the motor is supposed to move backward
	// 1cm / degree after a limit switch even occured
	_limitEventRetractionDist = 1;
}



// ============================================================================
// P U B L I C   F U N C T I O N S
// ============================================================================

// ============================================================================
void StepperMotor::setPinEnable(uint8_t pin_enable, bool enableState) {
	
	// stor the pin
	_pin_enable = pin_enable;
	// set the enable - pin as output
	pinMode(_pin_enable, OUTPUT);
	
	// remember the enable state
	_enable_state = enableState;
	
	// disable the motor for now
    disable();
		
}




/*
// ============================================================================
void StepperMotor::registerLimitSwitchEvent(CallbackFunction newFunction) {
	_functionLimitSwitchEvent = newFunction;
}
*/

// ============================================================================
void StepperMotor::setLimitRetractionDistance(float value) {
	_limitEventRetractionDist = value;
}

// ============================================================================
float StepperMotor::getLimitRetractionDistance() {
	return _limitEventRetractionDist;
}



// ============================================================================
void StepperMotor::limitSwitchEvent() {
	
	// set the limit-event flag
	_limitActive = true;
	
	// invert the motor direction;
	setDirection(!_dir);
	
	// move back 0.5 cm
	long backSteps = _limitEventRetractionDist * _calibration;
		
	// move the motor back
	for (int i=0; i<=backSteps; i++) {
		
		setBit(*_port_step, _bit_step);
		delayMicroseconds(5);
    	deleteBit(*_port_step, _bit_step);
    
		if (_dir) _motor_position--;
    	else      _motor_position++;
		
		delayMicroseconds(500);
	}

}

// ============================================================================
void StepperMotor::resetLimitSwitch() {
	_limitActive = false;
}

// ============================================================================
bool StepperMotor::isLimitSwitch() {
	return _limitActive;
}




// ============================================================================
void StepperMotor::setType(uint8_t type) {
	_type = type;
}

// ============================================================================
uint8_t StepperMotor::getType() {
	return _type;
}


// ============================================================================
void StepperMotor::setCalibration(float cal) {
	_calibration = cal;
}

// ============================================================================
float StepperMotor::getCalibration() {
	return _calibration;
}




// ============================================================================
bool StepperMotor::isEnabled() {
	return _enabled;
}


// ============================================================================
void StepperMotor::enable() {
	if (_enable_state) {
		digitalWrite(_pin_enable, HIGH);
	} else {
		digitalWrite(_pin_enable, LOW);
	}
	
	_enabled = true;							      
}

// ============================================================================
void StepperMotor::disable() {
	
	// only diable if the motor should not be kept powered
	if (!_keep_powered) {
		if (_enable_state) {
			digitalWrite(_pin_enable, LOW);
		} else {
			digitalWrite(_pin_enable, HIGH);
		}
		
		_enabled = false;

	} 				      
}

// ============================================================================
void StepperMotor::setKeepPowered(bool value) {
	_keep_powered = value;
	
	// is the motor should be kept powered then enable it now
	if (_keep_powered) enable();
	else 			   disable();

}

// ============================================================================
bool StepperMotor::isKeepPowered(void) {
	return _keep_powered;
}


// ============================================================================
int32_t StepperMotor::getMotorRawPosition() {
	return _motor_position;
}

// ============================================================================
float StepperMotor::getMotorPosition() {
	return ((float)_motor_position) / _calibration;
}

// ============================================================================
void StepperMotor::setMotorPosition(int32_t value) {
	_motor_position = value;
}

// ============================================================================
void StepperMotor::setDirection(bool dir) {
	_dir = dir;
	
	// set the new direction
	if (_dir ^ _dir_flipped) {
		
		// set the dir pin high: digitalWrite(_pin_dir, HIGH);
		setBit(*_port_dir, _bit_dir);
		
	} else {
		
		// set the dir pin low: digitalWrite(_pin_dir, LOW);
		deleteBit(*_port_dir, _bit_dir);
	}
			
}

// ============================================================================
bool StepperMotor::getDirection() {
	return _dir;
}

// ============================================================================
bool StepperMotor::isDirectionFlipped() {
	return _dir_flipped;
}

// ============================================================================
void StepperMotor::setDirectionFlipped(bool state) {
	
	// set the new state
	_dir_flipped = state;
	
	// set the new direction
	if (_dir ^ _dir_flipped) {
		
		// set the dir pin high: digitalWrite(_pin_dir, HIGH);
		setBit(*_port_dir, _bit_dir);
		
	} else {
		
		// set the dir pin low: digitalWrite(_pin_dir, LOW);
		deleteBit(*_port_dir, _bit_dir);
	}
}



// ============================================================================
uint32_t StepperMotor::getPostDelay(void) {
	return _post_delay;
}

// ============================================================================
void StepperMotor::setPostDelay(uint32_t value) {
	_post_delay = value;	
}


// ============================================================================
float StepperMotor::getMaxSpeed() {
	return _max_speed;
}

// ============================================================================
void StepperMotor::setMaxSpeed(float value) {
	_max_speed = value;
}


// ====================================================================
// sets the jog-speed in cm/sec
// could be called by a rotary encoder:
// speed = (rotaryPosition / 10) ^ 2
bool StepperMotor::setJogSpeed(float speed) {
	
	// if the speed is not exceeding the max speed 
	if (abs(speed) <= _max_speed) {
		
		// set the new speed;
		_jog_func.slope = speed;
		// set the flag that the speed was changed;
		_jog_speed_change = true;
		
		return true;
	}
	
	return false;
}

// ====================================================================
float StepperMotor::getJogSpeed() {
	
	return _jog_func.slope;
}




// ====================================================================
// starts the motor jog
void StepperMotor::startJog() {
	_jog = true;
	setJogSpeed(0);
	enable();
}


// ====================================================================
// stops the motor jog
void StepperMotor::stopJog() {
	_jog = false;
	disable();
}


// ====================================================================
// processes the motor.jog and moves the motor to the new pos if needed
void StepperMotor::processJog() {
	
	// if we are in jog-mode right now
	if (_jog) {
		
		if (_jog_speed_change) {
			// set the new offset;
			_jog_func.yoffset = ((float) _motor_position) / _calibration;
			// set the new start time for this linear segment
			_jog_time = millis();
			// turn off the speed change flag
			_jog_speed_change = false;
		}
				
		// how much time went by since the 
		int32_t nTime = millis() - _jog_time;
		int32_t y = _jog_func.getY((float) nTime / 1000.0) * _calibration;
		int32_t stepsToDo = y - _motor_position;
		
		if (stepsToDo != 0) {
			
			// do we have a direction change?
			bool dir = (bool)(stepsToDo < 0);
			if (dir != _dir) setDirection(dir);
			
			// make the steps positive
			stepsToDo = abs(stepsToDo);
			
			// do all steps
			for (int i=0; i<stepsToDo; i++) {
				
				step();
				
				// wait a little bit if we need to do more steps
				if (i<(stepsToDo-1)) delayMicroseconds(5);
			}

		}
		
	}
	
}




// ============================================================================
bool StepperMotor::step() {
    
	if (!_limitActive) {
	
		// set the step pin high: digitalWrite(_pin_step, HIGH);
		setBit(*_port_step, _bit_step);
		
		delayMicroseconds(1);
    	
		// set the step pin low: digitalWrite(_pin_step, LOW);
    	deleteBit(*_port_step, _bit_step);
	
		if (_dir) _motor_position--;
    	else      _motor_position++;
		
		return true;
	} 
		
	return false;
	
}



















