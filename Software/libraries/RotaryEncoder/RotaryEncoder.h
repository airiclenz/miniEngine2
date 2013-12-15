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


#ifndef RotaryEncoder_h
#define RotaryEncoder_h


#include "Arduino.h"
#include "bitOps.h"


// ============================================================================
// ============================================================================
// ============================================================================
class RotaryEncoder {



	///////////////////////////////////////////
	public:
	///////////////////////////////////////////
		
	
		RotaryEncoder(byte, byte);
		
		boolean		process(void);
		
		int32_t 	getPosition(void);
		float 		getStepWidth(void);
		uint16_t	getHighVelocity(void);
		uint16_t	getLowVelocity(void);
		
		boolean		isKeyMode(void);
		boolean		isKeyEvent(void);
		boolean		isKeyUp(void);
		boolean		isKeyDown(void);
		boolean		isFlipped(void);
		
		boolean 	clearKeyEvents(void);
		void		clearPositionLimits(void);
				
		void 		setPosition(float);
		void 		setPositionMax(int16_t);
		void 		setPositionMin(int16_t);
		void 		setStepWidth(float);
		void 		setStepDampZone(int, int, float);
		void 		setKeyMode(boolean);
		void 		setFlipped(boolean);
		
		void 		disableStepDampZone(void);
		
		
	///////////////////////////////////////////	
	private:
	///////////////////////////////////////////

		uint8_t 			_pinA;
		uint8_t 			_pinB;
		
		float 				_rotary_step;
		volatile float	 	_rotary_position;
		volatile int16_t	_rotary_position_old;
		
		volatile int16_t	_rotary_position_max;
		volatile int16_t	_rotary_position_min;
		
			
		// B0 = flipped
		// B1 = damp enbaled
		// B2 = key mode
		// B3 = 
		// B4 = 
		// B5 =
		// B6 = key UP 
		// B7 = key DOWN
		uint8_t				_rotary_status;
		
		volatile uint8_t 	_state;
		volatile uint8_t 	_inter_state;
		volatile uint8_t	_inter_state_old;
		
		
		int16_t 			_damp_bottom;
		int16_t				_damp_top;
		float 				_damp_factor;
		
		uint16_t			_velocity;
		uint32_t			_lastStepTime;
		
};

#endif