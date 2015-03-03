
/*

    Author: Airic Lenz
    
    See www.airiclenz.com for more information

    The 1st version of this code, dealing with core functionalities, 
    was heavily inspired by the OpenMoCo Engine by C.A. Church
    and is basically based on it. Thank you for your great work! 
        
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


////////////////////////////////////////////////////////
//                                                    //
//  I N C L U D E S                                   //
//                                                    //
////////////////////////////////////////////////////////


#include <RotaryEncoder.h>
#include <bitOps.h>


////////////////////////////////////////////////////////
//                                                    //
//  D E F I N I T I O N S                             //
//                                                    //
////////////////////////////////////////////////////////

#define   PIN_ROTARY_A        3 
#define   PIN_ROTARY_B        2  
#define   PIN_ROTARY_PRESS    0  

#define   INT_ROTARY_A        0  // Leonardo: Pin 3
#define   INT_ROTARY_B        1  // Leonardo: Pin 2
#define   INT_ROTARY_PRESS    2  // Leonardo: Pin 0


#define   DEF_INPUT_DEBOUNCE  150


////////////////////////////////////////////////////////
//                                                    //
//  G L O B A L   V A R I A B L E S                   //
//                                                    //
////////////////////////////////////////////////////////

// RotaryEncoder:
RotaryEncoder rotary(PIN_ROTARY_A, PIN_ROTARY_B);

// a variable for button debouncing
unsigned long rotary_press_time = 0;

// a flag for remembering if the button was pressed
boolean rotary_is_pressed = false;

// a flag for remembering if the button was pressed
boolean rotary_is_turned = false;


// ============================================================================
// This function initializes the Rotary encoder
// ===================================================================
void initRotary() {
  
  // set the pin mode of the press button. The pin-modes of the
  // A and B pins are set automatically when creating the rotary pbject
  pinMode(PIN_ROTARY_PRESS, INPUT);
    
  
  // attach interrupts to all the buttons
  // (if you are not using Interrupts, remove the following 3 lines)
  attachInterrupt(INT_ROTARY_PRESS, interruptRotaryPress,  RISING);
  attachInterrupt(INT_ROTARY_A,     interruptRotaryRotate, CHANGE);
  attachInterrupt(INT_ROTARY_B,     interruptRotaryRotate, CHANGE); 
  
}


// ============================================================================
// This function is called when a rotary-interrupt was triggered
// ============================================================================
void interruptRotaryRotate() {
  
  // process the rotary encoder and set the flag for further processing 
  // the encoder's status if we have a new rotary-position
  if (rotary.process()) rotary_is_turned = true;
    
}

// ============================================================================
// This function is called when the rotary-press-interrupt was triggered
// ============================================================================
void interruptRotaryPress() {
  
  // do some software debouncing
  if ((rotary_press_time + DEF_INPUT_DEBOUNCE) < millis()) {
    
    // register the press start time for debouncing
    rotary_press_time = millis();
    
    // set the flag
    rotary_is_pressed = true;
  }
  
}


// ===================================================================
// ===================================================================
// ===================================================================
void setup() {                
  Serial.begin(9600);
  initRotary();
}

// ===================================================================
// ===================================================================
// ===================================================================
void loop() {
 
  // check if the button was pressed
  if (rotary_is_pressed) {
    rotary_is_pressed = false;
    Serial.println("PRESSED!");
  }  
  
  
  // check if the rotary encoder was turned:
  if (rotary_is_turned) {
    rotary_is_turned = false;
    Serial.print("Rotary position: ");
    Serial.println(rotary.getPosition(), DEC);
  }
  
  // if you are not using interrupts, 
  // uncomment the following code block:
  
  /*
  
  if (rotary.process()) {
    rotary_is_turned = true;  
  }
  
  if (digitalRead(PIN_ROTARY_PRESS) == HIGH) {
    rotary_is_pressed = true;
  }
  
  
  */
  
  
}
