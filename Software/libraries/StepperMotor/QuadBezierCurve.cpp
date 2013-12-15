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



const float fPolys[21][4] = { 
  {1.000000,  0.000000,  0.000000,  0.000000},
  {0.857375,  0.135375,  0.007125,  0.000125},
  {0.729000,  0.243000,  0.027000,  0.001000},
  {0.614125,  0.325125,  0.057375,  0.003375},
  {0.512000,  0.384000,  0.096000,  0.008000},
  {0.421875,  0.421875,  0.140625,  0.015625},
  {0.343000,  0.441000,  0.189000,  0.027000},
  {0.274625,  0.443625,  0.238875,  0.042875},
  {0.216000,  0.432000,  0.288000,  0.064000},
  {0.166375,  0.408375,  0.334125,  0.091125},
  {0.125000,  0.375000,  0.375000,  0.125000},
  {0.091125,  0.334125,  0.408375,  0.166375},
  {0.064000,  0.288000,  0.432000,  0.216000},
  {0.042875,  0.238875,  0.443625,  0.274625},
  {0.027000,  0.189000,  0.441000,  0.343000},
  {0.015625,  0.140625,  0.421875,  0.421875},
  {0.008000,  0.096000,  0.384000,  0.512000},
  {0.003375,  0.057375,  0.325125,  0.614125},
  {0.001000,  0.027000,  0.243000,  0.729000},
  {0.000125,  0.007125,  0.135375,  0.857375},
  {0.000000,  0.000000,  0.000000,  1.000000}
};


#include <Arduino.h>
#include "QuadBezierCurve.h"


// ============================================================================
QuadBezierCurve::QuadBezierCurve() {

	p0 = Point();
	p1 = Point();
	p2 = Point();
	p3 = Point();
	
	xDimension = 0;
	yDimension = 0;

}

// ============================================================================
// generates a curve with the given duration, distance and a ramp percentage
// ============================================================================
QuadBezierCurve::QuadBezierCurve(float duration, float distance, float ramp) {

	if (ramp < 0) ramp = 0;
	if (ramp > 1) ramp = 1;
	 
	p0 = Point(                                 0,        0);
	p1 = Point(           ((duration * ramp) / 2),        0);
	p2 = Point(duration - ((duration * ramp) / 2), distance);
	p3 = Point(                          duration, distance);
	
	updateDimension();

}


// ============================================================================
void QuadBezierCurve::updateDimension() {
	
	float min = p0.x;
	float max = p0.x;
		
	//if (p0.x < min) min = p0.x;
	if (p1.x < min) min = p1.x;
	if (p2.x < min) min = p2.x;
	if (p3.x < min) min = p3.x;
	
	//if (p0.x > max) max = p0.x;
	if (p1.x > max) max = p1.x;
	if (p2.x > max) max = p2.x;
	if (p3.x > max) max = p3.x;
	
	xDimension = max - min;
	
	min = p0.y;
	max = p0.y;
	
	//if (p0.y < min) min = p0.y;
	if (p1.y < min) min = p1.y;
	if (p2.y < min) min = p2.y;
	if (p3.y < min) min = p3.y;
	
	//if (p0.y > max) max = p0.y;
	if (p1.y > max) max = p1.y;
	if (p2.y > max) max = p2.y;
	if (p3.y > max) max = p3.y;
	
	yDimension = max - min;

}


// ============================================================================
void QuadBezierCurve::scale(Point vector) {
	
	p0.multiply(vector);
	p1.multiply(vector);
	p2.multiply(vector);
	p3.multiply(vector);

}


// ============================================================================
void QuadBezierCurve::shift(Point vector) {
	
	p0.add(vector);
	p1.add(vector);
	p2.add(vector);
	p3.add(vector);

}


// ============================================================================
QuadBezierCurve QuadBezierCurve::clone() {
	
	QuadBezierCurve res = QuadBezierCurve();
	
	res.p0 = this->p0.clone();
	res.p1 = this->p1.clone();
	res.p2 = this->p2.clone();
	res.p3 = this->p3.clone();
	
	res.xDimension = this->xDimension;
	res.yDimension = this->yDimension;
	
	return res;
	
}




// ============================================================================
Point QuadBezierCurve::getPoint(float t) {
		
	return Point(getPointX(t), getPointY(t));
}


// ============================================================================
float QuadBezierCurve::getPointX(float t) {
		
	return p0.x * pow(1-t, 3) +
		   p1.x * 3 * t * pow(1-t, 2) + 
		   p2.x * 3 * pow(t, 2) * (1-t) + 
		   p3.x * pow(t, 3);

}

// ============================================================================
float QuadBezierCurve::getPointY(float t) {
		
	return p0.y * pow(1-t, 3) +
		   p1.y * 3 * t * pow(1-t, 2) + 
		   p2.y * 3 * pow(t, 2) * (1-t) + 
		   p3.y * pow(t, 3);

}


// ============================================================================
Point QuadBezierCurve::getFastPoint(char t) {
    
	Point res = Point();
  	
  	res.x = ( (p0.x * fPolys[t][0]) +
              (p1.x * fPolys[t][1]) +
              (p2.x * fPolys[t][2]) +
              (p3.x * fPolys[t][3]) );
    
  	res.y = ( (p0.y * fPolys[t][0]) +
              (p1.y * fPolys[t][1]) +
              (p2.y * fPolys[t][2]) +
              (p3.y * fPolys[t][3]) );
    
  	return res;
}


//================================================================================
float QuadBezierCurve::getTFromX(float x, float limit) {

	float tC = 0.5f;
	float xC = getPointX(tC);
	float delta = abs(xC - x);
	float step = 0.5;
		
	while (delta > limit) {
			
		step /= 2;
			
		if (x < xC) {
			tC -= step;	
		} else {
			tC += step;		
		}
			
		xC = getPointX(tC);
		delta = xC - x;
		if (delta < 0) delta *= -1;
	}
				
	return tC;
				
}



