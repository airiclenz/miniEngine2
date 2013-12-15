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


#ifndef Point_h
#define Point_h



// ============================================================================
// ============================================================================
// ============================================================================
class Point {


	///////////////////////////////////////////
	public:
	///////////////////////////////////////////
		
		float 	x;
		float 	y;
	
		Point(void);
		Point(float, float);
		
		Point operator + (const Point &) const;
		Point operator - (const Point &) const;
		Point operator * (const Point &) const;
		Point operator / (const Point &) const;
		
		Point operator + (const float &) const;
		Point operator - (const float &) const;
		Point operator * (const float &) const;
		Point operator / (const float &) const;
				
		void 	add(Point);
		void 	substract(Point);
		void 	multiply(Point);
		void 	multiply(float);
		void 	divide(float);
		
		Point	clone(void);		
		
		

};

#endif






