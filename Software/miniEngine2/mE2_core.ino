/*

    See www.airiclenz.com for more information

    (c) 2013 Airic Lenz, C.A. Church
    
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
//  C O R E   V A R I A B L E S                       //
//                                                    //
////////////////////////////////////////////////////////


// B0 = program is running
// B1 = start immediately
// B2 = jog mode
// B3 = 
// B4 = 
// B5 = 
// B6 = 
// B7 = 
uint8_t core_status;



// B0 = move motor to home pos before program start
// B1 = autosave settings
// B2 = 
// B3 = 
// B4 = 
// B5 = 
// B6 = 
// B7 = 
uint8_t core_settings = B00000010;



uint32_t  core_program_start_time;


// ============================================================================
boolean core_isProgramRunningFlag()             { return isBit(core_status, BIT_0); }
void    core_setProgramRunningFlag()            { setBit(core_status, BIT_0); }
void    core_deleteProgramRunningFlag()         { deleteBit(core_status, BIT_0); }

boolean core_isStartImmediatelyFlag()           { return isBit(core_status, BIT_1); }
void    core_setStartImmediatelyFlag()          { setBit(core_status, BIT_1); }
void    core_deleteStartImmediatelyFlag()       { deleteBit(core_status, BIT_1); }

boolean core_isJogModeFlag()                    { return isBit(core_status, BIT_2); }
void    core_setJogModeFlag()                   { setBit(core_status, BIT_2); }
void    core_deleteJogModeFlag()                { deleteBit(core_status, BIT_2); }

boolean core_isMoveToHomeBeforeStartFlag()      { return isBit(core_settings, BIT_0); }
void    core_setMoveToHomeBeforeStartFlag()     { setBit(core_settings, BIT_0); }
void    core_deleteMoveToHomeBeforeStartFlag()  { deleteBit(core_settings, BIT_0); }
void    core_toggleMoveToHomeBeforeStartFlag()  { toggleBit(core_settings, BIT_0); }


// ============================================================================
// initialize the core parts of the software
// ============================================================================
void core_init() {
  
  // no status flags set so far
  core_status = 0;
    
}



// ============================================================================
// processes some core functionalities
// ============================================================================
void core_process() {
  
  // TODO ??
  
  return;
    
}


// ============================================================================
void core_checkIfProgramDone() {
  
  // did we reach the program end?
  if (cam_getShootCount() >= setup_frame_count) {
          
    // stop the program
    core_stopProgram();
          
    // reset the system phase flag and thus restart the cycle
    system_phase = 0;
    
  }  
  
}



// ============================================================================
// starts the program
// ============================================================================
void core_startProgram() {
    
  // reset the camera shoot count
  cam_resetShootCount();
  // set the program-is-running-flag
  core_setProgramRunningFlag();   
  // repaint the user interface
  uicore_setRepaintFlag();
  uicore_process(); 
    
  
  // check if the motors need to be moved home first and
  // move them home in case "yes"
  core_checkMoveHomeBeforeStart();
  // store the current motor pos as start reference
  motor_storeMotorReferencePositions();
  // define the moves we need to do
  motor_makeKeyframes();
  
  
  
  
  // enable the motors
  motor_powerAll();
  // start the timer...
  motor_startMoveTimer();
  // set the start-immediately-flag
  core_setStartImmediatelyFlag(); 
  // remember the time when we started  
  core_program_start_time = millis();
  system_cycle_start = millis();
  
  
  
  // if we are in one of the following modes,
  // start the curve based move for all motors
  if (isBit(core_mode, SYSTEM_MODE_CONTINUOUS)) {
    
    // start a continuous move if we are in continuous mode
    motor_startContinuousMove();
  }
  
}



// ============================================================================
// stops the program
// ============================================================================
void core_stopProgram() {
  
  // set the program-is-running-flag
  core_deleteProgramRunningFlag();   
 
  // disable all motors on program Stop 
  motor_disableAll(); 
 
  // reset the cycle warning
  system_cycle_too_long = false;
 
  // request a repaint of the user interface
  uicore_setRepaintFlag();

}


// ============================================================================
// next shot needed?
// ============================================================================
void core_checkIfCycleWasToLong() {
  
  // paint a warning if the cycles take too long
  // but only if we are in SMS mode
  if (((system_cycle_start + setup_interval_length) < millis()) &&
      (isBit(core_mode, SYSTEM_MODE_SMS))) {
    
    system_cycle_too_long = true;
    
    uicore_setRepaintFlag();

  }
}


// ============================================================================
// next shot needed?
// ============================================================================
boolean core_isNextCycle() {
  
  // is the cycle over?
  return (core_program_start_time + (setup_interval_length * (uint32_t) cam_getShootCount())) <= millis();
    
}



// ============================================================================
// checks if the motors need to be moved home before start - 
// and moves the motors home if "yes"
// ============================================================================
boolean core_checkMoveHomeBeforeStart() {
  
  // are we requested to move the motor to home before starting?
  if (core_isMoveToHomeBeforeStartFlag()) {
    
    // move all motors home first
    for (int i=0; i<DEF_MOTOR_COUNT; i++) {
      
      #ifdef DEBUG
      Serial.println();
      Serial.println("-------------------------------");
      Serial.print("defining move home ");
      Serial.println(i);
      #endif      
            
      motor_defineMoveToPosition(i, 0, true);  
    }
                   
    // start the moves
    motor_startMovesToPosition();
    
    // wait until all motors reached home
    while (motor_isMoveToPositionRunning()) {
      
      // abort if the select key is pressed
      if (input_isKeyEvent()) {
        
        if (input_getPressedKey() == KEY_1) {
          
          input_clearKeyEvent(); 
          
          return false;  
        }
   
        input_clearKeyEvent();   
        
      } // end: key event
      
    } // end: while motors are moving 
  
  } // end: move home first  
  
  return true;
  
}






// ============================================================================
// sets the new setup style
// ============================================================================
boolean core_setSetupMode(uint8_t new_setup_mode) {
  
  // SETUP_MODE_RUN
  // SETUP_MODE_CYCLE
  // SETUP_MODE_KEYFRAMES
  
  // these combinations are not allowed
  if (isBit(core_mode, SYSTEM_MODE_VIDEO) &&
       (
         isBit(new_setup_mode, SETUP_MODE_CYCLE) ||
         isBit(new_setup_mode, SETUP_MODE_RUN) 
       ) 
     ) {
       
    return false;
  }
    
  // delete the old mode (all possible bits)
  deleteBit(core_mode, B11100000);
    
  // set the new mode
  setBit(core_mode, new_setup_mode);

  return true;

}


// ============================================================================
// sets the new setup mode
// ============================================================================
boolean core_setSystemMode(uint8_t new_system_mode) {
  
  // SYSTEM_MODE_SMS
  // SYSTEM_MODE_CONTINUOUS
  // SYSTEM_MODE_STOPMOTION
  // SYSTEM_MODE_VIDEO
  
  // these combinations are not allowed
  if (isBit(new_system_mode, SYSTEM_MODE_VIDEO) &&
       (
         isBit(core_mode, SETUP_MODE_CYCLE) ||
         isBit(core_mode, SETUP_MODE_RUN) 
       ) 
     ) return false;
  
  // delete the old mode (all possible bits)
  deleteBit(core_mode, B00001111);
  
  // set the new system mode
  setBit(core_mode, new_system_mode);

  return true;

}




// ============================================================================
// checks all the values for the different modes
// ============================================================================
void core_checkValues() {
  
  ////////////////////////////  
  // run setup?
  if (isBit(core_mode, SETUP_MODE_RUN)) {
    
    setup_frame_count     = cam_fps_values[cam_fps_index] * (setup_play_time / 1000.0); 
    setup_interval_length = setup_record_time / (setup_frame_count - 1);
    
  }
  
  ////////////////////////////  
  // cycle setup
  if (isBit(core_mode, SETUP_MODE_CYCLE)) {
    
    // TODO
  }
  
  
  ////////////////////////////  
  // TODO
  
}




// ============================================================================
void core_delay(unsigned int milliseconds) {
  
  unsigned long startTime = millis();

  while ((startTime + milliseconds) > millis()) {
    input_process();       
  } 
    
}
