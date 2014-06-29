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

#include "MotorBezier.h"



// ============================================================================
// P U B L I C   F U N C T I O N S
// ============================================================================

// ============================================================================
MotorBezier::MotorBezier() {
	
	//_functionArray[MOTORBEZIER_SEGMENT_COUNT];
	
	_index 			= 0;
	_segmentCount 	= MOTORBEZIER_SEGMENT_COUNT;
	_endX			= 0;
	_maxSlope		= 0;
	
	//debug = false;
}


// ============================================================================
MotorBezier::~MotorBezier() { 
	free(&_index);
	free(&_segmentCount);
	free(&_functionArray); 
	free(&_endX); 
	free(&_maxSlope);
}



//================================================================================
void MotorBezier::initMove() {
	_index = 0;
}

//================================================================================
uint16_t MotorBezier::getIndex() {
	return _index;
}



/**===============================================================================
*	This function just sets the used cound to a value between 0 and MOTORBEZIER_SEGMENT_COUNT.
*	The actual array size doesn't get changed (for memory segmentation reasons)
**/
uint16_t MotorBezier::setSegmentCount(uint16_t value) {
	if (value <= MOTORBEZIER_SEGMENT_COUNT) {
		_segmentCount = value;
	} else {
		_segmentCount = MOTORBEZIER_SEGMENT_COUNT;
	}
	
	return _segmentCount;
}

//================================================================================
uint16_t MotorBezier::getSegmentCount() {
	return _segmentCount;
}


//================================================================================
float MotorBezier::getY(float x) {
		
	// jump up to the next segment if needed
	while ((_index < (_segmentCount - 1)) &&
		   (x > _functionArray[_index + 1].xstart)) {
			
		_index++;
	}
	
		
	return _functionArray[_index].getY(x);
				
}

//================================================================================
float MotorBezier::getStartX() {
	
	if (_segmentCount > 0) return _functionArray[0].xstart;
	else				   return 0;				
}

//================================================================================
float MotorBezier::getEndX() {
		
	return _endX;
				
}


//================================================================================
// Returns the max slope which is known after the curve was segmentated.
// This value is always returned positively as an ABS(maxSlpe)
//================================================================================
float MotorBezier::getMaxSlope() {
	return _maxSlope; 
}





//================================================================================
// This function takes a bezier curve object as parameter and then converts
// it to linear segments. It just divides the t-variable into linear parts
// (e.g. 200 segments: tStep = 1 / 200)
// Then it goes along t and saves the linear segments between the different points
//================================================================================
void MotorBezier::segmentateCurve(QuadBezierCurve curve) {
	
	Point f, fd, fdd, fddd, fdd_per_2, fddd_per_2, fddd_per_6;
	LinearFunction segment;
	
	this->_maxSlope = 0.0;
	float abs_slope = 0.0;
	float t = (1.0 / (double)(_segmentCount));
	float temp = t * t;
	
	f = curve.p0;
	fd = (curve.p1 - curve.p0) * t * 3;
	fdd_per_2 = (curve.p0 - (curve.p1 * 2) + curve.p2) * temp * 3;
	fddd_per_2 = (((curve.p1 - curve.p2) * 3) + curve.p3 - curve.p0) * temp * t * 3;
	
	fddd = fddd_per_2 + fddd_per_2;
	fdd = fdd_per_2 + fdd_per_2;
	fddd_per_6 = fddd_per_2 * (1.0 / 3.0);
	
	Point pTemp = f;
	
	for (uint16_t i=0; i<_segmentCount; i++) {
	  
	  	f = f + fd + fdd_per_2 + fddd_per_6;
	  	fd = fd + fdd + fddd_per_2;
	  	fdd = fdd + fddd;
	  	fdd_per_2 = fdd_per_2 + fddd_per_2;
		
		// check the max - slope value
		this->_functionArray[i] = LinearFunction(pTemp, f);	
				
		/*
		// print all the points
		Serial.print("seg:");
		Serial.print(i);
		Serial.print("; p0(");
		Serial.print(pTemp.x);
		Serial.print(",");
		Serial.print(pTemp.y);
		Serial.print("); p1(");
		Serial.print(f.x);
		Serial.print(",");
		Serial.print(f.y);
		Serial.print("); xs:");
		Serial.print(this->_functionArray[i].xstart);
		Serial.print(" sl:");
		Serial.print(this->_functionArray[i].slope);
		Serial.print(" yo:");
		Serial.println(this->_functionArray[i].yoffset);
		*/
		
				
		abs_slope = abs(this->_functionArray[i].slope);
		
		if (abs_slope > this->_maxSlope) {
			this->_maxSlope = abs_slope;
		}
		
		
		pTemp = f;
	}
	
	this->_endX = curve.p3.x;

}





