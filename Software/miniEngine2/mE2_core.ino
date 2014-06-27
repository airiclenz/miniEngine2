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
uint8_t core_settings = B00000011;



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
bool core_startProgram() {
  
  // if the program is not yet running
  if (!core_isProgramRunningFlag()) {
  
    
    //////////////////////////////////////////////
    // P R O G R A M   I N I T I A L I Z A T I O N 
    ////////////////////////////////////////////// 
    // reset the camera shoot count
    cam_resetShootCount();
    // set the program-is-running-flag
    core_setProgramRunningFlag();   
    // repaint the user interface
    uicore_setRepaintFlag();
    uicore_repaint(true);
    // turn all trigger interrupts on
    trigger_enableAllInterrupts();
    
    
    //////////////////////////////////////////////
    // M O T O R   P R E P A R A T I O N
    //////////////////////////////////////////////
    
    // do we have curves assigned?
    motor_checkIfCurveExists();
    // print a "moving motor to home" message
    uicore_showMessage(225, 226, 226, 1);
    // enable the motors
    motor_powerAll();  
    // check if the motors need to be moved home first and
    // move them home in case "yes"
    core_checkMoveHomeBeforeStart();
    // store the current motor pos as start reference
    motor_storeMotorReferencePositions();
    // define / check the moves we need to do
    motor_checkKeyframes();    
    
    
    //////////////////////////////////////////////
    // S T A R T   T R I G G E R S
    //////////////////////////////////////////////
    // clear the trigger cache for making sure we have not false
    // trigger events cause by enabling the interrupts
    trigger_clearEvents();  
    
    // check triggers
    if (trigger_isStartTriggerDefined()) {
      
      // print a "waiting for trigger" message and show it for
      // at least on millisecond
      uicore_showMessage(220, 221, 222, 1);
  
      // wait until the trigger signal comes
      while (!trigger_isStartTriggered()) {
        
        // check the keys for abort events or
        // if the stop trigger came before the
        // start
        if (input_isKeyEvent() ||
            trigger_isStopTriggered()) {
          
          // clear the key input event buffer
          input_clearKeyEvent();
          
          // stop the program
          core_stopProgram();
          
          // remove all possible meessages from the screen
          uicore_deleteMessageOnScreenFlag();
          // do a full repaint to have a fresh screen
          uicore_repaint(true);
          // remove the repaint flag
          uicore_deleteRepaintFlag();
                  
          // and now leave this function
          return false;
        }
          
      }
          
      // trigger the backlight on the trigger event
      uicore_setBacklight(true);
            
    }
        
        
    //////////////////////////////////////////////
    // R E P A I N T
    //////////////////////////////////////////////
    // remove all possible messages from the screen
    uicore_deleteMessageOnScreenFlag();
    // do a full repaint to have a fresh dashboard
    uicore_repaint(true);
    
    //////////////////////////////////////////////
    // S T A R T
    //////////////////////////////////////////////
        
    
    // if we are in one of the following modes,
    // start the curve based move for all motors
    if (isBit(core_mode, MODE_TIMELAPSE) || 
        isBit(core_mode, MODE_VIDEO)) {
            
      if (isBit(core_move_style, MOVE_STYLE_CONTINUOUS)) {
        
        // check the max speeds...
        if (!motor_checkCurvesMaxSpeedOk()) {

          // stop everything
          core_stopProgram();
          // do a full repaint to have a fresh screen
          // and updated variabled for not painting
          // the screen unneededly
          uicore_repaint(true);
          // remove the repaint flag
          uicore_deleteRepaintFlag();
          // send a "max speed exceeded" message to the user
          uicore_showMessage(227, 228, 229, 3000);
          // leave this function
          return false;
          
        }
        
        // start a continuous move if we are in continuous mode
        motor_startContinuousMove();
      }
      
    }
    
    // set the start-immediately-flag
    core_setStartImmediatelyFlag(); 
    // remember the time when we started  
    core_program_start_time = millis();
    system_cycle_start = millis();
    // start the timer...
    motor_startMoveTimer();
    
  } // end: program running? 
  
  return true;
  
}



