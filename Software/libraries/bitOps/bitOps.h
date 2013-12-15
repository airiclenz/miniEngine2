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


#ifndef bitOps_h
#define bitOps_h


//#include <Arduino.h>


////////////////////////////////////////////////
//                                            //
//  B I T    O P E R A T I O N   M A C R O S  //
//                                            //
////////////////////////////////////////////////


// THE BIT OPERATION MACROS
#define isBit(v, b)     ((v & b) != 0) 
#define setBit(v, b)    (v |= b)
#define toggleBit(v, b) (v ^= b)
#define deleteBit(v, b) (v &= ~b)

// THE BITS FOR USAGE IN BITOPERATION MACROS
const uint8_t BIT_0 = B00000001;
const uint8_t BIT_1 = B00000010;
const uint8_t BIT_2 = B00000100;
const uint8_t BIT_3 = B00001000;
const uint8_t BIT_4 = B00010000;
const uint8_t BIT_5 = B00100000;
const uint8_t BIT_6 = B01000000;
const uint8_t BIT_7 = B10000000;


#define shiftPack2Bytes(d1, d2) 			((uint32_t) d1 <<  8) + d2
#define shiftPack3Bytes(d1, d2, d3) 		((uint32_t) d1 << 16) + ((uint32_t) d2 <<  8) + d3
#define shiftPack4Bytes(d1, d2, d3, d4) 	((uint32_t) d1 << 24) + ((uint32_t) d2 << 16) + ((uint32_t) d3 <<  8) + d4


#endif