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




#include "Arduino.h"
#include "RotaryEncoder.h"


// we use the effect that a rotation shows a very specific pattern as
// a gray code:
//
// Rotation				0		1/2		1/2		3/4		1
// --------------------------------------------------------
// pin A (Bit0)			0		1		1		0		0
// pin B (Bit1)			0		0		1		1		0
// --------------------------------------------------------
// value as Byte		0		1		3		2		0
//
//
// ...this means if we receive a pattern of [0,1,3,2,0] we know
// that the encoder was rotated in one direction and if we receive a
// [0,2,3,1,0] it was rotated in the other direction. Thus we want
// to have some tollerance built in and we don't need the full cycle
// to identify a valid rotation we just check the last 3 values of the
// sequence (...[3,2,0], ...[3,1,0]). These sequences are here stored
// as a bit sequence (2 bits together are one number):
//				   		   3 1 0
#define DIR_CW			B00110100	 
//				   		   3 2 0
#define DIR_CCW			B00111000


#define BIT_FLIP		B00000001	// flipped rotation flag
#define BIT_DAMP		B00000010	// dampening enabled flag
#define BIT_KEYMODE		B00000100	// keymode enabled flag

#define BIT_UP			B01000000	// KEY UP
#define BIT_DOWN		B10000000	// KEY DOWN


// when rotating faster than a step every 150ms, the fast velocity is 
// active
	
#define VELOCITY_CHANGE_FAST	 17		
#define VELOCITY_CHANGE_SLOW	178		



// ============================================================================
RotaryEncoder::RotaryEncoder(byte pinA, byte pinB) {
	
	pinMode(_pinA, INPUT);
  	pinMode(_pinB, INPUT);
	
	_pinA					= pinA;
	_pinB					= pinB;
	
	_rotary_step 			= 1;
	_rotary_position		= 0;
	_rotary_position_max	= 0; // 0 = no limit
	_rotary_position_min	= 0; // 0 = no limit
	 
	
	_state 					= 0;
	_inter_state			= 0;
	_inter_state_old		= 0;
			
	_damp_bottom 			= 0;
	_damp_top 				= 0;
	_damp_factor 			= 1;
	
	
	_velocity				= 1;
		
	// set flipped, dampening and keymode to OFF
	_rotary_status 			= 0;

}



// ============================================================================
// Read input pins and process for events. Call this either from a
// loop or an interrupt (eg pin change or timer).
boolean RotaryEncoder::process() {
  
	// get the current pin state
	_inter_state = (digitalRead(_pinB) << 1) | digitalRead(_pinA);
	
	// any changes in the state?
	if (_inter_state != _inter_state_old) {
		
		// store the old value
		_inter_state_old = _inter_state;
	
		// shift the old states in by 2 
		// (uses the byte as buffer for the last 4 states)
		_state <<= 2;
		
		// add the new state to B0, B1   
		_state |= _inter_state;
	
		// if the last state is 00 one gray-code cycle is over 
		if (_inter_state == 0) {
			
			float step = _rotary_step;
			
		  	if ((isBit(_rotary_status, BIT_DAMP)) && 
				(_rotary_position > _damp_bottom) && 
				(_rotary_position < _damp_top)) {
					
		 		step *= _damp_factor;
		  	} 
			
			// use a bit mask to just the the last 3 states
			if ((_state & B00111111) == DIR_CW) {
				
				// add the step to the rotary position
				_rotary_position -= step;
				
				// check limits
				if ((_rotary_position_min != 0) && 
					(_rotary_position < _rotary_position_min)) {
						
					_rotary_position = _rotary_position_min;
				}
								
				// how fast are we turning?
				uint32_t sinceLastStep = millis() - _lastStepTime;
				if (sinceLastStep < VELOCITY_CHANGE_FAST) {
					_velocity = 100;			
				} else if (sinceLastStep > VELOCITY_CHANGE_SLOW) {
					_velocity = 1;					
				} else {
					_velocity = 10;	
				}
				
				// remember the time whn we did this
				_lastStepTime += sinceLastStep;
				
				// key mode enabled?
				if (isBit(_rotary_status, BIT_KEYMODE)) {
					
					// new full (integer) step done?
					if (((int16_t)_rotary_position) != _rotary_position_old) {
						
						if (isBit(_rotary_status, BIT_FLIP)) {
							setBit(_rotary_status, BIT_UP);
						} else {
							setBit(_rotary_status, BIT_DOWN);
						}
						
						// store the new value
						_rotary_position_old = _rotary_position;
					}
				}
				
				return true;
			} 
			
			// use a bit mask to just the the last 3 states
			if ((_state & B00111111) == DIR_CCW) {
				
				// add the step to the rotary position
				_rotary_position += step;
				
				// check limits
				if ((_rotary_position_max != 0) && 
					(_rotary_position > _rotary_position_max)) {
						
					_rotary_position = _rotary_position_max;
				}	
				
				// how fast are we turning?
				uint32_t sinceLastStep = millis() - _lastStepTime;
				if (sinceLastStep < VELOCITY_CHANGE_FAST) {
					_velocity = 100;			
				} else if (sinceLastStep > VELOCITY_CHANGE_SLOW) {
					_velocity = 1;					
				} else {
					_velocity = 10;	
				}
				
				// remember the time whn we did this
				_lastStepTime += sinceLastStep;
								
				// key mode enabled?
				if (isBit(_rotary_status, BIT_KEYMODE)) {
						
					// new full (integer) step done?
					if (((int16_t)_rotary_position) != _rotary_position_old) {
						
						if (isBit(_rotary_status, BIT_FLIP)) {
							setBit(_rotary_status, BIT_DOWN);
						} else {
							setBit(_rotary_status, BIT_UP);
						}
						
						// store the new value
						_rotary_position_old = _rotary_position;
					}
				}
								
				return true;
			}
				
		}
		
	}
	
	return false;
  
}


