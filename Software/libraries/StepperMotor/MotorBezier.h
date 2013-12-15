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


#ifndef MotorBezier_h
#define MotorBezier_h

#include "QuadBezierCurve.h"
#include "LinearFunction.h"
#include <Arduino.h>
#include <math.h> 



#define  MOTORBEZIER_SEGMENT_COUNT  200


// ============================================================================
// ============================================================================
// ============================================================================
class MotorBezier {


	///////////////////////////////////////////
	public:
	///////////////////////////////////////////
			
		MotorBezier(void);	
	//	MotorBezier(int);
		~MotorBezier(void);
		
	//	int 		segmentateCurveDynamic(QuadBezierCurve, float);
		void		segmentateCurveOptimized(QuadBezierCurve);
		void		segmentateCurve(QuadBezierCurve);

			
		void 		initMove();
		float 		getY(float);
		
		uint16_t	setSegmentCount(uint16_t);
		uint16_t 	getSegmentCount();
		uint16_t 	getIndex();
		
		float 		getStartX();
		float 		getEndX();
		
		boolean 	setIndex(int);
		Point 		getSegmentStartPoint();
				
		
	///////////////////////////////////////////
	private:
	///////////////////////////////////////////

		LinearFunction			_functionArray[MOTORBEZIER_SEGMENT_COUNT];
		
		float					_endX;
		uint16_t				_segmentCount;
		uint16_t				_index;
		
		float 		getAngleFromSlope(float);

		
		
};

#endif		