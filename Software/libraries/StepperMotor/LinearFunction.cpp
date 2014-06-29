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



#include "LinearFunction.h"




// ============================================================================
LinearFunction::LinearFunction() {
	this->slope = 0.0f;
	this->xstart = 0.0f;
	this->yoffset = 0.0f;
}


//================================================================================
LinearFunction::LinearFunction(Point point0, Point point1) {
		
	Point p0;
	Point p1;
		
	// sort the points
	if (point0.x < point1.x) {
		p0 = point0;//.clone();
		p1 = point1;//.clone();		
	} else {
		p0 = point1;//.clone();
		p1 = point0;//.clone();
	}
	
	this->xstart = p0.x;
	
	
	double xdist = (double) p1.x - (double) p0.x;
	
	if (xdist > 0.0000001) {
		this->slope = ((double) p1.y - (double) p0.y) / xdist;
	} else {
		this->slope = 0;
	}
	
	this->yoffset  = p0.y - (this->slope * p0.x);
		
}

//================================================================================
float LinearFunction::getY(float x) {
	return (slope * x) + yoffset;
}


//================================================================================
LinearFunction LinearFunction::clone() {
	
	LinearFunction res = LinearFunction();
	res.slope = this->slope;
	res.xstart = this->xstart;
	res.yoffset = this->yoffset;
		
	return res;
}




