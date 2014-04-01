/*

    Author: Airic Lenz

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

#define TIMER_PERIOD            100         // call the timers every X microseconds 
#define DEF_LS_DEBOUNCE         50          // ms debounce for limit switches
#define DEF_MIN_POS_ERROR       0.00001     // unit: cm/°; used for accuracy check of float variables


#define MOVE_MODE_NONE          0           // this motor is not supposed to move
#define MOVE_MODE_TO_POS	BIT_1       // this motors is using the temp curves for individual moved
#define MOVE_MODE_CURVE         BIT_2       // this motor is doing long moves with multiple curves



// B0 = limit switch - motor 1 - switch 1
// B1 = limit switch - motor 1 - switch 2
// B2 = limit switch - motor 2 - switch 1
// B3 = limit switch - motor 2 - switch 2
// B4 = 
// B5 = 
// B6 = 
// B7 =  
volatile uint8_t motor_ls_status = 0;


// variables used for soft-debouncing of the limit switches
volatile uint32_t lsm11_time,
                  lsm12_time,
                  lsm21_time,
                  lsm22_time;



// B0 = Jog timer running (Timer 1)
// B1 = Jog timer processing (semaphore)
// B2 = Move timer running (Timer 2)
// B3 = Move timer processing (sempahore) 
// B4 = 
// B5 = 
// B6 = 
// B7 =  
volatile uint8_t motor_timer_status = 0;


// B0 = store the move-start time for motor 0
// B1 = store the move-start time for motor 1
// B2 = store the move-start time for motor 2 (unused in this version)
// B3 = store the move-start time for motor 3 (unused in this version)
// B4 = ...
// B5 = 
// B6 = 
// B7 = 
volatile uint8_t motor_time_status = 0;


// B0 = motor-post is active for motor 0
// B1 = motor-post is active for motor 1
// B2 = motor-post is active for motor 2 (unused in this version)
// B3 = motor-post is active for motor 3 (unused in this version)
// B4 = ...
// B5 = 
// B6 = 
// B7 = 
volatile uint8_t motor_post_delay_status = 0;




// the currently selected motor
byte motor_selected = 0;

// the global motor factor for speeding up moves
volatile float motor_time_factor;

// the time used for ramping into- and out of moves (longer = smoother) 
float motor_ramp_time[DEF_MOTOR_COUNT];
 
// turn off the motor after moving
float motor_sleep[DEF_MOTOR_COUNT]; 

// defines the actual method hwo the motor needs to be moved
// >> SMS or Continuous (=Video)
volatile uint8_t motor_move_mode[DEF_MOTOR_COUNT];
	

// an array that stores which of the available curves are used
// by the different motors 
// dimension 1: all motors
// dimension 2: # of the curve in the main curves array (index pointer)
volatile uint8_t motor_used_curves[DEF_MOTOR_COUNT][CURVE_COUNT];

// how much of the used-curves-array did we actually use?
// stis array is used together with motor_used_curves and defines up to 
// where the stack (motor_used_curves) is filled
volatile uint8_t motor_used_curves_count[DEF_MOTOR_COUNT];

// the current curve being used (position in the motor_used_curves array)
// this little array is for running the curves
volatile uint8_t motor_used_curves_index[DEF_MOTOR_COUNT];


// some variables to track the motor direction during moves
//boolean motor_dir, motor2_dir, motor1_dir_old, motor2_dir_old;
//volatile bool  motor_dir[DEF_MOTOR_COUNT];
//volatile bool  motor_dir_old[DEF_MOTOR_COUNT];

// the direction the motor should use in the program
volatile bool  motor_program_direction[DEF_MOTOR_COUNT];

// when whas a motor move started? (in seconds)
volatile float motor_start_time[DEF_MOTOR_COUNT];

// when whas a motor post started? (in system milliseconds)
volatile uint32_t motor_post_start_time[DEF_MOTOR_COUNT];

// where was the motor when the program was started?
volatile float  motor_reference_pos[DEF_MOTOR_COUNT] = { 0.0, 0.0 };


// used timers for checking if the motors need to move.
// When running a timer is called 20000 times per second.


DueTimer timer_move(0);  // --> All curve based motor moves
DueTimer timer_jog(1);   // --> Jogging




// ============================================================================
float motor_getTimeFactor() { return motor_time_factor; }



// ============================================================================
// Motor initialization
// ============================================================================
void motor_init() {
	
  // set the limit switch pins as input
  pinMode(PIN_MOTOR1_LS1, INPUT);
  pinMode(PIN_MOTOR1_LS2, INPUT);
  pinMode(PIN_MOTOR2_LS1, INPUT);
  pinMode(PIN_MOTOR2_LS2, INPUT);

  // enable the internal pull-ups
  digitalWrite(PIN_MOTOR1_LS1, HIGH);
  digitalWrite(PIN_MOTOR1_LS2, HIGH);
  digitalWrite(PIN_MOTOR2_LS1, HIGH);
  digitalWrite(PIN_MOTOR2_LS2, HIGH);
  
  // init the motor ENABLE pins with the state in which the motor is ENABLED
  motors[0].setPinEnable(PIN_MOTOR1_ENABLE, LOW);
  motors[1].setPinEnable(PIN_MOTOR2_ENABLE, LOW);
	
  // attach all the motor-limit-switch interrupts with the state
  // in which they are active
  motor_attachLimitSwitchInterrupts(LOW);
  
  // attach the interrupt function to the jog timer
  timer_jog.attachInterrupt(motor_handleJog);
  // attach the interrupt function to the general motor timer
  timer_move.attachInterrupt(motor_handleMove);
  
  // the time factor to real-time
  motor_time_factor = 1.0;
	

  /////////////////////////////
  // initialize all motor curves
  for (int i=0; i<CURVE_COUNT; i++) {
    mCurves[i].used = false;
  }

  /////////////////////////////
  // initialize all motors with standard values
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
  
    // motor ramp time
    motor_ramp_time[i] = 0.5;
    
    // motor calibration
    motors[i].setCalibration(500);
    
    // motor max speed
    motors[i].setMaxSpeed(20);
    
    // motor sleep
    motor_sleep[i] = false;
    
    // motor move modes
    motor_move_mode[i] = MOVE_MODE_NONE;
    
    // how much curves do we use at the moment? None.
    motor_used_curves_count[i] = 0;
    
    // reset the index in the array to the first position 
    motor_used_curves_index[i] = 0;  
    
    // One curve is always used as we need it to define even the
    // simplet move --> assign an unused curve
    motor_assignNewCurve(i);
          
    // reset the motor move-start times
    motor_start_time[i] = 0;
    
    // direction variables  
    //motor_dir[i] = motors[i].getDirection();  
    //motor_dir_old[i] = motors[i].getDirection();  
  
    // reset the limit switches
    motors[i].resetLimitSwitch();
  }
	
}




// ============================================================================
// Motor processing function for general status checks
// ============================================================================
void motor_process() {
	
  ///////////////////////////////////      
  // check limit switch flags
  if (isBit(motor_ls_status, B00001111)) {
		
    // stop all the timers
    timer_jog.stop();
    timer_move.stop();
		
    // now move the motor a little bit backwards
    if (isBit(motor_ls_status, B00000011)) {
			
      // only retract if the motor is enabled
      if (motors[0].isEnabled()) {
        motors[0].limitSwitchEvent();
	motors[0].resetLimitSwitch();
        
      }
      
    } else {
		
      // only retract if the motor is enabled
      if (motors[1].isEnabled()) {
		  
	motors[1].limitSwitchEvent();
	motors[1].resetLimitSwitch();
      }
	
    }
				
    // delete the limit switch flags
    deleteBit(motor_ls_status, B00001111);
    
    // stop all motor actions and delete official flags
    motor_disableAll();
    motor_stopJog();
    uicore_stopJog();

    // stop the program if it was running...
    // (this also de-poweres all motors and sets the repaint flag)
    core_stopProgram();
	
  }
	
  //////////////////////////////////      
  // no limit switch event
  else {
		
    ///////////////////////////////////  
    // start jog mode if needed
    if ((!isBit(motor_timer_status, BIT_0)) && (core_isJogModeFlag())) {
      motor_startJog(); 
    }
    
    ///////////////////////////////////   
    // stop jog mode if needed 
    if ((isBit(motor_timer_status, BIT_0)) && (!core_isJogModeFlag())) {
      motor_stopJog(); 
    } 
    
    
    // loop all motors
    for (int i=0; i<DEF_MOTOR_COUNT; i++) {
      
      ///////////////////////////////////   
      // check if we need to delete any of the motors post delays:
      if (isBit(motor_post_delay_status, (1 << i))) {
        
        // check if the time is over
        if ((motor_post_start_time[i] + motors[i].getPostDelay()) <= millis()) {
          
          // remove the "delay-is-active" flag
          deleteBit(motor_post_delay_status, (1 << i));
                  
        } // end: post delay time is over  
        
      } // end: post delay is active
      
    } // end: loop all motors
        	
  } // end: no limit switch event
	
}


// ============================================================================
// powers all motors
// ============================================================================
void motor_powerAll() {

  for (int j=0; j<DEF_MOTOR_COUNT; j++) {  
    motors[j].enable();
  }
  
}


// ============================================================================
// sets the motors to sleep with the sleep setting enabled
// ============================================================================
void motor_startSleep() {
  
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
    if (motor_sleep[i]) {
      motors[i].disable();
    }
    
  }
    
}


// ============================================================================
// powers all motors
// ============================================================================
void motor_startMoveTimer() {

  // if the timer is not yet running
  if (!isBit(motor_timer_status, BIT_2)) {
    
    // set the flag that indicated the timer2 is running
    setBit(motor_timer_status, BIT_2);
    
    // start the motor-timer 
    timer_move.start(TIMER_PERIOD);  

  }
  
}



// ============================================================================
// Halt all motor activity
// ============================================================================
void motor_disableAll() {
	
  // stop the timers
  timer_jog.stop();
  timer_move.stop();
	
  // delete the "timer running flags"
  deleteBit(motor_timer_status, BIT_0 || BIT_2 || BIT_3);

	
  // finally disable the motors
  for (int j=0; j<DEF_MOTOR_COUNT; j++) {
    
    // set the motor to status stopp
    motor_move_mode[j] = MOVE_MODE_NONE;
    
    // and disable the motor
    motors[j].disable();
    
  
  }
	
}

// ============================================================================
// Does a basic check if at least one curve is assigned to every motor
// ============================================================================
void motor_checkIfCurveExists() {
 
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
      
    // if not, then assign one curve because even the simplest
    // moves require one curve 
    if (motor_used_curves_count[i] < 1) {
      motor_assignNewCurve(i);
    }
    
  } 
  
}



// ============================================================================
// Assigns a new curve to the motor. The info about that gets stored 
// in the motor specific curve-assignment/addressing-array
// ============================================================================
int16_t motor_assignNewCurve(uint8_t motorNum) {
  
  // which is the next free curve in the main curves array?
  uint8_t nextFreeCurve = motor_getNextFreeCurve();  
  
  if (nextFreeCurve >= 0) {
    
    // make the curve used
    mCurves[nextFreeCurve].used = true;
    
    // store the used curves in our curve-addressing array
    motor_used_curves[motorNum][motor_used_curves_count[motorNum]] = nextFreeCurve;
    
    // count the used curves value one up
    motor_used_curves_count[motorNum]++;  
    
    return nextFreeCurve;
    
  }
  
  return -1;
  
}


// ============================================================================
// Unassigns all curves from the currently selected motor
// ============================================================================
void motor_freeAllCurves(uint8_t motorNum) {
  
  
  // loop all esed curves of the motor
  for (int i=0; i<motor_used_curves_count[motorNum]; i++) {
    
    // set the curve as unused
    mCurves[motor_used_curves[motorNum][i]].used = false;
    
  }
  
  // set the amount of curves used to zero
  motor_used_curves_count[motorNum] = 0;
    
  
}



// ============================================================================
// gets the index of the next free and unused curve
// ============================================================================
uint8_t motor_getNextFreeCurve() {
  
  for (int i=0; i<CURVE_COUNT; i++) {
    
    if (mCurves[i].used == false) {

      return i;
    }  
    
  } 
  
  // in case no curves are free, return -1
  return -1;
  
}





// ============================================================================
// sets the current motor pos as quasi-home (used e.g. as 0-ref when the
// program gets started)
// ============================================================================
void motor_storeMotorReferencePositions() {
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
    motor_reference_pos[i] = motors[i].getMotorPosition();  
  }  
}


// ============================================================================
// starts the motor phase. this means defining the motor target and initiating
// the motor start
// ============================================================================
void motor_startMotorPhase() {
  
  // the variable which will hold the motor position to which the motors needs to move
  float new_motor_pos;
    
  // the current time in seconds
  float x; 
    
  // loop all motors
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
    
    //////////////////////////
    // new approach moving along a curve
        
    // calculate the time since the program started (in seconds)
    // calculate the time for the last cycle start and then add
    // one cycle because the motor needs to be "ahead of the camera" by
    // one cycle
    x = ((float)(system_cycle_start - core_program_start_time + setup_interval_length)) / 1000.0; // * motor_time_factor;
    
    
    // check if we need to move on to the next curve 
    // (because all curves should be sorted in time, we know when a curve is not
    // any longer valid because the current time is bejond the curves-time-span)   
    while ( x > mCurves[motor_used_curves[i][motor_used_curves_index[i]]].curve.getEndX() ) {
      
      // don't use more curves than availabe
      if (motor_used_curves_index[i] < (motor_used_curves_count[i] - 1)) {
        motor_used_curves_index[i]++;  
      } else {
        
        // valid curve available? (it might be that a cycle takes longer than planned.
        // in this case the position for the motor would exceed the defined curves on the
        // x axis when the planned program is reached, but not all cycles are executed yet).
        // when this happens, we cannot do further moves
        return;
      }

    }
      
    // calculate the position where the motor needs to be at this moment in time (in cm / °)
    new_motor_pos = mCurves[motor_used_curves[i][motor_used_curves_index[i]]].curve.getY(x); 
    
    // set the new position
    motor_defineMoveToPosition(i, new_motor_pos, false);  
  
  } // end: loop all motors
      
}



// ============================================================================
// initiates a simple move to a specific position
// smooth: multiply the regular ramp time by 3
// ============================================================================
void motor_defineMoveToPosition(uint8_t mNum, float newPos, bool smooth) {
  	
  // only do this if the motor is not yet in any other move mode
  if (motor_move_mode[mNum] == MOVE_MODE_NONE) {
    
    // if we used the smooth option, multiply the regular ramp time by 3
    // just for this move (used for long move-home-moves for example)
    float ramptime;
    if (smooth) {
      ramptime = motor_ramp_time[mNum] * 3.0;
    } else {
      ramptime = motor_ramp_time[mNum];  
    }
    
    
    QuadBezierCurve curve;
    float fac, rampTime100, totalTime;
    
      
    // calculate the distance after which we need to do a curve with
    // a linear segment (every move shorter than this ditance can be done 
    // as a 100% ramp curve)
    float rampChangeDist = ramptime * motors[mNum].getMaxSpeed();
    
    // calculate the total distances we are asked to move for the motor
    float moveDist = abs(newPos - motors[mNum].getMotorPosition());
    
    
    
    // if we are asked to move at least a tiny bit
    if (moveDist >= DEF_MIN_POS_ERROR) {
    
      // calculate the full time needed for the curve
      totalTime = (ramptime * 2.0) + ((moveDist - rampChangeDist) / motors[mNum].getMaxSpeed());
        
      // is the distance to be moved is bigger than the 100%-ramp-curve-distance:
      if (moveDist > rampChangeDist) {
        
        curve.p0 = Point(0,                    motors[mNum].getMotorPosition());
        curve.p1 = Point(ramptime,             motors[mNum].getMotorPosition());
        curve.p2 = Point(totalTime - ramptime, newPos);
        curve.p3 = Point(totalTime,            newPos);
        
      } 
      // do a 100%-ramp curve
      else {
        
        // calculate the full time needed for a 100% ramp curve
        // when ramping to full speed at 50% distance
        rampTime100 = (moveDist / motors[mNum].getMaxSpeed()) * 2.0; 
        // prevent to fast moves on short distances - 
        // the motor would otherwise always speed up to full speed.
        // this means we need to extend the move-time artificially
        fac = rampTime100 / (ramptime * 2);
        rampTime100 = (((1 - fac) * rampTime100) + totalTime + (fac * totalTime)) / 2.0;
            
        curve.p0 = Point(0,                 motors[mNum].getMotorPosition());
        curve.p1 = Point(rampTime100,       motors[mNum].getMotorPosition());
        curve.p2 = Point(rampTime100,       newPos);
        curve.p3 = Point(rampTime100 * 2.0, newPos);
        
      }
      
        
      // get the min and max values of the curves
      curve.updateDimension();
        
      // convert the just defined Bezier curve into linear segments
      tempCurves[mNum].segmentateCurveOptimized(curve);
      //tempCurves[mNum].segmentateCurve(curve);
             
      // init the motor move 
      tempCurves[mNum].initMove(); 
            
      // set the flag for the current motor that
      // the time will be stored when the timer is process
      // (needed for calculation the position versus time)
      setBit(motor_time_status, (1 << mNum));
      
      // set the move style to use a temporary curve and not the big
      // continuous move array
      motor_move_mode[mNum] = MOVE_MODE_TO_POS;  
     
    } // end: move dist is > 0
           
  } // end: do this kind of move only when...
		
}



// ============================================================================
// dummy function for processing timer 2
// ============================================================================
void motor_startMovesToPosition() {
    
  // enable all the motors
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
    
    // init this motor if needed
    if (isBit(motor_move_mode[i], MOVE_MODE_TO_POS)) {
      
      // jump to the first curve
      motor_used_curves_index[i] = 0;
      
      // enable the motor      
      motors[i].enable();  
      
    }
      
  }
  
  // start the timer
  motor_startMoveTimer();
    
}


// ============================================================================
// check if any of the motors is doing a move to a position
// ============================================================================
boolean motor_isMoveToPositionRunning() {
  
  // loop all motors
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
    
    // init this motor if needed
    if (isBit(motor_move_mode[i], MOVE_MODE_TO_POS)) {
      return true;  
    }
  
  }  
  
  return false;
  
}


// ============================================================================
// starts the motor phase. this means defining the motor target and initiating
// the motor start
// ============================================================================
void motor_startContinuousMove() {

  // loop all motors
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
    
    // set the motor to the start position
    motor_move_mode[i] = MOVE_MODE_CURVE;
    
    // jump to the first curve
    motor_used_curves_index[i] = 0;
    
    // set the flag for the current motor that
    // the time will be stored when the timer is process
    // (needed for calculation the position versus time)
    setBit(motor_time_status, (1 << i));
    
  }
  
}


// ============================================================================
// check if any of the motors is doing a move to a position
// ============================================================================
boolean motor_isCurveBasedMoveRunning() {
  
  // loop all motors
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
    
    // init this motor if needed
    if (isBit(motor_move_mode[i], MOVE_MODE_CURVE)) {
      return true;  
    }
  
  }  
  
  return false;
  
}




// ============================================================================
// returns true if any of the motors is in post delay
// ============================================================================
boolean motor_isPostDelay() {
  return motor_post_delay_status > 0;  
}


// ============================================================================
// Check if we need to create keyframes (when not in Keyframes Mode) or
// if we need to check the runtine (when in Keyfrmes mode)
// ============================================================================
void motor_checkKeyframes() {

  if (isBit(core_mode, MODE_TIMELAPSE) ||
      isBit(core_mode, MODE_VIDEO)) {
    
    ///////////////////////////////////
    // Setup Style   R U N
    if (isBit(core_setup_style, SETUP_STYLE_RUN)) {
     
      motor_makeKeyframes();
     
    } // end: setup style run
    
    ///////////////////////////////////
    // Setup Style   K E Y F R A M E S
    else if (isBit(core_setup_style, SETUP_STYLE_KEYFRAMES)) {
    
      // the variable for our max duration
      uint32_t duration = 0;
      
      // check which motor needs the most time for its curves;
      // loop all motors fir this:
      for (int i=0; i<DEF_MOTOR_COUNT; i++) {
        
        int curveIndex;
        
        // loop all curves
        for (int c=0; c<motor_used_curves_count[i]; c++) {
        
          // and initialize them for the comming move
          curveIndex = motor_used_curves[i][c];
          mCurves[curveIndex].curve.initMove();
        
        } // end: loop all curves
        
        
        // if this motor has curves:
        if (motor_used_curves_count[i] > 0) {
          
          // get the index of the last curve of the current motor  
          curveIndex = motor_used_curves[i][motor_used_curves_count[i] - 1];
          
          // is this curve running longer than your current duration?
          if (mCurves[curveIndex].curve.getEndX() > duration) {
            
            // set the new max value
            duration = mCurves[curveIndex].curve.getEndX();
            
          } // end: new max value
        
        } // end: motor has curves
        
      } // end: loop all motors
      
      // set the program duration to our new value
      setup_record_time = duration * 1000;
            
      // recalculate core values if needed
      core_checkValues();
            
            
    } // end: setup style keyframes
       
  } 

}





// ============================================================================
// creates keyframes for the moves that are not based on keyframes
// ============================================================================
void motor_makeKeyframes() {
  
  QuadBezierCurve curve;
  float curveDuration;
  float newPos;
  
  uint8_t curveIndex;
  
  
  // loop all motors
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
        
    Serial.print("motor ");
    Serial.println(i);
        
    // remove all existing curves for this motor
    motor_freeAllCurves(i);
    
    Serial.print("curve count after free ");
    Serial.println(motor_used_curves_count[i]);
    
    // assign a new curve for this motor
    int res = motor_assignNewCurve(i);
    
    Serial.print("assigned curve index ");
    Serial.println(res);
        
    Serial.print("curve count after assign ");
    Serial.println(motor_used_curves_count[i]);
        
    // the duration of the curve to be defined.
    curveDuration = ((float)setup_record_time) / 1000.0;
    
    // the position to where the motor needs to go
    // (our start point is the motor ref-point - not home! - because we
    // might want to start from a non-home position)
    if (motor_program_direction[i]) {
      newPos = motor_reference_pos[i] - motor_total_distance[i];   
    } else {
      newPos = motor_reference_pos[i] + motor_total_distance[i];
    }
    
    // define a curve (linear when no ramping was defined)
    // the points p1 and p2 are the helper points of the Bezier which define
    // the smoothness and thus the ramping of the curve
    curve.p0 = Point(0,                                                                motor_reference_pos[i]);
    curve.p1 = Point(curveDuration * ((float)        setup_run_ramp_in[i]   / 100.0),  motor_reference_pos[i]);
    curve.p2 = Point(curveDuration * ((float) (100 - setup_run_ramp_out[i]) / 100.0),  newPos);
    curve.p3 = Point(curveDuration,                                                    newPos);
    
    
    // get the min and max values of the curves
    curve.updateDimension();
        
    // get the index of the first used curve in the global
    // curve array
    curveIndex = motor_used_curves[i][0];
    
    Serial.print("curve index ");
    Serial.println(curveIndex);
    
    // convert the just defined Bezier curve into linear segments
    // and store it in the global curve array
    //mCurves[curveIndex].curve.segmentateCurveOptimized(curve);
    mCurves[curveIndex].curve.segmentateCurve(curve); //segmentateCurveOptimized(curve);
           
    // init the motor move 
    mCurves[curveIndex].curve.initMove(); 
    
    Serial.println();
        
  }
  
}




// ============================================================================
// moves all motors to their start-keyframe position
// ============================================================================
boolean motor_moveToStartKeyframe() {
  
  int curveIndex;
  
  Serial.println("moving to start keyframes");
  
  // move all motors home first
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
    
    Serial.print("motor ");
    Serial.println(i);
    
    // if the motor has curves
    if (motor_used_curves_count[i] > 0) {
      
      Serial.print("motor has curves");
      
      // get the index of the first used curve
      curveIndex = motor_used_curves[i][0];
      // get the motor position at time 0
      float pos = mCurves[curveIndex].curve.getY(0);
      
      Serial.print("start pos ");
      Serial.println(pos);
      
      // move the motor to this position
      motor_defineMoveToPosition(i, pos, true);  
       
    } // end: motor has curves defines 
    
  } // end: loop all motors
  
  
  // start the moves
  motor_startMovesToPosition();
  
  Serial.print("moves to positions started");
  
  // wait until all motors reached home
  while (motor_isMoveToPositionRunning()) {
    
    // abort if the select key is pressed
    if (input_isKeyEvent()) {
      
      if (input_getPressedKey() == KEY_1) {
        
        input_clearKeyEvent(); 
        
        return false;  
      
      } // end: user input
 
      input_clearKeyEvent();   
      
    } // end: key event
    
  } // end: while motors are moving 
  
  
  Serial.print("moves to positions done");
  
  return true;
  
}



// ============================================================================
// dummy function for processing timer 2 interrupt
// ============================================================================
void motor_handleMove() {

  // check if we are allready processing this function
  if (!isBit(motor_timer_status, BIT_3)) {
    
    // set the semaphore flag
    setBit(motor_timer_status, BIT_3);
    
    
    float x;                 // current time in seconds since the move started 
    long newPos, stepsToDo;  // where to go and how far is that?
    uint8_t done = 0;        // a variable to check if we are done with all the moves
    
    
    /////////////////////
    // LOOP ALL MOTORS
    for (int i=0; i<DEF_MOTOR_COUNT; i++) {
      
      // do we need to set a new timestamp for a new move?
      if (isBit(motor_time_status, (1 << i))) {
                
        // delete the flag
        deleteBit(motor_time_status, (1 << i));
        // store the current time in seconds  
        motor_start_time[i] = ((float) millis()) / 1000.0;   
        
      }
      
      
      // how much time went by since we started the motors 
      // (the time = x axis on the curve graph)
      x = ((((float) millis()) / 1000.0) - motor_start_time[i]) * motor_time_factor;
            
            
      /////////////////////////////////////
      // S-M-S MODE
      if (motor_move_mode[i] == MOVE_MODE_TO_POS) {
        
        // start only if there is work left for this motor 
        if (x <= tempCurves[i].getEndX()) {
          
          newPos = tempCurves[i].getY(x) * motors[i].getCalibration();
          stepsToDo = newPos - motors[i].getMotorRawPosition();
          
          // are there steps we need to execute?            
          if (stepsToDo != 0) {
            
            // determine in which direction we need to move 
            boolean dir = (boolean)(stepsToDo < 0);
            
            // if the direction changed...
            if (dir != motors[i].getDirection()) {
              
              motors[i].setDirection(dir);
              //motor_dir_old[i] = dir;
            }
          
            stepsToDo = abs(stepsToDo);   
            for (int s=0; s<stepsToDo; s++) {
              motors[i].step();   
              
              // wait a little bit if we need to do more steps
	      if (i<(stepsToDo-1)) delayMicroseconds(1);
            }
          }
          
        } else {
          
          // deactivate this motor
          motor_move_mode[i] = MOVE_MODE_NONE;
          // store the post delay start time
          motor_post_start_time[i] = millis();
          // set the motor-post-delay-is-active-flag for this motore
          setBit(motor_post_delay_status, (1 << i));
          // this motor is ready
          done++;
        }
        
      } // end: SMS mode
      
      /////////////////////////////////////////////////  
      // C O N T I N U O U S   /   V I D E O   M O D E 
      else if (motor_move_mode[i] == MOVE_MODE_CURVE) {
        
        // check if we are still using the correct curve  
        while ( x > mCurves[motor_used_curves[i][motor_used_curves_index[i]]].curve.getEndX() ) {
          
          // don't use more curves than availabe
          if (motor_used_curves_index[i] < (motor_used_curves_count[i] - 1)) {
            
            motor_used_curves_index[i]++;  
            
          } else {
          
            // deactivate this motor
            motor_move_mode[i] = MOVE_MODE_NONE;
            // store the post delay start time
            motor_post_start_time[i] = millis();
            // set the motor-post-delay-is-active-flag for this motore
            setBit(motor_post_delay_status, (1 << i));
            // this motor is ready
            done++;
            
            // leave the while loop
            break;
            
          } // end: exceeded last curve
        
        } // end: move-to-next-curve-check
        
        
        // check if the motor is still turned on into curve-mode
        if (motor_move_mode[i] == MOVE_MODE_CURVE) {
          
          newPos = mCurves[motor_used_curves[i][motor_used_curves_index[i]]].curve.getY(x) * motors[i].getCalibration();
          stepsToDo = newPos - motors[i].getMotorRawPosition();
            
          // are there steps we need to execute?            
          if (stepsToDo != 0) {
            
            // determine in which direction we need to move 
            boolean dir = (boolean)(stepsToDo < 0);
            
            // if the direction changed...
            if (dir != motors[i].getDirection()) {
              
              motors[i].setDirection(dir);
              //motor_dir_old[i] = dir;
            }
          
            stepsToDo = abs(stepsToDo);   
            for (int s=0; s<stepsToDo; s++) {
              motors[i].step();   
              
              // wait a little bit if we need to do more steps
	      if (i<(stepsToDo-1)) delayMicroseconds(1);
            }
          
          } // end: if steps are available
          
        } // end: 2nd check if the motor is still in curve-mode
        
      } // end: CONTINUOUS MODE
      
      /////////////////////////////////////  
      // D E F A U L T
      else {
        // no valid move mode --> this motor is ready  
        done++;  
        
      }
      
    } // end: loop all motors

    
    // if all motors are done, stop everything
    // including the timer - but only if we are not in a program 
    if ((done == DEF_MOTOR_COUNT) &&  
        (!core_isProgramRunningFlag())) {
          
      // turn all motor-action off
      motor_disableAll(); 
      
    }
    
    
    // delete the semaphore flag
    deleteBit(motor_timer_status, BIT_3);
    
  }
  	
}
	
	

	
// ============================================================================
void motor_startJog() {
	
  // only start joggin if no motor move is executed
  if (!isBit(motor_timer_status, BIT_2)) {

    motors[motor_selected].enable();
    motors[motor_selected].startJog();

    // set the jog-timer is enabled flag
    setBit(motor_timer_status, BIT_0);

    // start the jog-timer with a call every 50 microseconds (0.05 millisec = 20k calls per sec)
    timer_jog.start(TIMER_PERIOD);

  }
	
}
	
	
// ============================================================================
void motor_stopJog() {
	
  timer_jog.stop();

  // stop jog for all motors - just in case  
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
    motors[i].stopJog();
  }
  
  // remove the jog-timer is enabled flag
  deleteBit(motor_timer_status, BIT_0);
}


// ============================================================================
// this function is called by a timer interrupt
// ============================================================================
void motor_handleJog() {
	
  // check if we are allready processing this function
  if (!isBit(motor_timer_status, BIT_1)) {
    
    // set the semaphore flag
    setBit(motor_timer_status, BIT_1);
  
    motors[motor_selected].processJog();
        
    // delete the semaphore flag
    deleteBit(motor_timer_status, BIT_1);
  
  }
  
}


// ============================================================================
bool motor_setJogSpeed(float jogSpeed) {
	
  motors[motor_selected].setJogSpeed(jogSpeed);  
  
}


// ============================================================================
void motor_selectMotor(uint8_t value) {
	
  if ((value >= 0) &&
      (value < DEF_MOTOR_COUNT)) {
    	
    motor_selected = value;
  }

}


// ============================================================================
uint8_t motor_getSelectedMotor() {
  return motor_selected;
}


// ============================================================================
void motor_detachLimitSwitchInterrupts() {
	
  detachInterrupt(PIN_MOTOR1_LS1);
  detachInterrupt(PIN_MOTOR1_LS2);
  detachInterrupt(PIN_MOTOR2_LS1);
  detachInterrupt(PIN_MOTOR2_LS2);  
  
}


// ============================================================================
void motor_attachLimitSwitchInterrupts(boolean activeState) {

  // first detach all interrupts (in case they are attached allready)
  motor_detachLimitSwitchInterrupts();
  
  
  // then attach them again fitting their active-state
  if (activeState == LOW) {

    attachInterrupt(PIN_MOTOR1_LS1, motor_interrupt_LSM11, FALLING);
    attachInterrupt(PIN_MOTOR1_LS2, motor_interrupt_LSM12, FALLING);
    attachInterrupt(PIN_MOTOR2_LS1, motor_interrupt_LSM21, FALLING);
    attachInterrupt(PIN_MOTOR2_LS2, motor_interrupt_LSM22, FALLING);
    
  } else {
    
    attachInterrupt(PIN_MOTOR1_LS1, motor_interrupt_LSM11, RISING);
    attachInterrupt(PIN_MOTOR1_LS2, motor_interrupt_LSM12, RISING);
    attachInterrupt(PIN_MOTOR2_LS1, motor_interrupt_LSM21, RISING);
    attachInterrupt(PIN_MOTOR2_LS2, motor_interrupt_LSM22, RISING);
    
  }
   
}


// ============================================================================
void motor_interrupt_LSM11() {
   
  if ((lsm11_time + DEF_LS_DEBOUNCE) < millis()) {
    
    lsm11_time = millis();
    setBit(motor_ls_status, BIT_0);
  }
    
}


// ============================================================================
void motor_interrupt_LSM12() {
   
  if ((lsm12_time + DEF_LS_DEBOUNCE) < millis()) {
    
    lsm12_time = millis();
    setBit(motor_ls_status, BIT_1);
  }
    
}


// ============================================================================
void motor_interrupt_LSM21() {
   
  if ((lsm21_time + DEF_LS_DEBOUNCE) < millis()) {
    
    lsm21_time = millis();
    setBit(motor_ls_status, BIT_2);
  }
    
}


// ============================================================================
void motor_interrupt_LSM22() {
   
  if ((lsm22_time + DEF_LS_DEBOUNCE) < millis()) {
    
    lsm22_time = millis();
    setBit(motor_ls_status, BIT_3);
  }
    
}


