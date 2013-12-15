/*

    See www.airiclenz.com for more information

    (c) 2013 Airic Lenz
        
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
//  T R I G G E R   D E F S   &   V A R I A B L E S   //
//                                                    //
////////////////////////////////////////////////////////


#define DEF_TRIGGER_DEBOUNCE         10



// B0 = trigger 1 active
// B1 = trigger 2 active
// B2 = 
// B3 = 
// B4 = 
// B5 = 
// B6 = 
// B7 = 
volatile uint8_t trigger_status;


// variables used for soft-debouncing
volatile uint32_t  trigger1_action, 
                   trigger2_action;




// ============================================================================
// initializes the external trigger stuff 
// ============================================================================
void trigger_init() {

  // set the trigger pins as input
  pinMode(PIN_TRIGGER1, INPUT);
  pinMode(PIN_TRIGGER2, INPUT);
  
  //enable the internal pull ups
  digitalWrite(PIN_TRIGGER1, HIGH);
  digitalWrite(PIN_TRIGGER2, HIGH);
  
  // attach the interrupts
  attachInterrupt(PIN_TRIGGER1, trigger_event1, FALLING);
  attachInterrupt(PIN_TRIGGER2, trigger_event2, FALLING);
  
}


// ============================================================================
// processes the external trigger
// ============================================================================
void trigger_process() {
  
  if ((isBit(trigger_status, BIT_0)) &&
      (digitalRead(PIN_TRIGGER1) == LOW)) {
    
    // remove the trigger flag
    deleteBit(trigger_status, BIT_0);
  }
  
  if ((isBit(trigger_status, BIT_1)) &&
      (digitalRead(PIN_TRIGGER2) == LOW)) {
    
    // remove the trigger flag
    deleteBit(trigger_status, BIT_1);
  }
  
}



// ============================================================================
// is called when trigger 1 gets triggered
// ============================================================================
void trigger_event1() {
  
  if ((trigger1_action + DEF_TRIGGER_DEBOUNCE) < millis()) {
    
    trigger1_action = millis();
    setBit(trigger_status, BIT_0);
    
  }
  
}


// ============================================================================
// is called when trigger 2 gets triggered
// ============================================================================
void trigger_event2() {
  
  if ((trigger2_action + DEF_TRIGGER_DEBOUNCE) < millis()) {
    
    trigger2_action = millis();
    setBit(trigger_status, BIT_1);
    
  }
  
}

