/*

    Author: Airic Lenz, C.A. Church
    Year of release: 2015
    
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


#ifdef __arm__
  #include "itoa.h"
#endif

#include <SPI.h>
#include <SD.h>

#include <UTFT.h>
#include <RotaryEncoder.h>
#include <StepperMotor.h>
#include <MoCoM.h>
#include <bitOps.h>
#include <DueTimer.h>




// free mem stuff
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

extern char _end;
extern "C" char *sbrk(int i);
char *ramstart=(char *)0x20070000;
char *ramend=(char *)0x20088000;



////////////////////////////////////////////////////////
//                                                    //
//  L A N G U A G E                                   //
//                                                    //
////////////////////////////////////////////////////////

#define LANGUANGE_ENGLISH
//#define LANGUANGE_GERMAN



////////////////////////////////////////////////////////
//                                                    //
//  D I S P L A Y   T Y P E                           //
//                                                    //
////////////////////////////////////////////////////////


// if you have an older version of the display, use
// this line of code (if you get a white screen with
// this one, change to the other display type):
#define DISPLAY_TYPE        ITDB24E_16    

// if you have a newer version of the display, use this
// line of code (remove the "//" and add it to the above
// definition):
//#define DISPLAY_TYPE        TFT01_24_16


////////////////////////////////////////////////////////
//                                                    //
//  V E R S I O N                                     //
//                                                    //
////////////////////////////////////////////////////////


#define VERSION                      2
#define SUBVERSION                   0  
#define SUBSUBVERSION                7

const char* STR_VER                  = "PRE";


////////////////////////////////////////////////////////
//                                                    //
//  F O N T S                                         //
//                                                    //
////////////////////////////////////////////////////////


extern uint8_t smallMax[];
extern uint8_t mE_Clear[];
extern uint8_t Sinclair_M[];
extern uint8_t Arial[];


////////////////////////////////////////////////////////
//                                                    //
//  P I N   D E F I N I T I O N S                     //
//                                                    //
////////////////////////////////////////////////////////

#define PIN_BACKLIGHT                7

#define PIN_ROTARY_A                 44
#define PIN_ROTARY_B                 45
#define PIN_ROTARY_PRESS             47


#define PIN_TOUCH_CLK                21
#define PIN_TOUCH_CS                 20
#define PIN_TOUCH_DIN                17
#define PIN_TOUCH_DOUT               16
#define PIN_TOUCH_IRQ                15

#define PIN_TFT_RS                   39 
#define PIN_TFT_WR                   38
#define PIN_TFT_CS                   2 
#define PIN_TFT_RST                  3

#define PIN_KEY_1                    43
#define PIN_KEY_2                    42

#define PIN_CAM_SHUTTER              41
#define PIN_CAM_FOCUS                40
#define PIN_CAM_ALT_SHUTTER          49 
#define PIN_CAM_ALT_FOCUS            46 

#define PIN_MOTOR1_STEP              11 
#define PIN_MOTOR1_DIR               12
#define PIN_MOTOR1_ENABLE            13
#define PIN_MOTOR1_LS1               50 
#define PIN_MOTOR1_LS2               5 

#define PIN_MOTOR2_STEP              8 
#define PIN_MOTOR2_DIR               9
#define PIN_MOTOR2_ENABLE            10
#define PIN_MOTOR2_LS1               51 
#define PIN_MOTOR2_LS2               4


#define PIN_TRIGGER1                 48 
#define PIN_TRIGGER2                 53


#define PIN_BATTERY                  A11        

#define PIN_COM_DIR                  14  

#define PIN_SD_SS                    6


////////////////////////////////////////////////////////
//                                                    //
//  O T H E R   D E F I N I T I O N S                 //
//                                                    //
////////////////////////////////////////////////////////


#define DEF_MOTOR_COUNT                      2


#define  MILLIS_PER_SECOND                1000
#define  MILLIS_PER_MINUTE               60000
#define  MILLIS_PER_HOUR               3600000
#define  MILLIS_PER_DAY               86400000

#define  BYTE_MAX                          255
#define  UINT_MAX                        65535
#define  ULONG_MAX                  4294967295


// time delay after pressing the start button to allow 
// the system to settle down after pressing activating the motors
#define  SYSTEM_START_DELAY              1500L        


////////////////////////////////////////////////////////
//                                                    //
//  S Y S T E M   M O D E S                           //
//                                                    //
////////////////////////////////////////////////////////



// System Modes
#define  MODE_COUNT              3
#define  MODE_TIMELAPSE          B00000001
#define  MODE_VIDEO              B00000010    
#define  MODE_PANORAMA           B00000100    

// setup styles
#define  SETUP_STYLE_COUNT       2
#define  SETUP_STYLE_RUN         B00000001
#define  SETUP_STYLE_KEYFRAMES   B00000010

// move styles
#define  MOVE_STYLE_COUNT        2
#define  MOVE_STYLE_SMS          B00000001    
#define  MOVE_STYLE_CONTINUOUS   B00000010    



uint8_t core_mode         = MODE_TIMELAPSE;
uint8_t core_setup_style  = SETUP_STYLE_RUN;
uint8_t core_move_style   = MOVE_STYLE_SMS;




////////////////////////////////////////////////////////
//                                                    //
//  K E Y F R A M E   C U R V E S                     //
//                                                    //
////////////////////////////////////////////////////////


// the structure used for storing the curve data
typedef struct motorCurve {
  MotorBezier      curve;
  QuadBezierCurve  curveDefinition;
  uint8_t          used;
  
};


// Kurves for keyframes:
// every curve has 200 segments (which consist of 12 byte each) and 9 bytes other stuff
// variables: 
// --> (200 * 12 byte) + 13 bytes = 2413 byte
// --> 2413 byte * 25 = 60325 byte of RAM
#define CURVE_COUNT  25
struct motorCurve mCurves[CURVE_COUNT];
// curves for on demand moves like in SMS or Motor go home:
// --> 2 more curves in the RAM: 60325 byte + (2 * 2413 byte) = 65151 bytes
MotorBezier tempCurves[DEF_MOTOR_COUNT];



////////////////////////////////////////////////////////
//                                                    //
//  G L O B A L   V A R I A B L E S   &   D E F S     //
//                                                    //
////////////////////////////////////////////////////////



// a global variable for the current "time" in milliseconds since the CPU
// was powered up. This is to be used for non-precision events like the 
// display backlight
uint32_t now;


// key codes
#define  KEY_NONE                  B00000000
#define  KEY_ROTARY                B00000001
#define  KEY_1                     B00000010
#define  KEY_UP                    B00000100
#define  KEY_DOWN                  B00001000
#define  KEY_2                     B00010000

#define  KEY_2_LONG                B00100000
#define  KEY_ROTARY_LONG           B01000000


////////////////////////////////////////////////////////
//                                                    //
//  P R O G R A M   V A R A B L E S                   //
//                                                    //
////////////////////////////////////////////////////////



// B0 = camera phase
// B1 = motor phase
// B2 = motor post delay phase
// B3 = 
// B4 = 
// B5 = 
// B6 = 
// B7 = 
uint8_t system_phase = 0;



// a variable to remember when a system phase started;
uint32_t system_cycle_start;

// interval warning (actions during cycle longer than defined cycle)
bool system_cycle_too_long = false;


uint8_t   setup_run_ramp_out[DEF_MOTOR_COUNT]  = { 0  , 0   };
uint8_t   setup_run_ramp_in[DEF_MOTOR_COUNT]   = { 0  , 0   };



uint8_t   cam_fps               = 25;
uint32_t  cam_exposure          = 100;
uint32_t  cam_focus             = 0;
uint32_t  cam_post_delay        = 200;

uint32_t  setup_record_time     = 15 * MILLIS_PER_MINUTE;  // 15 minutes
uint32_t  setup_play_time       = 12 * MILLIS_PER_SECOND;  // 12 seconds

// the amount of photos we need to make during the run
int16_t   setup_frame_count     = cam_fps * (setup_play_time / 1000.0); 
uint32_t  setup_interval_length = setup_record_time / (setup_frame_count - 1);


float     motor_total_distance[DEF_MOTOR_COUNT]= { 0.0, 0.0 };

uint32_t  core_program_start_time;


////////////////////////////////////////////////////////
//                                                    //
//  G L O B A L   O B J E C T S                       //
//                                                    //
////////////////////////////////////////////////////////


// RotaryEncoder:
RotaryEncoder rotary(PIN_ROTARY_A, PIN_ROTARY_B);

// StepperMotor:
StepperMotor motors[DEF_MOTOR_COUNT] = { StepperMotor(PIN_MOTOR1_STEP, PIN_MOTOR1_DIR),
                                         StepperMotor(PIN_MOTOR2_STEP, PIN_MOTOR2_DIR)
                                       };

// COM (for daisy chaingin):
MoCoM com(PIN_COM_DIR, Serial1);


// Display:   
UTFT tft(DISPLAY_TYPE, PIN_TFT_RS, 
                       PIN_TFT_WR, 
                       PIN_TFT_CS, 
                       PIN_TFT_RST);




// ============================================================================
// ============================================================================
// ============================================================================
void setup() {
  
  // used for uplink functionality (miniEngine Studio)
  Serial.begin(19200);
  
  // randomize
  uint16_t a0 = analogRead(A0);
  uint16_t a4 = analogRead(A4);
  uint16_t a7 = analogRead(A7);
  randomSeed((a0 |= a7) &= ~a7);
  
  
  // initialize all components
  core_init();
  uicore_init();
  motor_init();
  input_init();
  com_init();
  cam_init();
  trigger_init();
  //touch_init();
  
    
  //printFreeRam();
  
  
  // paint the splashscreen  
  uipaint_splashScreen();  
  // clear the screen
  tft.clrScr();
  
  
  sd_init();  // also loads the settings
  power_init();
 
  
  // check if all values are calculated correctly after loading
  // the settings from the SD card
  core_checkValues();
  
  
  // do a full repaint
  uicore_repaint(true);
  // enable the backlight
  uicore_setBacklight(true);
  // clear all input events that might have happened so far
  input_clearKeyEvent();
  
  
}




// ============================================================================
// ============================================================================
// ============================================================================
void loop() {
  
  // for avoiding the overhead of leaving the loop function and then 
  // entering it again, we just use an eternal loop
  while(1) {
    
    // what time is it?
    now = millis();
    
    // process the different parts of the software
    input_process();
    uicore_process();
    power_process();
    motor_process();
    com_process();
    cam_process();
    sd_process();
    trigger_process();
    uplink_process();
       
    
    //////////////////////////////////
    if (core_isProgramRunningFlag()) {
            
      ////////////////////////////
      // T I M E L A P S E
      if (isBit(core_mode, MODE_TIMELAPSE)) {
      
        ////////////////////////////
        // did the next cycle start?    
         if (
             (system_phase == 0) &&
             core_isNextCycle()) {
               
          // when did this cycle start?   
          system_cycle_start = millis();   
              
          // set the system status to the camera-phase
          system_phase = BIT_0;    
          
          // trigger the camera  
          cam_start();
                      
        } // end: the next cycle started
        
       
        ////////////////////////////   
        // camera phase
        if (isBit(system_phase, BIT_0)) {
          
          // is the camer phase over?
          if (!cam_isCameraWorking()) {
            
            // check if the program is done
            core_checkIfProgramDone();
            
            // if we are in a curve-based-move mode, just go back to
            // the cycle start
            if (isBit(core_move_style, MOVE_STYLE_CONTINUOUS)) {
              
              // reset the system phase flag and thus restart the cycle
              system_phase = 0;
                          
              // check if we need tp print a warning sign
              core_checkIfCycleWasToLong();
              
            } 
            
            // if we are in a SMS mode, do the motor move stuff now
            else {
              
              // go to the post delay phase 
              // (shift the phase bit one position to the left)
              system_phase = system_phase << 1;
              
              // initiate the motor stuff
              motor_startMotorPhase();
              
            }
                               
            
          } // end: camera is done
           
        } // end: camera phase ended
         
         
         
        ////////////////////////////
        // motor phase
        if (isBit(system_phase, BIT_1)) {
           
          // check if the motor moves are done
          if (!motor_isMoveToPositionRunning()) {
            
            // go to the motor post phase
            system_phase = system_phase << 1;
            
          } 
           
        } // end: motor phase
         
         
        ////////////////////////////
        // motor post delay
        if (isBit(system_phase, BIT_2)) {
         
          // when the motor post delay ended
          if (!motor_isPostDelay()) {
            
            // checks and sets the sleep function if needed          
            motor_startSleep();
            
            // reset the system phase flag and thus restart the cycle
            system_phase = 0;
            
            // check if we need tp print a warning sign
            core_checkIfCycleWasToLong();
            
          
          } // end: motor post delay ended
         
        } // end: motor post delay phase
                    
      } // end: Timelapse mode            
             
             
      ////////////////////////////
      // V I D E O
      else if (isBit(core_mode, MODE_VIDEO)) {      
      
        // if the program time is over
        if (core_isProgramOver()) {
          
          // are we in bouncing mode?
          if (core_isBouncingFlag()) {
            
            // this markes if the following move is backwards or not
            // if this flag is set, the move will be inverted
            core_toggleBouncingMoveFlag();
            
            // invert all the curves
            motor_invertCurves();
            
            // register the new start time
            core_program_start_time = millis();
            
            // start the new move
            motor_startContinuousMove();
            
            
          } else {
            
            // stop the program
            core_stopProgram(true);
            
          }
          
        } // end: is program over? 
        
      } // end: is video mode? 
                     
    } // end: program is running
        
  } // end: while 1 inner eternal loop
    
} // end: function loop









// ============================================================================
void printFreeRam() {

  char *heapend=sbrk(0);
  register char * stack_ptr asm ("sp");
  struct mallinfo mi=mallinfo();

  Serial.println();
  Serial.println();
  Serial.println();
  
  Serial.print(F("Dynamic ram used: ...... "));
  Serial.print(mi.uordblks);
  Serial.println(F(" bytes"));
  
  Serial.print(F("Program static ram used: "));
  Serial.print(&_end - ramstart); 
  Serial.println(F(" bytes"));
  
  Serial.print(F("Stack ram used: ........ "));
  Serial.print(ramend - stack_ptr);
  Serial.println(F(" bytes")); 
 
  Serial.print(F("~ Free mem: ............ "));
  Serial.print(stack_ptr - heapend + mi.fordblks);
  Serial.println(F(" bytes"));
  
  Serial.println();
  Serial.println();
}