//================================================================================
// This function takes a bezier curve object as parameter and then converts
// it to linear segments. It segmentates the way that the curved ends of the
// bezier curve get 90% off the total amount of segemts and the more linear
// part gets the other 10%. This is to ensure that the speed changes between the
// different segments are as minimal as possible.
//================================================================================
void MotorBezier::segmentateCurveOptimized(QuadBezierCurve curve) {
	
	this->_maxSlope = 0.0;
	float abs_slope = 0.0;
	
	
	// the portion of sub segments only for the curved parts (90%).
	// this means 10% are used for the linear parts
	float curvatureSegmentFactor = 0.9;
	int rampSubs = (int) round(( (float)_segmentCount) * curvatureSegmentFactor );
	
	// use ABS for close to zero values which might get negative due to
	// the floating point inaccuracy
	float timeTotal     = abs(curve.p3.x - curve.p0.x);
	float rampInTime    = abs(curve.p1.x - curve.p0.x);
	float rampOutTime   = abs(curve.p3.x - curve.p2.x);
	float rampTimeTotal = rampInTime + rampOutTime;
	
	
	/*
	Serial.print("timeTotal: ");
	Serial.println(timeTotal);
	Serial.print("rampInTime: ");
	Serial.println(rampInTime);
	Serial.print("rampOutTime: ");
	Serial.println(rampOutTime);
	Serial.print("rampTimeTotal: ");
	Serial.println(rampTimeTotal);
	Serial.println();
	*/
	
		
	// if the total ramp distance is bigger than the segmentation-factor-percentage OR
	// there is effectively no ramp-time	
	// ---> it is more reasonable to use the default sementation method
	if (
		((rampTimeTotal / timeTotal) >= curvatureSegmentFactor) ||
		((rampTimeTotal / timeTotal) <= 0.01f )	 
	   ) {
		   
		//default segmentation
		segmentateCurve(curve);
		return;
						
	} else {
		
		// calculate the amount of segments for the different sections / parts of the curve
		int rampInSubs  = (int)((rampInTime / rampTimeTotal) * rampSubs);
		int rampOutSubs = rampSubs - rampInSubs;
		int restSubs    = _segmentCount - rampSubs;
		
		
		/*
		Serial.print("rampInSubs: ");
		Serial.println(rampInSubs);
		Serial.print("rampOutSubs: ");
		Serial.println(rampOutSubs);
		Serial.print("restSubs: ");
		Serial.println(restSubs);
		Serial.println();	
		*/


		// get the t-coordinates for the different section borders
		// ("ramp in" <- tSeg1 -> "linear part" <- tSeg2 -> "ramp out")
		float tSeg1 = curve.getTFromX(curve.p1.x, rampTimeTotal * 0.001f);
		float tSeg2;
		
		if (rampOutTime != rampInTime) {
			tSeg2 = curve.getTFromX(curve.p2.x, rampTimeTotal * 0.001f);
		} else {
			tSeg2 = 1.0f - tSeg1;
		}
		
		// is there something strange? (position of the second t before the fist one?)
		// then do the default segmentation (fallback)				
		if (tSeg2 < tSeg1) {
			segmentateCurve(curve);
			return;
		}


		int segNo;
		float step;
		float t = 0.0f;

		if (rampInSubs > 0) {
			segNo = 0;
			step = tSeg1 / (float) rampInSubs;
		} 
		else if (restSubs > 0) {
			segNo = 1;
			step = (tSeg2 - tSeg1) / (float) restSubs;
		} 
		else {
			segNo = 2;
			step = (1.0f - tSeg2) / (float) rampOutSubs;
		}
		
		
		
		Point p0;
		Point p1 = curve.getPoint(0);
		
		// loop all segments
		for (int i=1; i<=_segmentCount; i++) {
				
			t += step;
			p0 = p1;
			p1 = curve.getPoint(t);
			
			// store the segment and the point in the result array
			this->_functionArray[i-1] = LinearFunction(p0, p1);	
			
			/*
			Serial.print("seg");
			Serial.print(i);
			Serial.print("; p0(");
			Serial.print(p0.x);
			Serial.print(", ");
			Serial.print(p0.y);
			Serial.print("); p1(");
			Serial.print(p1.x);
			Serial.print(", ");
			Serial.print(p1.y);
			Serial.print("); xs:");
			Serial.print(this->_functionArray[i-1].xstart);
			Serial.print("; sl:");
			Serial.print(this->_functionArray[i-1].slope);
			Serial.print("; yo:");
			Serial.println(this->_functionArray[i-1].yoffset);	
			*/
			
			// check the max - slope value
			abs_slope = abs(this->_functionArray[i-1].slope);
				
			if (abs_slope > this->_maxSlope) {
				this->_maxSlope = abs_slope;
				
				//Serial.print("new max slope: ");
				//Serial.println(this->_maxSlope);	
			}
			
			// do we need to change the s-step size because we reched the next phase?
			if ((t >= tSeg1) && (segNo == 0)) {
				segNo++;
				step = (tSeg2 - tSeg1) / (float) restSubs;
			}
			
			// change the step size bevause we reached the last phase
			else if ((t >= tSeg2) && (segNo == 1)) {
				segNo++;
				step = (1.0f - tSeg2) / (float) rampOutSubs;
			}

		} // end: for
		
		this->_endX = curve.p3.x;
						
	} // end: do intelligent segmentation 
		
};





