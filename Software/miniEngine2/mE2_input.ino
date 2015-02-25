/*

    Author: Airic Lenz
    Year of release: 2015
    
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



#define DEF_INPUT_DEBOUNCE         150
#define DEF_INPUT_MENU_DELAY       500



////////////////////////////////////////////////////////
//                                                    //
//  I N P U T   V A R I A B L E S                     //
//                                                    //
////////////////////////////////////////////////////////


// B0 = rotary position changed
// B1 = rotary key pressed       (key rotary) 
// B2 = select pressed           (key 1 select)
// B3 = edit pressed             (key 2 short)
// B4 = rotary up "pressed"      (key up)
// B5 = rotary down "pressed"    (key down)
// B6 = edit long pressed        (key 2 long)
// B7 = rotary long pressed      (key rotary long)
volatile uint8_t input_status;

volatile boolean edit_down = false;
volatile boolean rotary_key_down = false;


// variables used for soft-debouncing
volatile uint32_t key_1_time, 
                  key_2p_time,
                  key_2r_time,
                  key_rot_p_time,
                  key_rot_r_time;

int16_t rotary_old_pos;  




// ============================================================================
// the main initialization function
// ============================================================================
void input_init() {
  
  // set pin modes
  pinMode(PIN_ROTARY_PRESS, INPUT);
  pinMode(PIN_KEY_1, INPUT);
  pinMode(PIN_KEY_2, INPUT);
  
  
  // attach interrupts for the keys
  attachInterrupt(PIN_ROTARY_PRESS, input_interrupt_keyRotary_press,  RISING);
  attachInterrupt(PIN_KEY_1,        input_interrupt_key1,             RISING);
  attachInterrupt(PIN_KEY_2,        input_interrupt_key2_press,       RISING); 
    
  // attach interrupts to both encoder pins for every state change
  attachInterrupt(PIN_ROTARY_A,     input_interrupt_rotary,           CHANGE);
  attachInterrupt(PIN_ROTARY_B,     input_interrupt_rotary,           CHANGE);
  
    
  // set the initial rotary values & configurations
  rotary.setPosition(0);
  rotary.setKeyMode(true);
  rotary_old_pos = 0;
}



// ============================================================================
// The function that processes the rotary events
// ============================================================================
void input_process() {
  
  ///////////////////////////////////////////////
  // if the program is running and we have actions...
  if (core_isProgramRunningFlag()) {
    
    // when any action occured 
    if (isBit(input_status, B11111111)) {
    
      // reset the rotary position
      rotary.setPosition(rotary_old_pos);    
          
      // clear all bits out of the SELECT button   
      deleteBit(input_status, B11111011);
  
    }
  
  } else {
    
    /////////////////////////////////
    // check long key press of button 2
    if (edit_down && ((key_2p_time + DEF_INPUT_MENU_DELAY) < millis())) {
      
      edit_down = false;  
      // set the KEY2-LONG bit
      setBit(input_status, BIT_6);
      // delete the EDIT bit (in case it was set)
      deleteBit(input_status, BIT_3);

    }
    
    
    /////////////////////////////////
    // check long key press of the rotary button
    if (rotary_key_down && ((key_rot_p_time + DEF_INPUT_MENU_DELAY) < millis())) {
      
      rotary_key_down = false;  
      // set the ROTARY-PRESS-LONG bit
      setBit(input_status, BIT_7);
      // delete the ROTARY-PRESS bit (in case it was set)
      deleteBit(input_status, BIT_1);

    }
    
  }
  
  ///////////////////////////////////////////////
  // there is a rotary (simulated-) key event
  if (rotary.isKeyEvent()) {
    
    if (rotary.isKeyUp())   {
      setBit(input_status, BIT_4);
    }
    
    if (rotary.isKeyDown()) {
      setBit(input_status, BIT_5);
    }
   
    // remove the key-event-flags
    rotary.clearKeyEvents();
  }
  
  
}



// ============================================================================
boolean input_isEvent() {
  
  if (rotary.isKeyMode()) {
    
    // if the program is running send another result
    // (ignore al keys out of the SELECT key)
    if (core_isProgramRunningFlag()) {
      return isBit(input_status, BIT_2);
    } else {
      return isBit(input_status, B11111110);
    }
    
  } else {
    
    // if the program is running send another result
    // (ignore al keys out of the SELECT key)
    if (core_isProgramRunningFlag()) {
      return isBit(input_status, BIT_2);
    } else {
      return isBit(input_status, B11001111);
    }
        
  }  
  
}


// ============================================================================
boolean input_isKeyEvent() {
  
  // if the program is running send another result
  // (ignore all keys out of the SELECT key)
  if (core_isProgramRunningFlag()) {
    return isBit(input_status, BIT_2);
  } else {
    return isBit(input_status, B11111110);
  }

}


// ============================================================================
void input_clearKeyEvent() {
  
  // delete the 7 key bits / flags  
  deleteBit(input_status, B11111110);
}


// ============================================================================
boolean input_isRotaryEvent() {
  
  return isBit(input_status, B00000001);
}


// ============================================================================
void input_clearRotaryEvent() {
  
  rotary_old_pos = rotary.getPosition();
  deleteBit(input_status, BIT_0);
}

// ============================================================================
void input_undoRotaryEvent() {
  
  // reset the rotary position
  rotary.setPosition(rotary_old_pos);
  
  // delete the event flag for rotary action
  deleteBit(input_status, BIT_0);
}





  
// ============================================================================
uint8_t input_getPressedKey() {  
  
  uint8_t result = KEY_NONE;
    
  // KEY 1
  if (isBit(input_status, BIT_2)) {
    setBit(result, KEY_1);
    deleteBit(input_status, BIT_2);
  }

  
  ///////////////////////////////////
  // is the program running?
  // if yes, just end the function if
  // SELECT was pressed...
  if (core_isProgramRunningFlag() &&
      isBit(result, KEY_1)) {
    
    return result;       
  }
  
    
  // ROTARY KEY
  if (isBit(input_status, BIT_1)) {
    
    setBit(result, KEY_ROTARY);
    deleteBit(input_status, BIT_1);
  }
  
  // EDIT
  if (isBit(input_status, BIT_3)) {
    setBit(result, KEY_2);
    deleteBit(input_status, BIT_3);
  }
  
  // UP
  if (isBit(input_status, BIT_4)) {
    setBit(result, KEY_UP);
    deleteBit(input_status, BIT_4);
  }
  
  // DOWN
  if (isBit(input_status, BIT_5)) {
    setBit(result, KEY_DOWN);
    deleteBit(input_status, BIT_5);
  }
  
  // EDIT LONG
  if (isBit(input_status, BIT_6)) {
    setBit(result, KEY_2_LONG);
    deleteBit(input_status, BIT_6);
  }
  
  // ROTARY LONG
  if (isBit(input_status, BIT_7)) {
    setBit(result, KEY_ROTARY_LONG);
    deleteBit(input_status, BIT_7);
  }
  
  return result;
  
}





// ======================================================================================
boolean input_readKeyMenu() {
    
  if (digitalRead(PIN_KEY_2) == HIGH) return true;
  
  return false;
  
}




// ============================================================================
// This function is called when a rotary-interrupt was triggered
// ============================================================================
void input_interrupt_rotary() {
  
  // process the rotary encoder and set the flag for further processing 
  // the encoder's status if we have a new rotary-position
  if (rotary.process()) setBit(input_status, BIT_0);
    
}

// ============================================================================
// This function is called when the rotary button is pressed
// ============================================================================
void input_interrupt_keyRotary_press() {
  
  // attach the new interrupt for button release
  detachInterrupt(PIN_ROTARY_PRESS); 
  attachInterrupt(PIN_ROTARY_PRESS, input_interrupt_keyRotary_release, FALLING); 
  
  
  if ((key_rot_p_time + DEF_INPUT_DEBOUNCE) < millis()) {
    
    // just register the press start time.
    // this button has two functions and the corresponding
    // action will be determined on the release event
    key_rot_p_time = millis();
    
    // remember that edit is down
    rotary_key_down = true;
  }
  
}


// ============================================================================
// This function is called when the rotary key is released
// ============================================================================
void input_interrupt_keyRotary_release() {
  
  // attach the new interrupt for button press
  detachInterrupt(PIN_ROTARY_PRESS); 
  attachInterrupt(PIN_ROTARY_PRESS, input_interrupt_keyRotary_press,   RISING); 
  
  
  if (((key_rot_r_time + DEF_INPUT_DEBOUNCE) < millis()) &&
      ((key_rot_p_time - key_rot_r_time) > DEF_INPUT_DEBOUNCE)) {
    
    // did we release the button before the "menu" long press time?
    if ((key_rot_p_time + DEF_INPUT_MENU_DELAY) > millis()) {
      
      // a regular short press was done
      setBit(input_status, BIT_1);
      key_rot_r_time = millis();
    }
    
    rotary_key_down = false;
  }
  
}



// ============================================================================
// This function is called when the key 1 is pressed
// ============================================================================
void input_interrupt_key1() {
  
  if ((key_1_time + DEF_INPUT_DEBOUNCE) < millis()) {
    
    key_1_time = millis();
    setBit(input_status, BIT_2);
    
  }
  
}

// ============================================================================
// This function is called when the key 2 is presses
// ============================================================================
void input_interrupt_key2_press() {
  
  // attach the new interrupt for button release
  detachInterrupt(PIN_KEY_2); 
  attachInterrupt(PIN_KEY_2, input_interrupt_key2_release, FALLING); 
  
  
  if ((key_2p_time + DEF_INPUT_DEBOUNCE) < millis()) {
    
    // just register the press start time.
    // this button has two functions and the corresponding
    // action will be determined on the release event
    key_2p_time = millis();
    
    // remember that edit is down
    edit_down = true;
  }
  
}


// ============================================================================
// This function is called when the key 2 is released
// ============================================================================
void input_interrupt_key2_release() {
  
  // attach the new interrupt for button press
  detachInterrupt(PIN_KEY_2); 
  attachInterrupt(PIN_KEY_2, input_interrupt_key2_press,   RISING); 
  
  
  if (((key_2r_time + DEF_INPUT_DEBOUNCE) < millis()) &&
      ((key_2p_time - key_2r_time) > DEF_INPUT_DEBOUNCE)) {
    
    // did we release the button before the "menu" long press time?
    if ((key_2p_time + DEF_INPUT_MENU_DELAY) > millis()) {
      
      // a regular short press was done
      setBit(input_status, BIT_3);
      key_2r_time = millis();
    }
    
    edit_down = false;
  }
  
}

