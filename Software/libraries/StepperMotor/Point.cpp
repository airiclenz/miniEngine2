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



#include "Point.h"



// ============================================================================
Point::Point() {
	
	x = 0;
	y = 0;

}

// ============================================================================
Point::Point(float x, float y) {
	
	this->x = x;
	this->y = y;

}


// ============================================================================
Point Point::operator + (const Point &rhs) const {
	return Point(x + rhs.x, y + rhs.y);
}
Point Point::operator + (const float &value) const {
	return Point(x + value, y + value);
}

// ============================================================================
Point Point::operator - (const Point &rhs) const {
	return Point(x - rhs.x, y - rhs.y);
}
Point Point::operator - (const float &value) const {
	return Point(x - value, y - value);
}

// ============================================================================
Point Point::operator * (const Point &rhs) const {
	return Point(x * rhs.x, y * rhs.y);
}
Point Point::operator * (const float &value) const {
	return Point(x * value, y * value);
}

// ============================================================================
Point Point::operator / (const Point &rhs) const {
	return Point(x / rhs.x, y / rhs.y);
}
Point Point::operator / (const float &value) const {
	return Point(x / value, y / value);
}


// ============================================================================
void Point::add(Point p) {
	
	x += p.x;
	y += p.y;
}

// ============================================================================
void Point::substract(Point p) {
	
	x -= p.x;
	y -= p.y;
}


// ============================================================================
void Point::multiply(float value) {
	
	x *= value;
	y *= value;
}

// ============================================================================
void Point::multiply(Point vector) {
	
	x *= vector.x;
	y *= vector.y;

}

// ============================================================================
void Point::divide(float value) {
	
	x /= value;
	y /= value;

}


// ============================================================================
Point Point::clone() {
	
	return Point(this->x, this->y);

}