/*

// ============================================================================
int MotorBezier::segmentateCurveDynamic(QuadBezierCurve curve, float angleLimit) {

	//_functionArray.clear();
	
	// store the last X point
	_endX = curve.p3.x;
	
	curve.updateDimension();
	float ratio = curve.xDimension / curve.yDimension;
	float yFac = ratio;
	float timeFac = curve.xDimension / 60000;
	if (timeFac < 1) timeFac = 1;
		
	int subs = (int)((20 / angleLimit) * timeFac); 
	int jumpedSubs = 0;
		
	double step = 1 / (double) subs;
		
	double angleOld = 0;
	double angleTest, angleDiff;
		
	LinearFunction segment;
		
	Point p0 = curve.getPoint(0);
	Point p1 = Point();
	Point pNext;
	Point p1Temp;
	Point p0Temp = p0;//.clone();
	p0Temp.y = p0Temp.y * yFac;
			
						
	// loop all subDivisions of the Bézier curve 	
	for (int i=0; i<subs; i++) {
						
		// get the position of the next point (i+1)
		pNext = curve.getPoint((i + 1) * step);
			
		p1Temp = pNext;//.clone();
		p1Temp.y = p1Temp.y * yFac;
						
		segment = LinearFunction(p0Temp, p1Temp);
		angleTest = getAngleFromSlope(segment.slope);
		angleDiff = abs(angleOld - angleTest);
			
		
		// do we need to close the current segment?
		if  (
				   ((angleDiff > angleLimit) && (i != 0))  		// <-- angle bigger than the limit
				|| (i == (subs - 1)) 						// <-- last point (save the last segments)
				|| (jumpedSubs == (int)(subs / 9))		// <-- do at least 10 segments (even on a straight line)
			) {
				
			jumpedSubs = 0;
				
			// we reached the last loop - close the current segment
			if (i == (subs - 1)) {
				p1 = pNext;//.clone();
			}
				
			// store the segment and the point in the result array
			_functionArray.add(LinearFunction(p0, p1));
								
			// set the new last angle
			p1Temp = p1;//.clone();
			p1Temp.y = p1Temp.y * yFac;
			segment = LinearFunction(p0Temp, p1Temp);
			angleOld = getAngleFromSlope(segment.slope);
								
			// set the new last valid start point
			p0 = p1;//.clone();
			p0Temp = p0;//.clone();
			p0Temp.y = p0Temp.y * yFac;
								
		} else {
				
			jumpedSubs++;
		}
					
		p1 = pNext;//.clone();
									
	}
	
	_segmentCount = _functionArray.size();
	return _segmentCount;
	
}	

*/



// ============================================================================
// P R I V A T E   F U N C T I O N S
// ============================================================================


//================================================================================
float MotorBezier::getAngleFromSlope(float slope) {
	return atan(slope) * 57.2957795;
}