// ============================================================================
int32_t RotaryEncoder::getPosition() {
	return (int32_t)_rotary_position;  
}

// ============================================================================
void RotaryEncoder::setPosition(float value) {
	_rotary_position = value;  
}

// ============================================================================
void RotaryEncoder::setPositionMin(int16_t value) {
	_rotary_position_min = value;  

	if (_rotary_position < _rotary_position_min) {
		_rotary_position = _rotary_position_min;
	}
}

// ============================================================================
void RotaryEncoder::setPositionMax(int16_t value) {
	_rotary_position_max = value;  

	if (_rotary_position > _rotary_position_max) {
		_rotary_position = _rotary_position_max;
	}
}

// ============================================================================
void RotaryEncoder::clearPositionLimits() {
	_rotary_position_min = 0;  
	_rotary_position_max = 0;  
}



// ============================================================================
// this function returns the current velocity as step-width
// ============================================================================
uint16_t RotaryEncoder::getHighVelocity() {
	return _velocity;
}

// ============================================================================
// this function returns the current velocity as step-width
// ============================================================================
uint16_t RotaryEncoder::getLowVelocity() {
	uint16_t lowVel = _velocity / 10;
	if (lowVel == 0) return 1;
	else 			 return lowVel;
}


// ============================================================================
float RotaryEncoder::getStepWidth() {
	if (_rotary_step >= 0) return _rotary_step;
  	else 				   return -_rotary_step;
}

// ============================================================================
void RotaryEncoder::setStepWidth(float step) {
  
	if (isBit(_rotary_status, BIT_FLIP)) _rotary_step = -step;
	else              					 _rotary_step = step;
}


// ============================================================================
void RotaryEncoder::setFlipped(boolean value) {
  
  	// flip the step if needed:
    if (value != isBit(_rotary_status, BIT_FLIP)) {
      _rotary_step = -_rotary_step;  
    }

	// set the flag
  	if (value) {
  		setBit(_rotary_status, BIT_FLIP);
  	} else {
  		deleteBit(_rotary_status, BIT_FLIP);
  	}
  
} 

// ============================================================================
boolean RotaryEncoder::isFlipped() {
	return isBit(_rotary_status, BIT_FLIP);
} 


// ============================================================================
void RotaryEncoder::setStepDampZone(int bottom, int top, float factor) {

	setBit(_rotary_status, BIT_DAMP);
	
	_damp_bottom = bottom;
	_damp_top = top;
	_damp_factor = factor;

}


// ============================================================================
// Enable the key-mode (UP & DOWN)
void RotaryEncoder::setKeyMode(boolean value) {
	
	// set the flag
	if (value) {
		setBit(_rotary_status, BIT_KEYMODE);
	} else {
		deleteBit(_rotary_status, BIT_KEYMODE);
	}
		
}


// ============================================================================
// is the key-mode enabled?
boolean RotaryEncoder::isKeyMode() {
	return isBit(_rotary_status, BIT_KEYMODE);
}


// ============================================================================
void RotaryEncoder::disableStepDampZone() {
	deleteBit(_rotary_status, BIT_DAMP);
}


// ============================================================================
boolean RotaryEncoder::isKeyEvent() {
	return isBit(_rotary_status, B11000000);
}


// ============================================================================
boolean RotaryEncoder::clearKeyEvents() {
	return deleteBit(_rotary_status, B11000000);
}


// ============================================================================
boolean RotaryEncoder::isKeyUp() {
	return isBit(_rotary_status, BIT_UP);
}

// ============================================================================
boolean RotaryEncoder::isKeyDown() {
	return isBit(_rotary_status, BIT_DOWN);
}