// ============================================================================
// stops the program
// ============================================================================
void core_stopProgram() {
  
  // set the program-is-running-flag
  core_deleteProgramRunningFlag();   
 
  // disable all motors on program Stop 
  motor_disableAll(); 
 
  // turn all trigger interrupts off
  trigger_disableAllInterrupts(); 
 
  // clear all remaining trigger events
  trigger_clearEvents();  
 
  // reset the cycle warning
  system_cycle_too_long = false;
 
  // enable the backlight 
  uicore_setBacklight(true); 
 
  // do a full repaint to have everything updated
  uicore_setRepaintFlag();
  //uicore_repaint(true);
    
}

// ============================================================================
// does a basic move of the saved curves
// ============================================================================
void core_doPreview() {
  
  // remeber the current move style
  byte old_style = core_move_style;
  
  // set the move style to continuous
  core_move_style = MOVE_STYLE_CONTINUOUS;

  // enable the motors
  motor_powerAll();  
  
  // start the moves
  motor_startContinuousMove();
  
  // start the motor timer...
  motor_startMoveTimer();

  
  // wait until the preview is done  
  while (
          (motor_isCurveBasedMoveRunning()) &&
          (! input_isKeyEvent())
        ) {
    
    motor_process();
    input_process();
            
  }
  
  // disable all motors on program Stop 
  motor_disableAll(); 
        
  // clear all possible input events;      
  input_clearKeyEvent();      
  
  // re-set the old mode
  core_move_style = old_style;
  
  
}


// ============================================================================
// next shot needed?
// ============================================================================
void core_checkIfCycleWasToLong() {
  
  // paint a warning if the cycles take too long
  // but only if we are in SMS mode
  if (((system_cycle_start + setup_interval_length) < millis()) &&
      (isBit(core_mode, MOVE_STYLE_SMS))) {
    
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
// next shot needed?
// ============================================================================
boolean core_isProgramOver() {
  
  return (core_program_start_time + (setup_record_time * motor_getTimeFactor())) <= millis();
    
}



// ============================================================================
// checks if the motors need to be moved home before start - 
// and moves the motors home if "yes"
// ============================================================================
boolean core_checkMoveHomeBeforeStart() {
  
  /////////////////////////////
  // TIMELAPSE OR VIDEO
  if (isBit(core_mode, MODE_TIMELAPSE) ||
      isBit(core_mode, MODE_VIDEO)) {
    
    if (isBit(core_setup_style, SETUP_STYLE_RUN)) {
      
      // are we requested to move the motor to home before starting?
      if (core_isMoveToHomeBeforeStartFlag()) {
        
        // move all motors home first
        for (int i=0; i<DEF_MOTOR_COUNT; i++) {
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
            
    } // end: setup style run
    
    else if (isBit(core_setup_style, SETUP_STYLE_KEYFRAMES)) {
      
      // move all motors to the position of their start keyframe
      motor_moveToStartKeyframe();
      
    } // end: setup style keyframes
    
  } // end: timelapse / video
  
  
  
  return true;
  
}





// ============================================================================
// checks the system modes for any invalid configurations
// ============================================================================
void core_checkModes() {
  
  if (isBit(core_mode, MODE_VIDEO)) {
    
    if (isBit(core_move_style, MOVE_STYLE_SMS)) {
      core_move_style = MOVE_STYLE_CONTINUOUS;
    }  
    
  }
    
}



// ============================================================================
// checks all the values for the different modes
// ============================================================================
void core_checkValues() {
  
  ////////////////////////////
  // T I M E L A P S E
  if (isBit(core_mode, MODE_TIMELAPSE)) {
      
    setup_frame_count     = cam_fps * (setup_play_time / 1000.0); 
    setup_interval_length = setup_record_time / (setup_frame_count - 1);
    
  } // end: timalpse
  
  
  
  ////////////////////////////  
  // V I D E O

  ////////////////////////////  
  // P A N O R A M A
 
  
}




// ============================================================================
void core_delay(unsigned int milliseconds) {
  
  unsigned long startTime = millis();

  while ((startTime + milliseconds) > millis()) {
    input_process();       
  } 
    
}


// ============================================================================
void core_deleteSettings() {
  
  // remove the old settings file
  sd_deleteSettings(); 
  // show the reset message
  uicore_showMessage(113, 114, 116, 1000);  

  // wait forever until the user removed the power
  while(1){};  
  
}


// ======================================================================================
// Function to restart the Arduino
// ======================================================================================
//core_reset() function:
void(* core_reset) (void) = 0; //declare reset function @ address 0


