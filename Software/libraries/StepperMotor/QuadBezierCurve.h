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


#ifndef QuadBezierCurve_h
#define QuadBezierCurve_h


#include "Point.h"


// ============================================================================
// ============================================================================
// ============================================================================
class QuadBezierCurve {


	///////////////////////////////////////////
	public:
	///////////////////////////////////////////
		
		Point	p0;
		Point	p1;
		Point	p2;
		Point	p3;
		
		float	xDimension;
		float	yDimension;
		
				
		QuadBezierCurve(void);
		QuadBezierCurve(float, float, float);
		
		void				updateDimension(void);
		void				scale(Point);
		void				shift(Point);
		
		Point 				getPoint(float);
		float 				getPointX(float);
		float 				getPointY(float);
		Point 				getFastPoint(char);
		
		float 				getTFromX(float, float);
		
		QuadBezierCurve		clone(void);
		
};

#endif




