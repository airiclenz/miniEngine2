/*

    Author: Airic Lenz
    Year of release: 2016
    
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



////////////////////////////////////////////////////////
//                                                    //
//  T R I G G E R   D E F S   &   V A R I A B L E S   //
//                                                    //
////////////////////////////////////////////////////////


// debouncing delay in milliseconds
#define DEF_TRIGGER_DEBOUNCE         150



// variables used for soft-debouncing
volatile uint32_t  trigger1_action, 
                   trigger2_action;


#define ACTION_START  0
#define ACTION_STOP   1
#define ACTION_SHOOT  2


typedef struct trigger_action {
  byte    action;
  char    *name;
};

#define  TRIGGER_ACTION_COUNT  3
trigger_action trigger_actions[TRIGGER_ACTION_COUNT] = {
  {
    ACTION_START,
    "Start"
  },  
  
  {
    ACTION_STOP,
    "Stop"
  },  
  
  {
    ACTION_SHOOT,
    "Shoot"
  } 
    
};



// B0 - Trigger 1 triggered
// B1 - Trigger 1 triggered
// B2 - 
// B3 - 
// B4 - 
// B5 - 
// B6 - 
// B7 - 

byte trigger_status = 0;


typedef struct trigger {
  boolean   enabled;
  byte	    type;
  byte      action;
  boolean   debounce;
};

#define  DEF_TRIGGER_COUNT  2
trigger trigger_triggers[DEF_TRIGGER_COUNT] = { 
  {
    false,
    FALLING,
    ACTION_START,
    true
  },

  {
    false,
    FALLING,
    ACTION_STOP,
    true
  }
};

// the currently selected trigger
byte trigger_selected = 0;


// ============================================================================
byte trigger_getTriggercount()    { return DEF_TRIGGER_COUNT; }



// ============================================================================
// initializes the external trigger stuff 
// ============================================================================
void trigger_init() {

  // set the trigger pins as input
  pinMode(PIN_TRIGGER1, INPUT);
  pinMode(PIN_TRIGGER2, INPUT);
    
}


// ============================================================================
// processes the external trigger
// ============================================================================
void trigger_process() {
    
  // do we have a trigger event on triggers 1?
  if (isBit(trigger_status, BIT_0)) {
   
    switch(trigger_triggers[0].action) {
    
      case ACTION_STOP:   {
                            // enable the backlight
                            uicore_setBacklight(true); 
                           
                            // set the repaint flag
                            uicore_setRepaintFlag();
                          
                            // stop the program  
                            core_stopProgram(com.isMaster() && (com.getSlaveCount() > 0));
                            break;
                          } 
                        
      case ACTION_SHOOT:  {
                            cam_start();
                            break;
                          } 
    } 
    
    // delete the trigger events:
    trigger_status = 0;
  } 
  
  // do we have a trigger event on triggers 2?
  else if (isBit(trigger_status, BIT_1)) {
   
    switch(trigger_triggers[1].action) {
    
      case ACTION_STOP:   {
                            // enable the backlight
                            uicore_setBacklight(true); 
                           
                            // set the repaint flag
                            uicore_setRepaintFlag();
                          
                            // stop the program  
                            core_stopProgram(com.isMaster() && (com.getSlaveCount() > 0));
                            break;
                          } 
                        
      case ACTION_SHOOT:  {
                            cam_start();
                            break;
                          } 
    } 
    
    // delete the trigger events:
    trigger_status = 0; 
   
  }   
  
}



// ============================================================================
void trigger_setTriggerType(byte index, byte type) {
	
  // store the type
  trigger_triggers[index].type = type;
}


// ============================================================================
void trigger_setTriggerAction(byte index, byte action) {
  
  trigger_triggers[index].action = action;    
}


// ============================================================================
void trigger_setEnabled(byte index, boolean state) {

  // remember the state
  trigger_triggers[index].enabled = state;

}



// ============================================================================
void trigger_setDebounce(byte index, boolean state) {

  // remember the state
  trigger_triggers[index].debounce = state;

}


// ============================================================================
void trigger_enableAllInterrupts() {
  
  ////////////////////////
  // Trigger 1
  if (trigger_triggers[0].enabled) { 
                  
    // first detach old interrupts
    detachInterrupt(PIN_TRIGGER1); 

    if (trigger_triggers[0].type == FALLING) {
      digitalWrite(PIN_TRIGGER1, HIGH);
    } else {
      digitalWrite(PIN_TRIGGER1, LOW);
    }
    
    // set the new interrupt
    attachInterrupt(PIN_TRIGGER1, trigger_interrupt_1, trigger_triggers[0].type);
    
  } else {
    detachInterrupt(PIN_TRIGGER1);
    digitalWrite(PIN_TRIGGER1, LOW); 
  }
  
  ////////////////////////
  // Trigger 2
  if (trigger_triggers[1].enabled) { 
    
    // first detach old interrupts
    detachInterrupt(PIN_TRIGGER2); 

    if (trigger_triggers[1].type == FALLING) {
      digitalWrite(PIN_TRIGGER2, HIGH);  
    } else {
      digitalWrite(PIN_TRIGGER2, LOW);
    }

    // set the new interrupt
    attachInterrupt(PIN_TRIGGER2, trigger_interrupt_2, trigger_triggers[1].type);

  } else {
    detachInterrupt(PIN_TRIGGER2); 
    digitalWrite(PIN_TRIGGER2, LOW);
  }
  
  
}

// ============================================================================
void trigger_disableAllInterrupts() {
  
  ////////////////////////
  // Trigger 1
  detachInterrupt(PIN_TRIGGER1);
  digitalWrite(PIN_TRIGGER1, LOW); 
  
  ////////////////////////
  // Trigger 2
  detachInterrupt(PIN_TRIGGER2); 
  digitalWrite(PIN_TRIGGER2, LOW);
  
}



// ============================================================================
void trigger_clearEvents() {
  
  // clear the trigger event bugger     
  trigger_status = 0;
  
}



// ============================================================================
boolean trigger_isStartTriggerDefined() {
  
  // loop all triggers
  for (int i=0; i<DEF_TRIGGER_COUNT; i++) {
    
    // is the trigger action "start" and is the trigger enebaled?
    if ((trigger_triggers[i].action == ACTION_START) &&
        (trigger_triggers[i].enabled == true)) {
      
      return true;
          
    }
  }
  
  return false;
  
}


// ============================================================================
boolean trigger_isStartTriggered() {
  
  // do we have a trigger event on one of the triggers?
  if (isBit(trigger_status, BIT_0) &&
      (trigger_triggers[0].action == ACTION_START)) {
    return true;
  } else if (isBit(trigger_status, BIT_1) &&
      (trigger_triggers[1].action == ACTION_START)) {
    return true;
  }  
  
  return false;
}


// ============================================================================
boolean trigger_isStopTriggered() {
  
  // do we have a trigger event on one of the triggers?
  if (isBit(trigger_status, BIT_0) &&
      (trigger_triggers[0].action == ACTION_STOP)) {
    return true;
  } else if (isBit(trigger_status, BIT_1) &&
      (trigger_triggers[1].action == ACTION_STOP)) {
    return true;
  }  
  
  return false;
}






// ============================================================================
boolean trigger_isTriggerEnabled(byte index) {
  return trigger_triggers[index].enabled; 
}

// ============================================================================
boolean trigger_isTriggerDebounce(byte index) {
  return trigger_triggers[index].debounce; 
}

// ============================================================================
byte trigger_getTriggerType(byte index) {
  return trigger_triggers[index].type; 
}

// ============================================================================
byte trigger_getTriggerAction(byte index) {
  return trigger_triggers[index].action; 
}




// ============================================================================
void trigger_interrupt_1() {
  
  if (trigger_triggers[0].debounce) {
    
    if ((trigger1_action + DEF_TRIGGER_DEBOUNCE) < millis()) {
      trigger1_action = millis();
      setBit(trigger_status, BIT_0);
    }  
    
  } else {
    
    setBit(trigger_status, BIT_1);
  }
	
}

// ============================================================================
void trigger_interrupt_2() {
	
  if (trigger_triggers[1].debounce) {
    
    if ((trigger2_action + DEF_TRIGGER_DEBOUNCE) < millis()) {
      trigger2_action = millis();
      setBit(trigger_status, BIT_1);
    }  
    
  } else {
    
    setBit(trigger_status, BIT_1);
  }
      
	
}












