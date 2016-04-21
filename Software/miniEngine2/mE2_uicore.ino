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
//  U I   D E F I N I T I O N S                       //
//                                                    //
////////////////////////////////////////////////////////


#if defined(__arm__)
  // values for the 3.3V Arduino DUE
  #define BACKLIGHT_NORM                   193        // the PWM value where the display-backlight is at 100%
  #define BACKLIGHT_MAX_PROCENT            132        // the max percent-value that can be set as backlight level
#else
  // 5V values for avr based Arduinos
  #define BACKLIGHT_NORM                   128        // the PWM value where the display-backlight is at 100%
  #define BACKLIGHT_MAX_PROCENT            200        // the max percent-value that can be set as backlight level
#endif


#define MENU_MAX_LINES                     40
#define TIMER_CYCLE                        100        // when the timer is active, it gets called every n ms 

// the max number of lines that can be displayed on one menu screen
#define header_height                      60   // height of header in px
#define menu_screen_line_height            30   // height of each menu line in px 
#define menu_max_screen_lines              6    // amount of line that can be displayed at once


////////////////////////////////////////////////////////
//                                                    //
//  F O N T S                                         //
//                                                    //
////////////////////////////////////////////////////////


#define SMALL_FONT   smallMax
//#define DEFAULT_FONT mediumMax || Sinclair_M

// the structure used for storing the curve data
typedef struct fonts {
  char      *name;
  uint8_t*  font;
};

#define  FONT_COUNT  3
fonts uicore_fonts[FONT_COUNT] = { 
  {
    "mE-Clear",
    mE_Clear
  },

  {
    "Sinclair",
    Sinclair_M
  },

  {
    "Arial",
    Arial
  },  
};

// the currently selected font
uint8_t   uicore_font_index = 0;
uint8_t   uicore_font_index_old = 0;



////////////////////////////////////////////////////////
//                                                    //
//  C O L O R   S C H E M E S                         //
//                                                    //
////////////////////////////////////////////////////////




// the structure used for a color scheme
typedef struct colorScheme {
  
  char      *name;
  uint16_t  background;
  uint16_t  background_header;
  uint16_t  scrollbar;
  uint16_t  seperator;

  uint16_t  font;
  uint16_t  font_disabled;
  uint16_t  font_soft;
  uint16_t  font_header;
  uint16_t  font_dashboard;
  uint16_t  font_bg_even;
  uint16_t  font_bg_odd;

  uint16_t  font_selected;
  uint16_t  font_bg_selected;  
  
  uint16_t  font_menu_selected;
  
};



// How many color schemes are defined?
const uint8_t uicore_color_scheme_count = 3;

// our menu tree
struct colorScheme color_schemes[uicore_color_scheme_count] = {
    
  // COLOR SCHEME 0
  { "Default",                          // name (max 8 character)
    uicore_getRGB565(  0,   0,   0),    // background 
    uicore_getRGB565( 90,  80,  60),    // background_header
    uicore_getRGB565(230, 220, 200),    // scrollbar
    uicore_getRGB565(150, 140, 130),    // seperator
    uicore_getRGB565(255, 250, 240),    // font
    uicore_getRGB565(155, 150, 140),    // font_disabled
    uicore_getRGB565(200, 195, 185),    // font_soft
    uicore_getRGB565(255, 255, 220),    // font_header
    uicore_getRGB565(215, 235, 255),    // font_dashboard
    uicore_getRGB565(  0,   0,   0),    // font_bg_even
    uicore_getRGB565( 50,  40,  30),    // font_bg_odd
    uicore_getRGB565( 40,  45, 120),    // font_selected
    uicore_getRGB565(160, 165, 235),    // font_bg_selected
    uicore_getRGB565(  0,   0,   0),    // font_menu_selected
  },
  
  // COLOR SCHEME 1
  { "Night",                            // name (max 8 character)
    uicore_getRGB565(  0,   0,   0),    // background 
    uicore_getRGB565( 80,   0,   0),    // background_header
    uicore_getRGB565(200,   0,   0),    // scrollbar
    uicore_getRGB565(100,   0,   0),    // seperator
    uicore_getRGB565(255,   0,   0),    // font
    uicore_getRGB565(170,   0,   0),    // font_disabled
    uicore_getRGB565(220,   0,   0),    // font_soft
    uicore_getRGB565(200,   0,   0),    // font_header
    uicore_getRGB565(225,   0,   0),    // font_dashboard
    uicore_getRGB565(  0,   0,   0),    // font_bg_even
    uicore_getRGB565( 70,   0,   0),    // font_bg_odd
    uicore_getRGB565(  0,   0,   0),    // font_selected
    uicore_getRGB565(255,   0,   0),    // font_bg_selected
    uicore_getRGB565(  0,   0,   0),    // font_menu_selected
  },
  
  // COLOR SCHEME 2
  { "Contrast",                         // name (max 8 character)
    uicore_getRGB565(  0,   0,   0),    // background
    uicore_getRGB565( 40,  40,  40),    // background_header 
    uicore_getRGB565(255, 255, 255),    // scrollbar
    uicore_getRGB565(255, 255, 255),    // seperator
    uicore_getRGB565(255, 255, 255),    // font
    uicore_getRGB565(150, 150, 150),    // font_disabled
    uicore_getRGB565(235, 235, 235),    // font_soft
    uicore_getRGB565(255, 255, 255),    // font_header
    uicore_getRGB565(255, 255, 255),    // font_dashboard
    uicore_getRGB565(  0,   0,   0),    // font_bg_even
    uicore_getRGB565( 60,  60,  60),    // font_bg_odd
    uicore_getRGB565(  0,   0,   0),    // font_selected
    uicore_getRGB565(255, 255, 255),    // font_bg_selected
    uicore_getRGB565(  0,   0,   0),    // font_menu_selected
  }
    
};


// the selected color scheme
uint8_t uicore_col_scheme = 0; 
uint8_t uicore_col_scheme_old = 0;





////////////////////////////////////////////////////////
//                                                    //
//  U I   V A R I A B L E S                           //
//                                                    //
////////////////////////////////////////////////////////




// B0 = backlight flag (ON / OFF)
// B1 = repaint (general repaint needed)
// B2 = 2nd repaint needed
// B3 = repaint battery
// B4 = repaint shoot count
// B5 = 
// B6 = message on screen
// B7 = pop-up menu on screen
uint8_t uicore_status = 0;

// a variable to track general ui-status changes
uint8_t uicore_status_old = 0;


// the backlight level we are using 
uint8_t uicore_backlight_level;
// the time after which the backlight should turn off in seconds
uint32_t uicore_backlight_time;
// the time the backlight was activated the last time
uint32_t uicore_action_time;


// variables to store the display dimensions
uint16_t display_width, display_height;


// content of the menus (we initialize the array with some items - 
// fitting the longest possible menu.
char lines[32][MENU_MAX_LINES]; 
char data_line[255];
char temp[255];

// help array for the lines to know which value corresponds to which
// line. this is filled by the item_codes when loading the menu content 
uint8_t line_codes[MENU_MAX_LINES]; 

// help array for knowing which lines are disabled
boolean lines_disabled[MENU_MAX_LINES]; 


// position of the cursor on the screen. this is needed to paint verticall scrolling
uint8_t menu_screen_pos = 0;
// position in the menu 
uint8_t menu_pos = 0;
// count of the current menu/text lines
uint8_t menu_length;


// position in the pop-up-menu 
uint8_t popup_menu_pos = 0;
// count of the current pop-up-menu/text lines
uint8_t popup_menu_length;


// how detailed are are editing a value right now? ( stepsize 1 / granularity) 
uint16_t edit_granularity = 10;

// main position in our UI.
uint8_t screen_code;
uint8_t screen_code_old = 255;
// are we editing an item right now?
boolean menu_editing     = false;
boolean menu_editing_old = false;
// program status changed?
boolean program_is_running_old = false;
// jog-mode change?
boolean core_is_jog_mode_old = false;
// cam shoot count
uint32_t cam_shoot_count_old = 0;
// cycle length warning change?
boolean system_cycle_too_long_old = false;

// hold the lates pressed keys locally
uint8_t key;


// the time a message was displayed
uint32_t uicore_message_start_time;
// the duration for which a message should be displayed (in ms)
uint32_t uicore_message_duration;


////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                            //
// T H E   M A I N   M E N U   T R E E                                                        //
//                                                                                            // 
// screen IDs (left side in the array):                                                       //       
// ------------------------------------                                                       //
// 10:  SMS            run setup         >> Timelapse                                         //
// 11:  SMS            keyframes         >> Timelapse                                         //
// 20:  Continuous     run setup         >> Timelapse                                         //
// 21:  Continuous     keyframes         >> Timelapse                                         //
// 30:  Video          run setup         >> Video                                             //
// 31:  Video          keyframes         >> Video                                             //
// 40:  Panorama       n.a.              >> Panorama                                          //
//                                                                                            //
//                                                                                            //
// 100: Settings GENeral                                                                      //
// 101: Settings CAMera                                                                       //
// 102: Settings MOTor                                                                        //
// 103: Settings CHAIN                                                                        //
// 104: Settings TRIGGER                                                                      //
//                                                                                            //
//                                                                                            //
// 200: popup menu                                                                            //
//                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////

// the structure used for our menu relations
typedef struct uiRelation {
   uint8_t   screen;
   uint16_t  menu_item;
};

// amount of menu entires
const uint8_t uicore_content_relation_count = 86;

// our menu tree
struct uiRelation ui_content_relations[uicore_content_relation_count] = {
    
  {  10, 202 }, {  10, 203 }, {  10, 204 }, {  10, 205 }, {  10, 140 }, {  10, 200 }, {  10, 201 }, {  10, 206 }, {  10, 207 }, {  10, 208 }, 
  {  11, 202 }, {  11, 203 }, {  11, 204 }, {  11, 205 }, {  11, 140 }, {  11, 206 }, {  11, 207 }, {  11, 208 }, 
  {  20, 202 }, {  20, 203 }, {  20, 204 }, {  20, 205 }, {  20, 140 }, {  20, 200 }, {  20, 201 }, {  20, 206 }, {  20, 207 }, {  20, 208 }, 
  {  21, 202 }, {  21, 203 }, {  21, 204 }, {  21, 205 }, {  21, 140 }, {  21, 206 }, {  21, 207 }, {  21, 208 }, 
  {  30, 203 }, {  30, 140 }, {  30, 200 }, {  30, 201 }, {  30, 206 }, {  30, 207 },
  {  31,  23 },
  {  40, 210 }, {  40, 211 },
  
    
  { 100, 108 }, { 100, 109 }, { 100, 111 }, { 100, 113 }, { 100, 114 }, { 100, 100 }, { 100, 101 }, { 100, 102 }, { 100, 107 }, { 100, 103 }, { 100, 110 }, { 100, 112 },
  { 101, 120 }, { 101, 121 }, { 101, 124 }, { 101, 122 }, 
  { 102, 140 }, { 102, 150 }, { 102, 154 }, { 102, 141 }, { 102, 142 }, { 102, 143 }, { 102, 144 }, { 102, 145 }, { 102, 161 }, { 102, 146 }, { 102, 151 }, { 102, 155 }, /* { 102, 147 }, */   
    
  { 103, 165 }, { 103, 168 }, 
  { 104, 180 }, { 104, 181 }, { 104, 182 }, { 104, 183 }, { 104, 187 },
  
  
  { 200, 152 }, { 200, 148 }, { 200, 149 }, { 200, 156 }, { 200, 157 }, { 200, 160 },  
    
};

////////////////////////////////////////////////////////
//                                                    //
//  G E N E R A L   S T R I N G   D E F S             //
//                                                    //
////////////////////////////////////////////////////////


const char  STR_LINEFEED    = '\n'; 
const char* STR_EMPTY       = ""; 
const char* STR_SPACE1      = " ";
const char* STR_SPACE2      = "  ";
const char* STR_SPACE3      = "   ";
const char* STR_POINT       = ".";
const char* STR_DPOINT      = ":";
const char* STR_NULL        = "0";
const char* STR_RECBRACKOP  = "[";
const char* STR_RECBRACKCL  = "]";
const char* STR_SLASH       = "/";
const char* STR_EXCLAMATION = "!";
const char* STR_PERCENT     = "%";
const char* STR_NONE        = "---";









// ============================================================================
// status fuctions
// ============================================================================
boolean uicore_isBacklightFlag()             { return isBit(uicore_status, BIT_0); }

boolean uicore_isRepaintFlag()               { return isBit(uicore_status, BIT_1); }
void    uicore_setRepaintFlag()              { setBit(uicore_status, BIT_1); }
void    uicore_deleteRepaintFlag()           { deleteBit(uicore_status, BIT_1); }

boolean uicore_is2ndRepaintFlag()            { return isBit(uicore_status, BIT_2); }
void    uicore_set2ndRepaintFlag()           { setBit(uicore_status, BIT_2); }
void    uicore_delete2ndRepaintFlag()        { deleteBit(uicore_status, BIT_2); }

boolean uicore_isRepaintBatteryFlag()        { return isBit(uicore_status, BIT_3); }
void    uicore_setRepaintBatteryFlag()       { setBit(uicore_status, BIT_3); }
void    uicore_deleteRepaintBatteryFlag()    { deleteBit(uicore_status, BIT_3); }

boolean uicore_isRepaintShootCount()         { return isBit(uicore_status, BIT_4); }
void    uicore_setRepaintShootCount()        { setBit(uicore_status, BIT_4); }
void    uicore_deleteRepaintShootCount()     { deleteBit(uicore_status, BIT_4); }

boolean uicore_isMessageOnScreenFlag()       { return isBit(uicore_status, BIT_6); }
void    uicore_setMessageOnScreenFlag()      { setBit(uicore_status, BIT_6); }
void    uicore_deleteMessageOnScreenFlag()   { deleteBit(uicore_status, BIT_6); }

boolean uicore_isEditing()                   { return menu_editing; } 




// ============================================================================
// the main initialization function
// ============================================================================
void uicore_init() {
  
  pinMode(PIN_BACKLIGHT, OUTPUT);
  analogWrite(PIN_BACKLIGHT, 0);
  
       
  tft.InitLCD();
      
  display_width = tft.getDisplayXSize();
  display_height = tft.getDisplayYSize();
    
  // set the backlight to 100%
  uicore_backlight_level = 100;
  // set a backlight time of 15 seconds
  uicore_backlight_time = 15000;
  
  
  screen_code      = uicore_checkScreenPos();
  screen_code_old  = 255;
  menu_screen_pos  = 0;
  menu_pos         = 0;
  
  // init the ui timer used for periodic repaint events
  Timer4.attachInterrupt(uicore_handleTimer);
    
}




// ============================================================================
// processes the user interface
// ============================================================================
void uicore_process() {
  
  // the variable that hold the pressed keys if any
  key = KEY_NONE;
  
  
  /////////////////////////////////////////////////
  // I N P U T   A C T I O N                     //
  /////////////////////////////////////////////////
  if (input_isEvent()) {
    
    ///////////////////////////////////////////////
    // don't do anything if th backlight was off //
    ///////////////////////////////////////////////
    if (!uicore_isBacklightFlag()) {
      
      // delete the action flags and reset the
      // rotary encoder
      input_undoRotaryEvent();
      input_clearKeyEvent();
      
      // repaint the stati if the program is running
      if (core_isProgramRunningFlag()) {
        uicore_setRepaintFlag();
      }
      
    } 
    ///////////////////////////////////////////////
    // handle the input event                    //
    ///////////////////////////////////////////////
    else {
      
      /////////////////////////////////////////////
      // rotary event?
      if (input_isRotaryEvent()) {
        
        // delete the rotary-event-flag;
        input_clearRotaryEvent();
                
        // handle the rotary event
        uicore_handleRotary();
                
      }
      
      /////////////////////////////////////////////
      // key event?
      if (input_isKeyEvent()) {  
                
        // receive the key flags and store them
        key = input_getPressedKey();
        
        
        // if a message is on the screen, end it
        if (uicore_isMessageOnScreenFlag()) {
          
          // remove the message-is-on-screen-flag
          uicore_deleteMessageOnScreenFlag();
          
          // paint everything that needs to be painted
          uicore_repaint(true);
          
          
        // if not, do the regular - key handling
        } else {
          
          // handle the keys
          if (!uicore_handleKeyEvent(key)) {
            return;
          }
          
        } // end: no message on screen
        
      } // end: we have a key input
      
    } // Backlight on or off? 
    
    // enable the backlight  
    uicore_setBacklight(true);

  } // input event
  
  
  ///////////////////////////////////////////////////
  // M E S S A G E   O N   S C R E E N             //
  ///////////////////////////////////////////////////
  if (uicore_isMessageOnScreenFlag()) {
    
    // if the message duration is zero (infinite) then keep
    // it on the screen
    if (uicore_message_duration != 0) {
      
      // message duration time exceeded?
      if ((uicore_message_start_time + uicore_message_duration) <= millis()) {
        
        // remove the message-is-on-screen-flag
        uicore_deleteMessageOnScreenFlag();
        
        // paint everything that needs to be painted
        uicore_repaint(true);
          
      }
      
    }
 
  } else { // no message on screen  
    
    ///////////////////////////////////////////////////
    // B A C K L I G H T   T I M E O U T             //
    ///////////////////////////////////////////////////
    if (uicore_isBacklightFlag()) {
      
      if ((uicore_action_time + uicore_backlight_time) < now) {
        
        if (core_isProgramRunningFlag()) {
          // deactivate the backlight
          uicore_setBacklight(false);     
        } else {
          // fade out the backlight
          uicore_fadeOutBacklight(20);
        }
         
      }
          
    } 

    
    
    ///////////////////////////////////////////////////
    // R E P A I N T                                 //
    ///////////////////////////////////////////////////
    
    // don't paint if the backlight is off 
    if (uicore_isBacklightFlag()) {
      
      // is one of the repaint flags set?
      if (isBit(uicore_status, B00011110)) {
        
        // paint everything that needs to be painted
        uicore_repaint(false);
                      
        // delete all repaint flags (out of the 2nd - repaint flag)
        if (uicore_is2ndRepaintFlag()) {
         
          deleteBit(uicore_status, B00011110);
          uicore_setRepaintFlag();
          
        } else {
          
          deleteBit(uicore_status, B00011110);
          
        }
        
      }
      
    } // end: REPAINT; backlight is on
 
  } // end: no Message on Screen
  
}



// ===================================================================================
// returns a string from the flash 
// ===================================================================================
void uicore_getShortString(uint16_t buf_number, uint8_t target_line) {
  
  switch(buf_number) {
    
    ///////////////////////////////////////////////////////////////////
    // MAIN STRINGS AND UNITS
    
    case   0: strcpy(lines[target_line], string_0_short);       return;  
    case   1: strcpy(lines[target_line], string_1_short);       return;  
    case   2: strcpy(lines[target_line], string_2_short);       return;  
    case   3: strcpy(lines[target_line], string_3_short);       return;  
    case   4: strcpy(lines[target_line], string_4_short);       return;  
    case   5: strcpy(lines[target_line], string_5_short);       return;  
    case   6: strcpy(lines[target_line], string_6_short);       return;  
    case   7: strcpy(lines[target_line], string_6_short);       return;  
    case   8: strcpy(lines[target_line], string_8_short);       return;  
    case   9: strcpy(lines[target_line], string_9_short);       return;  
    case  10: strcpy(lines[target_line], string_10_short);      return;  
    case  11: strcpy(lines[target_line], string_11_short);      return;  
    case  12: strcpy(lines[target_line], string_12_short);      return;  
    case  13: strcpy(lines[target_line], string_13_short);      return;  
    case  14: strcpy(lines[target_line], string_14_short);      return;  
    case  15: strcpy(lines[target_line], string_15_short);      return;  
    case  16: strcpy(lines[target_line], string_16_short);      return;  
    case  17: strcpy(lines[target_line], string_17_short);      return;  
    case  18: strcpy(lines[target_line], string_18_short);      return;    
    case  19: strcpy(lines[target_line], string_19_short);      return;  
    case  20: strcpy(lines[target_line], string_20_short);      return;  
    case  21: strcpy(lines[target_line], string_21_short);      return;  
    case  22: strcpy(lines[target_line], string_22_short);      return;  
    case  23: strcpy(lines[target_line], string_23_short);      return;    
        
    
    ///////////////////////////////////////////////////////////////////
    // SYSTEM MODES
    
    case  30: strcpy(lines[target_line], string_30_short);      return;  
    case  31: strcpy(lines[target_line], string_31_short);      return;  

    case  60: strcpy(lines[target_line], string_60_short);      return;  
    case  61: strcpy(lines[target_line], string_61_short);      return;  
    case  62: strcpy(lines[target_line], string_62_short);      return;  
    
    
    ///////////////////////////////////////////////////////////////////
    // GENERAL EDIT- & JOG SCREEN STRINGS
    
    case  80: strcpy(lines[target_line], string_80_short);      return;  
    case  81: strcpy(lines[target_line], string_80_short);      return;  
    case  90: strcpy(lines[target_line], string_80_short);      return;  
    case  91: strcpy(lines[target_line], string_80_short);      return;  
        
        
    ///////////////////////////////////////////////////////////////////
    // GENERAL SETTING STRINGS
    
    case  92: strcpy(lines[target_line], string_92_short);      return;  
    case  93: strcpy(lines[target_line], string_93_short);      return;  
    case  94: strcpy(lines[target_line], string_94_short);      return;  
    case  95: strcpy(lines[target_line], string_95_short);      return;  
    case  96: strcpy(lines[target_line], string_96_short);      return;  
    case  97: strcpy(lines[target_line], string_97_short);      return;  
    case  98: strcpy(lines[target_line], string_98_short);      return;  
    
    
    ///////////////////////////////////////////////////////////////////
    // SETTINGS GENERAL
    case 100: strcpy(lines[target_line], string_100_short);     return;  
    case 101: strcpy(lines[target_line], string_101_short);     return; 
    case 102: strcpy(lines[target_line], string_102_short);     return; 
    case 103: strcpy(lines[target_line], string_103_short);     return;  
    case 104: strcpy(lines[target_line], string_104_short);     return;  
    case 105: strcpy(lines[target_line], string_105_short);     return;  
    case 106: strcpy(lines[target_line], string_106_short);     return;  
    case 107: strcpy(lines[target_line], string_107_short);     return; 
    case 108: strcpy(lines[target_line], string_108_short);     return; 
    case 109: strcpy(lines[target_line], string_109_short);     return;  
    case 110: strcpy(lines[target_line], string_110_short);     return;  
    case 111: strcpy(lines[target_line], string_111_short);     return;  
    case 112: strcpy(lines[target_line], string_112_short);     return;  
    case 113: strcpy(lines[target_line], string_113_short);     return;  
    case 114: strcpy(lines[target_line], string_114_short);     return;  
    //case 115: strcpy(lines[target_line], string_115_short);     return;  
    
    
    ///////////////////////////////////////////////////////////////////
    // SETTINGS CAMERA
    case 120: strcpy(lines[target_line], string_120_short);     return;  
    case 121: strcpy(lines[target_line], string_121_short);     return;  
    case 122: strcpy(lines[target_line], string_122_short);     return;  
    case 123: strcpy(lines[target_line], string_123_short);     return;  
    case 124: strcpy(lines[target_line], string_124_short);     return;  
    
    ///////////////////////////////////////////////////////////////////
    // SETTINGS MOTOR
    case 140: strcpy(lines[target_line], string_140_short);     return;
    case 141: strcpy(lines[target_line], string_141_short);     return;  
    case 142: strcpy(lines[target_line], string_142_short);     return;  
    case 143: strcpy(lines[target_line], string_143_short);     return;  
    case 144: strcpy(lines[target_line], string_144_short);     return;  
    case 145: strcpy(lines[target_line], string_145_short);     return;  
    case 146: strcpy(lines[target_line], string_146_short);     return; 
    case 147: strcpy(lines[target_line], string_147_short);     return; 
    case 148: strcpy(lines[target_line], string_148_short);     return; 
    case 149: strcpy(lines[target_line], string_149_short);     return;  
    case 150: strcpy(lines[target_line], string_150_short);     return; 
    case 151: strcpy(lines[target_line], string_151_short);     return; 
    case 152: strcpy(lines[target_line], string_152_short);     return; 
    case 153: strcpy(lines[target_line], string_153_short);     return; 
    case 154: strcpy(lines[target_line], string_154_short);     return; 
    case 155: strcpy(lines[target_line], string_155_short);     return; 
    case 156: strcpy(lines[target_line], string_156_short);     return; 
    case 157: strcpy(lines[target_line], string_157_short);     return; 
    case 158: strcpy(lines[target_line], string_158_short);     return; 
    case 159: strcpy(lines[target_line], string_159_short);     return; 
    case 160: strcpy(lines[target_line], string_160_short);     return; 
    case 161: strcpy(lines[target_line], string_161_short);     return; 
    
    
    ///////////////////////////////////////////////////////////////////
    // SETTINGS CHAINING
    case 165: strcpy(lines[target_line], string_165_short);     return;
    case 166: strcpy(lines[target_line], string_166_short);     return;  
    case 167: strcpy(lines[target_line], string_167_short);     return;
    case 168: strcpy(lines[target_line], string_168_short);     return;  
    case 169: strcpy(lines[target_line], string_169_short);     return;
        
    
    ///////////////////////////////////////////////////////////////////
    // SETTINGS TRIGGER
    case 180: strcpy(lines[target_line], string_180_short);     return;
    case 181: strcpy(lines[target_line], string_181_short);     return;  
    case 182: strcpy(lines[target_line], string_182_short);     return;  
    case 183: strcpy(lines[target_line], string_183_short);     return;  
    case 184: strcpy(lines[target_line], string_184_short);     return;  
    case 185: strcpy(lines[target_line], string_185_short);     return;  
    case 186: strcpy(lines[target_line], string_186_short);     return;  
    case 187: strcpy(lines[target_line], string_187_short);     return;  
    
    
    ///////////////////////////////////////////////////////////////////
    // RUN MODE - TIMELAPSE
    
    case 200: strcpy(lines[target_line], string_200_short);     return;  
    case 201: strcpy(lines[target_line], string_201_short);     return;  
    case 202: strcpy(lines[target_line], string_202_short);     return;  
    case 203: strcpy(lines[target_line], string_203_short);     return;  
    case 204: strcpy(lines[target_line], string_204_short);     return;  
    case 205: strcpy(lines[target_line], string_205_short);     return;  
    case 206: strcpy(lines[target_line], string_206_short);     return;  
    case 207: strcpy(lines[target_line], string_207_short);     return;  
    case 208: strcpy(lines[target_line], string_208_short);     return;  
    
    ///////////////////////////////////////////////////////////////////
    // PANORAMA
    
    case 210: strcpy(lines[target_line], string_210_short);     return;  
    case 211: strcpy(lines[target_line], string_211_short);     return;  
    
  }
  
  // no valid result - fill in nothing
  strcpy(lines[target_line], STR_EMPTY);

}


// ===================================================================================
// returns a longer string from the flash 
// ===================================================================================
void uicore_getLongString(uint16_t buf_number) {
  
  switch(buf_number) {
    
    ///////////////////////////////////////////////////////////////////
    // MAIN STRINGS AND UNITS
    
    
    ///////////////////////////////////////////////////////////////////
    // SYSTEM MODES
    
    case  30: strcpy(data_line, string_30_long);      return;  
    case  31: strcpy(data_line, string_31_long);      return;  
    
    
    ///////////////////////////////////////////////////////////////////
    // GENERAL EDIT- & JOG SCREEN STRINGS
    
    
    ///////////////////////////////////////////////////////////////////
    // GENERAL SETTING STRINGS
    
    case 93: strcpy(data_line, string_93_long);       return;
    case 94: strcpy(data_line, string_94_long);       return;
    case 95: strcpy(data_line, string_95_long);       return;
    case 96: strcpy(data_line, string_96_long);       return;
    
    ///////////////////////////////////////////////////////////////////
    // SETTINGS GENERAL
    
    case 100: strcpy(data_line, string_100_long);     return;
    case 101: strcpy(data_line, string_101_long);     return;
    case 102: strcpy(data_line, string_102_long);     return;
    case 103: strcpy(data_line, string_103_long);     return;
    case 107: strcpy(data_line, string_107_long);     return;
    case 108: strcpy(data_line, string_108_long);     return;
    case 109: strcpy(data_line, string_109_long);     return;
    case 110: strcpy(data_line, string_110_long);     return;
    case 111: strcpy(data_line, string_111_long);     return;
    case 112: strcpy(data_line, string_112_long);     return;
    case 113: strcpy(data_line, string_113_long);     return;
    case 114: strcpy(data_line, string_114_long);     return;
    case 115: strcpy(data_line, string_115_long);     return;
    case 116: strcpy(data_line, string_116_long);     return;
    case 117: strcpy(data_line, string_117_long);     return;
    case 118: strcpy(data_line, string_118_long);     return;
    
    
    ///////////////////////////////////////////////////////////////////
    // SETTINGS CAMERA

    case 120: strcpy(data_line, string_120_long);     return;
    case 122: strcpy(data_line, string_122_long);     return;
    case 124: strcpy(data_line, string_124_long);     return;
    
    ///////////////////////////////////////////////////////////////////
    // SETTINGS MOTOR
        
    case 140: strcpy(data_line, string_140_long);     return;
    case 143: strcpy(data_line, string_143_long);     return;
    case 144: strcpy(data_line, string_144_long);     return;
    case 145: strcpy(data_line, string_145_long);     return;
    case 146: strcpy(data_line, string_146_long);     return;
    case 150: strcpy(data_line, string_150_long);     return;
    case 151: strcpy(data_line, string_151_long);     return;
    case 154: strcpy(data_line, string_154_long);     return;
    case 155: strcpy(data_line, string_155_long);     return;
    case 161: strcpy(data_line, string_161_long);     return;
        
    ///////////////////////////////////////////////////////////////////
    // SETTINGS CHAINING
    
    case 165: strcpy(data_line, string_165_long);     return;
    case 168: strcpy(data_line, string_168_long);     return; 
    
    ///////////////////////////////////////////////////////////////////
    // SETTINGS TRIGGER
    
    case 180: strcpy(data_line, string_180_long);     return;  
    case 181: strcpy(data_line, string_181_long);     return;  
    case 182: strcpy(data_line, string_182_long);     return;  
    case 183: strcpy(data_line, string_183_long);     return;  
    case 187: strcpy(data_line, string_187_long);     return;  
  
    ///////////////////////////////////////////////////////////////////
    // RUN MODE - TIMELAPSE
    
    case 200: strcpy(data_line, string_200_long);     return;  
    case 201: strcpy(data_line, string_201_long);     return;  
    case 202: strcpy(data_line, string_202_long);     return;  
    case 203: strcpy(data_line, string_203_long);     return;  
    case 204: strcpy(data_line, string_204_long);     return;      
    case 205: strcpy(data_line, string_205_long);     return;  
    case 206: strcpy(data_line, string_206_long);     return;  
    case 207: strcpy(data_line, string_207_long);     return;  
    case 208: strcpy(data_line, string_208_long);     return;  
    
    ///////////////////////////////////////////////////////////////////
    // PANORAMA
    
    case 210: strcpy(data_line, string_210_long);     return;  
    case 211: strcpy(data_line, string_211_long);     return;  

    
    ///////////////////////////////////////////////////////////////////
    // MESSAGES
    
    case 220: strcpy(data_line, string_220_long);     return;  
    case 221: strcpy(data_line, string_221_long);     return;  
    case 222: strcpy(data_line, string_222_long);     return;  
    
    case 225: strcpy(data_line, string_225_long);     return;  
    case 226: strcpy(data_line, string_226_long);     return;  
    
    case 227: strcpy(data_line, string_227_long);     return;  
    case 228: strcpy(data_line, string_228_long);     return;  
    case 229: strcpy(data_line, string_229_long);     return;  
    
    case 230: strcpy(data_line, string_230_long);     return;  
    case 231: strcpy(data_line, string_231_long);     return;  
    case 232: strcpy(data_line, string_232_long);     return;  
    
    case 233: strcpy(data_line, string_233_long);     return;  
    case 234: strcpy(data_line, string_234_long);     return;  
    case 235: strcpy(data_line, string_235_long);     return;  
    case 236: strcpy(data_line, string_236_long);     return;  
    case 237: strcpy(data_line, string_237_long);     return;   
    case 238: strcpy(data_line, string_238_long);     return;
    
  }
  
  // no result - return an empty string
  strcpy(data_line, STR_EMPTY);
  
}


// ===================================================================================
// handles the pressed key(s)
// ===================================================================================
boolean uicore_handleKeyEvent(uint8_t key) {
  
  uint8_t menu_pos_old = menu_pos;
  uint8_t popup_menu_pos_old = popup_menu_pos;
  uint8_t menu_screen_pos_old = menu_screen_pos;
  
  
  // =============
  // check if we need to stop the motors on any key-press
  // when a move to home is active
  if ((motor_isMoveToPositionRunning()) &&
      (!core_isProgramRunningFlag())) {
    
    // stop the motor   
    motor_disableAll();    
    // clear the key event
    input_clearKeyEvent();  
    // and leave this function - there is nothing more to do here
    return true;  
  } 
  
  
  
  // =============
  // KEY 1 
  // >> PLAY, STOP, BACK
  if (isBit(key, KEY_1)) {
    
    // is there a popup menu on the screen?
    if (isBit(uicore_status, BIT_7)) {
      
      // just end the popup menu
      uicore_endPopupMenu();
        
    } 
    // are we jogging? 
    else if (core_isJogModeFlag()) {
      
      // stop the jogging stuff  
      uicore_stopJog();
    }
    // no popup menu and not jogging
    else {
      
      // if we are editing a value...
      if (menu_editing) {
        
        //stop editing
        menu_editing = false;  
        rotary.setKeyMode(true);
        
      }  
      // int the settings menu?
      else if (uicore_isSettingsScreen()) {
        
        // not editing ?
        if (!menu_editing) {
          
          // jump back to the main screen
          screen_code = uicore_checkScreenPos();
          menu_screen_pos = 0;
          menu_pos        = 0;
          
        }
        
      } 
      // if none of the above...
      else {
        
        // start / stop the program
        if (core_isProgramRunningFlag()) {
          // stop the program
          core_stopProgram(com.isMaster() && (com.getSlaveCount() > 0));
        } else {
          if (!core_startProgram()) {
            // the start did no succeed, leave this function
            return false; 
          }
          
        }
        
      }
    
    }
  
  }
    
  
  // =============
  // KEY 2
  // >> MENU
  if (isBit(key, KEY_2)) {
    
    // is there a popup menu on the screen?
    if (isBit(uicore_status, BIT_7)) {
      
      // just end the popup menu
      uicore_endPopupMenu();
      
    } 
    // no popup menu
    else {
    
      // only go the menu if we are no editing a value
      if (!menu_editing) {
       
        // if we are not yet in the menu
        if (!uicore_isSettingsScreen()) {
          screen_code = 100;
          menu_screen_pos = 0;
          menu_pos = 0;
        }
        
        // we are allready in the menu
        else {
          
          // go to the next menu screen
          screen_code++;
          if (screen_code > 104) screen_code = 100; 
          
          // go to the top of the new screen
          menu_screen_pos  = 0;
          menu_pos         = 0;
                              
        } // end: we are already in the menu
      
      } // end: we are not editing 
      
      // when editing a value, this key is used to
      // toggle the granularity
      else {
                
        edit_granularity *= 10;
        if (edit_granularity > 100) edit_granularity = 1;
        
      }  
        
    } // end: no popup menu
    
  }
  
  
  
  // =============
  // KEY_ROTARY PRESS
  // >> EDIT, END EDIT, SELECT 
  if (isBit(key, KEY_ROTARY)) {
    
    // is the popup menu on the screen?    
    if (isBit(uicore_status, BIT_7)) {
      
      // do what we need to do for the selected menu point
      uicore_doAction(line_codes[popup_menu_pos]);
      
      // close the popup menu
      uicore_endPopupMenu();
              
    } else {
      
      // are we jogging the motor?
      if (core_isJogModeFlag()) {
        
        uicore_stopJog();
        
      } else {
      
        // check if the menu point we are standing on
        // is an action menu point (this also executed
        // the action if it is an action)
        if (!uicore_doAction(line_codes[menu_pos])) {
          
          // a special case: reset the settings
          if ((line_codes[menu_pos] == 112) &&
              (menu_editing == true))  {
            core_deleteSettings();      
          }
          
          
          // if this line is not disabled
          if (!lines_disabled[menu_pos]) {
            
            // toggle the editing var
            menu_editing = !menu_editing;  
            
          }
          
        }
                       
      }
          
    }
        
  }
  
  
  // =============
  // KEY_ROTARY-LONG PRESS
  // >> POPUP-MENU
  if (isBit(key, KEY_ROTARY_LONG)) {
    
    if (!menu_editing && 
        !core_isProgramRunningFlag() &&
        !uicore_isSettingsScreen()) {
    
      setBit(uicore_status, BIT_7);  
           
    }
        
  }
    
  
  
  // =============
  // KEY 2 LONG PRESS
  // >> POPUP-MENU
  if (isBit(key, KEY_2_LONG)) {
    
    if (!menu_editing && 
        !core_isProgramRunningFlag() &&
        !uicore_isSettingsScreen()) {
    
      setBit(uicore_status, BIT_7);  
           
    }
    
  }
    
  
  // =============
  // KEY UP (ROTARY ROTATION)
  if (isBit(key, KEY_UP)) {
    
    // popup menu?
    if (isBit(uicore_status, BIT_7)) {
      
      if (popup_menu_pos > 0) {
        popup_menu_pos--;
      }
    } 
    
    // regular menu:
    else {
    
      // regular scrolling
      if (!menu_editing) {
              
        if (menu_pos > 0) {
          menu_pos--;
        } 
        
        if (menu_screen_pos > 0) {
          menu_screen_pos--; 
        }
        
        if (rotary.getPosition() < 0) {
          rotary.setPosition(0);
        }
      
      } // end: regular scrolling
    
    } // end: regular menu
  
  }
  
  
  
  // =============
  // KEY DOWN (ROTARY ROTATION)
  if (isBit(key, KEY_DOWN)) {

    // popup menu?
    if (isBit(uicore_status, BIT_7)) {
      
      if (popup_menu_pos < popup_menu_length - 1)  {
        popup_menu_pos++;
      } 
    } 
    
    // regular menu:
    else {
    
      // if we are not editing a value 
      if (!menu_editing) {
        
        if (menu_pos < menu_length - 1) {
          menu_pos++; 
        }
          
        if ((menu_screen_pos < (menu_max_screen_lines - 1)) && 
            (menu_screen_pos < menu_length - 1)) {
          
            menu_screen_pos++; 
           
        }  
        
      }
    
    } // end: regular menu

  }
  
  
  
  // any UI changes or editing?
  if ((menu_editing && isBit(key, KEY_UP | KEY_DOWN))                            ||
      (screen_code                     != screen_code_old)                       ||
      (menu_screen_pos                 != menu_screen_pos_old)                   ||
      (menu_pos                        != menu_pos_old       )                   ||
      (menu_editing                    != menu_editing_old)                      ||
      (popup_menu_pos                  != popup_menu_pos_old)                    ||
      (core_is_jog_mode_old            != core_isJogModeFlag())                  ||
      (isBit(uicore_status, B10000000) != isBit(uicore_status_old, B10000000))   ||
      (isBit(uicore_status, B01000000) != isBit(uicore_status_old, B01000000))   
     ) {
    
    // repaint
    uicore_setRepaintFlag();  
    
  }
  
  return true;
    
}




// ===================================================================================
// handles the position change of the rotary encoder
// ===================================================================================
void uicore_handleRotary() {
  
  uicore_setRepaintFlag();  
  
  // are we in jog mode?
  if (core_isJogModeFlag()) {
    
    int16_t pos = rotary.getPosition();
    // a very very complex formula to achieve a non-linear speed-up / speed-down
    // behaviour while jogging
    float jogSpeed = pow((float) pos / 8.0, 2) / 2.0;
    
    // max limit...
    if (jogSpeed > motors[motor_selected].getMaxSpeed()) {
      jogSpeed = motors[motor_selected].getMaxSpeed();
    }
    
    // negative speed?
    if (pos < 0) jogSpeed = -jogSpeed;
    
    // set the speed to the currently selected motor
    motor_setJogSpeed(jogSpeed);
  } 
  
}




// ===================================================================================
// does the paint handling
// ===================================================================================
void uicore_repaint(boolean fullRepaint) {
  
  
  // check if we need to do a full 
  // repaint on certain ui-changes 
  // or events
  boolean full = (fullRepaint)                                                                ||
                 (screen_code_old                       != screen_code)                       ||
                 (uicore_col_scheme                     != uicore_col_scheme_old)             ||
                 (uicore_font_index                     != uicore_font_index_old)             || 
                 (menu_editing                          != menu_editing_old)                  ||
                 (core_isProgramRunningFlag()           != program_is_running_old)            ||
                 (isBit(uicore_status_old, B10000000)   != isBit(uicore_status, B10000000))   ||
                 (isBit(uicore_status_old, B01000000)   != isBit(uicore_status, B01000000))   ||
                 (core_is_jog_mode_old                  != core_isJogModeFlag())              ||
                 (uicore_is2ndRepaintFlag());

  
  // paint the dashboard if the program is running
  if (core_isProgramRunningFlag()) {
    
    uipaint_dashboard(full);  
    
  }
  
  
  // Are we in jog Mode?
  else if (core_isJogModeFlag()) {
    
    if (!uicore_isRepaintBatteryFlag()) {
      // paint the jog window
      uipaint_jogWindow(full);
    }
    
  }
    
  // we are displaying a popup menu
  else if (isBit(uicore_status, BIT_7)) {
    
    if (!uicore_isRepaintBatteryFlag()) {
      // load the menu strings
      uicore_loadPopupMenuStrings(200);
      // paint the popup menu
      uipaint_popupMenu(full);
    }
    
  } 
  
  
  // we are editing a value right now
  else if (menu_editing) {
    
    if (!uicore_isRepaintBatteryFlag()) {
      
      // paint the edit-screen-header
      uipaint_headerEdit(full);
      
      // paint the edit screen
      uipaint_editScreen(full);
      
    }
        
  } 
  
  
  // menu based screen
  else {

    // we are in the settings menu
    if (uicore_isSettingsScreen()) {
      
      if (!uicore_isRepaintBatteryFlag()) {
        boolean paintFullSettingsHeader = (screen_code_old < 100) ||
                                          (menu_editing != menu_editing_old);
      
        // paint the settings header
        uipaint_headerSettings(paintFullSettingsHeader);
      }
      
    }
    
    // regular screen
    else {
      
      // check the screen ID
      screen_code = uicore_checkScreenPos();
      
      if (!uicore_isRepaintBatteryFlag() ||
          full) {
            
        // paint the header
        uipaint_header(full);
              
        if (full) {
          // paint battery info on a full repaint
          uipaint_battery(true); 
          
        }
              
      } else {
        
        uipaint_battery(true); 
        
      }

      uipaint_cycleLength();
      uipaint_shotCount();
      
    }
    
    
    // paint the menu items
    if (!uicore_isRepaintBatteryFlag() ||
        full) {
          
      // load the menu strings
      uicore_loadMenuStrings();
      
      // paint the menu
      uipaint_menu(full);
       
    }
  
  }
 
  
  menu_editing_old          = menu_editing;
  uicore_col_scheme_old     = uicore_col_scheme;
  uicore_font_index_old     = uicore_font_index;
  screen_code_old           = screen_code;
  program_is_running_old    = core_isProgramRunningFlag();
  core_is_jog_mode_old      = core_isJogModeFlag(); 
  uicore_status_old         = uicore_status; 

  
}



// ===================================================================================
// Prepared a message and initializes as needed values.
// If the message-duration = 0 then it will be displayed until the users presses
// a key.
// ===================================================================================
void uicore_showMessage(byte title, byte fromLine, byte toLine, uint32_t duration) {
  
  menu_length = 0;
  
  // fill the line codes with the id of the title and the ids of the actual
  // message content:
  line_codes[0] = title; // title

  // fill the array with the content we want to paint  
  for (int i=fromLine; i<=toLine; i++) {
    line_codes[1 + menu_length++] = i; // content ...
  }
  
  // paint the message
  uipaint_message();
  
  // set the flag which indicates that a message is on the screen
  uicore_setMessageOnScreenFlag();
    
  // remember the time when the message was painted
  uicore_message_start_time = millis();
  
  // set the message duration in ms
  uicore_message_duration = duration;
  
  // enable the backlight
  uicore_setBacklight(true);
  
  
}



// ===================================================================================
// Generates the data right side of the menu (as well as when edited)
// ===================================================================================
void uicore_generateDataString(uint16_t line_code) {
  
  // empty the string
  strcpy(data_line, ""); 
  
  // go through all options and create the data string
  switch (line_code) {
    
    // backlight power
    case  100 :  {
                   if (menu_editing) {
                     //                                               step  min  max                    loop 
                     uicore_changeValueUByte(&uicore_backlight_level,    1,   5, BACKLIGHT_MAX_PROCENT, false);
                     
                     // set the new backlight value
                     uicore_setBacklight(true);
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
      
                   // generate teh data string 
                   itoa(uicore_backlight_level, data_line, 10);
                   strcat(data_line, "%"); 
                   break;
                     
    }
                 
    // backlight time
    case  101 :  {
                   if (menu_editing) {
                     //                                               step   min  max                   
                     uicore_changeValueULong(&uicore_backlight_time,  1000, 3000, 3600000); // max 1h
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   } 
      
                   // generate the data string 
                   itoa(uicore_backlight_time / 1000, data_line, 10);
                   strcat(data_line, string_6_short); // s
                   break;
                     
    }
                
    // color scheme
    case  102 :  {
                   if (menu_editing) {
                     //                                          step  min  max                            loop 
                     uicore_changeValueUByte(&uicore_col_scheme,    1,   0, uicore_color_scheme_count - 1, true);
                     
                     // do a full repaint again... but only if sometinhg was chenged
                     if (uicore_col_scheme != uicore_col_scheme_old) {
                       uicore_set2ndRepaintFlag();
                     }
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                  
                   // generate the data string 
                   strcpy(data_line, color_schemes[uicore_col_scheme].name);
                   break;
                     
     }    
                 
    // rotary flipped
    case  103 :  {
                   if ((menu_editing) &&
                       (isBit(key, KEY_UP) || isBit(key, KEY_DOWN))) {
                     rotary.setFlipped(!rotary.isFlipped());  
                 
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();    
                   }
                   
                  
                   // generate the data string 
                   if (rotary.isFlipped()) {
                     strcpy(data_line, string_3_short);  // enabled
                   } else {
                     strcpy(data_line, string_4_short);  // disabled
                   }  
                   
                   break;    
                     
    }    
    
    // start from home
    case 107 : {
                   if ((menu_editing) &&
                       (isBit(key, KEY_UP) || isBit(key, KEY_DOWN))) {
                     
                     // toggle the settings bit    
                     core_toggleMoveToHomeBeforeStartFlag();
                         
                     sd_setSettingsChangedFlag();    
                   }
                         
                   // generate the data string 
                   if (core_isMoveToHomeBeforeStartFlag()) {
                     strcpy(data_line, string_3_short);  // enabled
                   } else {
                     strcpy(data_line, string_4_short);  // disabled
                   }  
      
                   break;     
    }
    
    
    // system mode
    case 108 : {
                  if (menu_editing) {
                    
                    if (isBit(key, KEY_UP)) {

                      /*
                      if      (isBit(core_mode, MODE_TIMELAPSE))   core_mode = MODE_VIDEO; 
                      else if (isBit(core_mode, MODE_VIDEO))       core_mode = MODE_TIMELAPSE; 
                      //*/

                      //*  
                      if      (isBit(core_mode, MODE_TIMELAPSE))   core_mode = MODE_VIDEO; 
                      else if (isBit(core_mode, MODE_VIDEO))       core_mode = MODE_PANORAMA; 
                      else if (isBit(core_mode, MODE_PANORAMA))    core_mode = MODE_TIMELAPSE;
                      //*/  
                                                                    
                    }
                   
                    if (isBit(key, KEY_DOWN)) {

                      /*
                      if      (isBit(core_mode, MODE_TIMELAPSE))   core_mode = MODE_VIDEO; 
                      else if (isBit(core_mode, MODE_VIDEO))       core_mode = MODE_TIMELAPSE; 
                      //*/

                      //*  
                      if      (isBit(core_mode, MODE_TIMELAPSE))   core_mode = MODE_VIDEO; 
                      else if (isBit(core_mode, MODE_VIDEO))       core_mode = MODE_PANORAMA; 
                      else if (isBit(core_mode, MODE_PANORAMA))    core_mode = MODE_TIMELAPSE; 
                      //*/  
                      
                    }
                    
                    // check for validity
                    core_checkModes();
                    // send the new mode to the clients i we have some;
                    com_sendSystemMode(true);
                    // send the loop information to clients if needed
                    com_sendLoopData();
                    // set the "settings wer changed" flag                             
                    sd_setSettingsChangedFlag();  
                                        
                    
                    // show the long strings when in editing mode
                    if      (isBit(core_mode, MODE_TIMELAPSE)) strcpy(data_line, string_60_long);
                    else if (isBit(core_mode, MODE_VIDEO))     strcpy(data_line, string_61_long);
                    else if (isBit(core_mode, MODE_PANORAMA))  strcpy(data_line, string_62_long);
                  
                 } else {
                 
                   // show the short strings when in editing mode 
                   if      (isBit(core_mode, MODE_TIMELAPSE)) strcpy(data_line, string_60_short);
                   else if (isBit(core_mode, MODE_VIDEO))     strcpy(data_line, string_61_short);
                   else if (isBit(core_mode, MODE_PANORAMA))  strcpy(data_line, string_62_short); 
                   
                 }
                
                
                
                 break;
            
    }
    
    
    // setup style
    case 109 : {
                  if (!isBit(core_mode, MODE_PANORAMA)) {
                    
                    if (menu_editing) {
                      
                      if (isBit(key, KEY_UP)) {
                        
                       if      (isBit(core_setup_style, SETUP_STYLE_RUN))         core_setup_style = SETUP_STYLE_KEYFRAMES; 
                       else if (isBit(core_setup_style, SETUP_STYLE_KEYFRAMES))   core_setup_style = SETUP_STYLE_RUN; 
                                                                    
                      }
                     
                      if (isBit(key, KEY_DOWN)) {
                        
                        if      (isBit(core_setup_style, SETUP_STYLE_RUN))        core_setup_style = SETUP_STYLE_KEYFRAMES; 
                        else if (isBit(core_setup_style, SETUP_STYLE_KEYFRAMES))  core_setup_style = SETUP_STYLE_RUN; 
                        
                      }
                      
                      // check for validity
                      core_checkModes();                         
                      // send the new mode to the clients i we have some;
                      com_sendSystemMode(true);
                      // set the "settings wer changed" flag                             
                      sd_setSettingsChangedFlag();
                    
                      // show the long strings when in editing mode
                      if      (isBit(core_setup_style, SETUP_STYLE_RUN))         strcpy(data_line, string_50_long);
                      else if (isBit(core_setup_style, SETUP_STYLE_KEYFRAMES))   strcpy(data_line, string_51_long);
                    
                    } else {
                      
                      // show the short strings when in editing mode
                      if      (isBit(core_setup_style, SETUP_STYLE_RUN))         strcpy(data_line, string_50_short);
                      else if (isBit(core_setup_style, SETUP_STYLE_KEYFRAMES))   strcpy(data_line, string_51_short);
                        
                    }
                    
                  }
                  
                  break;
            
    }
    
    
    // system font
    case  110 :  {
                   if (menu_editing) {
                     //                                          step  min  max             loop 
                     uicore_changeValueUByte(&uicore_font_index,    1,   0, FONT_COUNT - 1, true);
                     
                     // do a full repaint again... but only if sometinhg was chenged
                     if (uicore_font_index != uicore_font_index_old) {
                       uicore_set2ndRepaintFlag();
                     }
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                  
                   // generate the data string 
                   strcpy(data_line, uicore_fonts[uicore_font_index].name);
                   break;
    }
    
    
    // move style
    case 111 : {
                  if (!isBit(core_mode, MODE_VIDEO)     &&
                      !isBit(core_mode, MODE_PANORAMA)) {
                    
                    if (menu_editing) {
                      
                      if (isBit(key, KEY_UP)) {
                        
                       if      (isBit(core_move_style, MOVE_STYLE_SMS))          core_move_style = MOVE_STYLE_CONTINUOUS; 
                       else if (isBit(core_move_style, MOVE_STYLE_CONTINUOUS))   core_move_style = MOVE_STYLE_SMS; 
                                                                    
                      }
                     
                      if (isBit(key, KEY_DOWN)) {
                        
                        if      (isBit(core_move_style, MOVE_STYLE_SMS))         core_move_style = MOVE_STYLE_CONTINUOUS; 
                        else if (isBit(core_move_style, MOVE_STYLE_CONTINUOUS))  core_move_style = MOVE_STYLE_SMS; 
                        
                      }
                      
                      // check for validity
                      core_checkModes();
                      // send the new mode to the clients i we have some;
                      com_sendSystemMode(true);
                      // set the "settings wer changed" flag                             
                      sd_setSettingsChangedFlag();  
                      
                      
                      
                      // show the long strings when in editing mode
                      if      (isBit(core_move_style, MOVE_STYLE_SMS))          strcpy(data_line, string_30_long);
                      else if (isBit(core_move_style, MOVE_STYLE_CONTINUOUS))   strcpy(data_line, string_31_long);
                    
                    } else {
                      
                      // show the short strings when in editing mode
                      if      (isBit(core_move_style, MOVE_STYLE_SMS))          strcpy(data_line, string_30_short);
                      else if (isBit(core_move_style, MOVE_STYLE_CONTINUOUS))   strcpy(data_line, string_31_short);
                    
                    }
                    
                  }
                  
                  break;
            
    }
    
    
    // loop
    case 113 : {
                  if ((menu_editing) &&
                      (isBit(key, KEY_UP) || isBit(key, KEY_DOWN))) {
                     
                     // toggle the settings bit    
                     core_toggleLoopFlag();
                     // send the loop information to clients if needed
                     com_sendLoopData();
                     // set settings were changed flag    
                     sd_setSettingsChangedFlag();    
                  }  
                               
                  
                  if (core_isLoopFlag()) {
                    strcpy(data_line, string_3_short); // enabled
                  } else {
                    strcpy(data_line, string_4_short); // disabled
                  }
                      
                  break;
    }


    // endless shoot
    case 114 : {
                  if ((menu_editing) &&
                      (isBit(key, KEY_UP) || isBit(key, KEY_DOWN))) {
                     
                     // toggle the settings bit    
                     core_toggleEndlessShootFlag();
                     // send the loop information to clients if needed
                     com_sendEndlessShootData();
                     // set settings were changed flag    
                     sd_setSettingsChangedFlag();    
                  }  
                               
                  
                  if (core_isEndlessShootFlag()) {
                    strcpy(data_line, string_3_short); // enabled
                  } else {
                    strcpy(data_line, string_4_short); // disabled
                  }
                      
                  break;
    }
    
    // camera type
    case  120 :  {
                   if ((menu_editing) &&
                       (isBit(key, KEY_UP) || isBit(key, KEY_DOWN))) {
                     // toggle between Canon & Nikon
                     cam_toggleCameraType();  
                 
                     sd_setSettingsChangedFlag();      
                   }
                   
                   // Nikon
                   if (cam_isCameraType()) {
                     strcpy(data_line, string_20_short);
                   } 
                   // Canon
                   else {
                     strcpy(data_line, string_19_short);
                   }
                   
                   break;
                   
    }  
    
    
    // camera test shot
    case  121 :  {  
      
                   strcpy(data_line,  string_123_short);  // SHOOT!
                   break; 
    
    }
    
    
    // camera post delay
    case  122 :  {  
                   if (menu_editing) {
                      
                     if (cam_post_delay < 1000) {
                       uicore_changeValueULong(&cam_post_delay, rotary.getLowVelocity() * 10, 0, 1000);   
                     } 
                     else if (cam_post_delay < 10000) {
                       uicore_changeValueULong(&cam_post_delay, rotary.getLowVelocity() * 100, 990, 10000);   
                     } 
                     else {
                       uicore_changeValueULong(&cam_post_delay, rotary.getLowVelocity() * 500, 9900, ULONG_MAX);   
                     }
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }    
                   
                   
                   if (cam_post_delay < 1000) {
                     itoa(cam_post_delay, data_line, 10);
                     strcat(data_line, string_11_short); // ms  
                   } else {
                     sprintf(data_line,"%.1f", ((float)cam_post_delay) / 1000.0);
                     strcat(data_line, string_6_short); // "  
                   }
                   
                   break;
                                            
    }
    
    
    // camera focus time
    case  124 :  {  
                   if (menu_editing) {
                      
                     if (cam_focus < 1000) {
                       uicore_changeValueULong(&cam_focus, rotary.getLowVelocity() * 10, 0, 1000);   
                     } 
                     else if (cam_focus < 10000) {
                       uicore_changeValueULong(&cam_focus, rotary.getLowVelocity() * 100, 990, 10000);   
                     } 
                     else {
                       uicore_changeValueULong(&cam_focus, rotary.getLowVelocity() * 500, 9900, ULONG_MAX);   
                     }
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }    
                   
                   
                   if (cam_focus < 1000) {
                     itoa(cam_focus, data_line, 10);
                     strcat(data_line, string_11_short); // ms  
                   } else {
                     sprintf(data_line,"%.1f", ((float)cam_focus) / 1000.0);
                     strcat(data_line, string_6_short); // "  
                   }
                   
                   break;
                                            
    }
                 
    // motor selection
    case 140 :   {
      
                   if (menu_editing) {
                     uicore_changeValueUByte(&motor_selected, 1, 0, DEF_MOTOR_COUNT - 1, true);  
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   } 
                    
                   strcpy(data_line, string_153_short);   // "motor "
                   itoa((motor_selected + 1), temp, 10);
                   
                   strcat(data_line, temp);          
                   break;  
                   
    }
    
    // motor go home
    case 141 :   {
                         
                   strcpy(data_line, string_158_short);      // GO!              
                   break;  
    }             
    
    // motor set home
    case 142 :   {
                         
                   strcpy(data_line, string_159_short);      // SET!                 
                   break;  
    }  
  
    // motor post delay
    case 143 :   {
                   
                   uint32_t value = motors[motor_selected].getPostDelay(); 
                    
                   if (menu_editing) {
                      
                     if (cam_post_delay < 1000) {
                       uicore_changeValueULong(&value, rotary.getLowVelocity() * 10, 0, 1000);   
                     } 
                     else if (cam_post_delay < 10000) {
                       uicore_changeValueULong(&value, rotary.getLowVelocity() * 100, 990, 10000);   
                     } 
                     else {
                       uicore_changeValueULong(&value, rotary.getLowVelocity() * 500, 9900, ULONG_MAX);   
                     }
                     
                     // set the motor post delay
                     motors[motor_selected].setPostDelay(value); 
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }    
                   
                   
                   if (value < 1000) {
                     itoa(value, data_line, 10);
                     strcat(data_line, string_11_short); // ms  
                   } else {
                     sprintf(data_line,"%.1f", ((float)value) / 1000.0);
                     strcat(data_line, string_6_short); // "  
                   }
                   
                   break; 
                   
    }  
    
    // motor type
    case 144 :   {
                   
                   if ((menu_editing) &&
                       (isBit(key, KEY_UP) || isBit(key, KEY_DOWN))) {
                     
                     uint8_t type = motors[motor_selected].getType();
                     uicore_changeValueUByte(&type, 1, 0, 1, true);   
                     motors[motor_selected].setType(type);        
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();          
                   } 
      
      
                   if (motors[motor_selected].getType() == MOTOR_TYPE_LINEAR) {
                     strcpy(data_line, string_14_short);  // LINEAR
                   } else {
                     strcpy(data_line, string_15_short);  // RADIAL                     
                   }
                   
                   
                   break;  
    }  
    
    // motor max speed
    case 145 : {
                   
                   float value = motors[motor_selected].getMaxSpeed();
                                       
                   if (menu_editing) {
                     uicore_changeValueFloat(&value, (float)rotary.getLowVelocity() / (float) edit_granularity, 0.1, 100000); 
                     motors[motor_selected].setMaxSpeed(value);
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                     
                     if      (edit_granularity == 1)   sprintf(data_line,"%.0f", value);   
                     else if (edit_granularity == 10)  sprintf(data_line,"%.1f", value); 
                     else if (edit_granularity == 100) sprintf(data_line,"%.2f", value);   
                   
                   } else {
                     sprintf(data_line,"%.1f", value); 
                   }       
      
                   
                   // motor type for correct unit 
                   if (motors[motor_selected].getType() == MOTOR_TYPE_LINEAR) {
                     strcat(data_line, string_17_short); // cm/sec
                   } else {
                     strcat(data_line, string_16_short); // °/sec
                   } 
                   
                       
                   break;  
    }  
    
    
    // motor ramp time
    case 146 : {
                   
                   if (menu_editing) {
                     uicore_changeValueFloat(&motor_ramp_time[motor_selected], 0.1, 0.1, 999); 
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }        

      
                   sprintf(data_line,"%.1f", motor_ramp_time[motor_selected]);     
                   strcat(data_line, string_6_short); // "
                       
                   break;  
    }  
    
    
    // motor limit switches
    case 147 :   {
                   
                   //strcpy(data_line, string_3_short); // enabled
                       
                   break;  
    }    
    
    // motor keep powered
    case  150 :  {
      
                   if ((menu_editing) &&
                       (isBit(key, KEY_UP) || isBit(key, KEY_DOWN))) {
                     
                     motors[motor_selected].setKeepPowered(!motors[motor_selected].isKeepPowered());
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                   
                   if (motors[motor_selected].isKeepPowered()) {
                     strcpy(data_line, string_3_short);   // enabled 
                   } else {
                     strcpy(data_line, string_4_short);   // disabled 
                   } 
                   
                   break;
    }
                   
                 
    // motor calibration
    case  151 :  {
                   float cal = motors[motor_selected].getCalibration();
      
                   if (menu_editing) {
                     uicore_changeValueFloat(&cal, (float)rotary.getLowVelocity() / (float) edit_granularity, 0.01, 100000.0); 
                     
                     // set the new calibration value
                     motors[motor_selected].setCalibration(cal);
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                   
                   
                   // give more precision if we are editing the value (up to 0.01)
                   if (menu_editing) {
                     
                     if      (edit_granularity == 1)   sprintf(data_line,"%.0f", cal);  
                     else if (edit_granularity == 10)  sprintf(data_line,"%.1f", cal);  
                     else if (edit_granularity == 100) sprintf(data_line,"%.2f", cal);  
                                          
                   } else {
                     itoa((int)round(cal), data_line, 10);
                   }                   
                   
                   // motor type for correct unit 
                   if (motors[motor_selected].getType() == MOTOR_TYPE_LINEAR) {
                     strcat(data_line, string_10_short); // st/cm  
                   } else {
                     strcat(data_line, string_21_short); // st/°
                   }
                   
                   
                   break;    
                     
    }  
    
    // motor sleep
    case  154 :  {
      
                   if ((menu_editing) &&
                       (isBit(key, KEY_UP) || isBit(key, KEY_DOWN))) {
                     
                     // toggel the setting
                     motor_sleep[motor_selected] = !motor_sleep[motor_selected];
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                   
                   if (motor_sleep[motor_selected]) {
                     strcpy(data_line, string_3_short);   // enabled 
                   } else {
                     strcpy(data_line, string_4_short);   // disabled 
                   } 
                   
                   break;
    }
    
    // motor direction flipped
    case  155 :  {
      
                   if ((menu_editing) &&
                       (isBit(key, KEY_UP) || isBit(key, KEY_DOWN))) {
                     
                     // toggel the setting
                     motors[motor_selected].setDirectionFlipped(!motors[motor_selected].isDirectionFlipped());
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                   
                   if (motors[motor_selected].isDirectionFlipped()) {
                     strcpy(data_line, string_3_short);   // enabled 
                   } else {
                     strcpy(data_line, string_4_short);   // disabled 
                   } 
                   
                   break;
    }
    
    // motor check max speed
    case  161 :  {
      
                   if ((menu_editing) &&
                       (isBit(key, KEY_UP) || isBit(key, KEY_DOWN))) {
                     
                     motor_check_speed[motor_selected] = !motor_check_speed[motor_selected];
                                          
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                   
                   if (motor_check_speed[motor_selected]) {
                     strcpy(data_line, string_3_short);   // enabled 
                   } else {
                     strcpy(data_line, string_4_short);   // disabled 
                   } 
                   
                   break;
    }
    
    
    // daisy chain device ID
    case  165 :  {
      
                   if ((menu_editing) &&
                       (isBit(key, KEY_UP) || isBit(key, KEY_DOWN))) {
                     
                     uint8_t comId = com.getID();    
                     uicore_changeValueUByte(&comId, 1, 0, MOCOM_MAX_SLAVES - 1, true);
                     
                     // set the new status
                     com.setID(comId);  
                     // we are no longer a registered (known) slave device
                     com_deleteRegisteredSlaveFlag();
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                   
                   // print the status to the screen
                   if (com.isMaster()) {
                     strcpy(data_line, string_166_short); // Master
                   } else {
                     strcpy(data_line, string_167_short); // Device
                     itoa(com.getID(), temp, 10);        
                     strcat(data_line, temp); 
                   }
                   
                   break;
    }
    
    // update the daisy chain
    case 168 :   {
                         
                   strcpy(data_line, string_169_short);   // UPDATE!               
                   break;  
    }  
        
    
    // trigger selection
    case 180 :   {
      
                   if (menu_editing) {
                     uicore_changeValueUByte(&trigger_selected, 1, 0, DEF_TRIGGER_COUNT - 1, true);  
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   } 
                    
                   strcpy(data_line, string_184_short);   // "trigger "
                   itoa((trigger_selected + 1), temp, 10);
                   
                   strcat(data_line, temp);          
                   break;  
    }
    
    // trigger eneabled
    case 181 :   {
      
                   if (menu_editing &&
                      (isBit(key, KEY_UP) || isBit(key, KEY_DOWN))) {
                     
                     // flip the enabled bit
                     trigger_setEnabled(trigger_selected, !trigger_triggers[trigger_selected].enabled);
                                          
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   } 
                   
                   if (trigger_triggers[trigger_selected].enabled) {
                     strcpy(data_line, string_3_short);   // enabled 
                   } else {
                     strcpy(data_line, string_4_short);   // disabled 
                   }  
                  
                   break;  
    }
    
    // trigger action
    case 182 :   {
      
                   if (menu_editing) {
                     
                     // change the trigger action variable
                     uicore_changeValueUByte(&trigger_triggers[trigger_selected].action, 1, 0, TRIGGER_ACTION_COUNT - 1, true);  
                                          
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   } 
                                      
                   strcpy(data_line, trigger_actions[trigger_triggers[trigger_selected].action].name);   // disabled 
                   
                   break;  
    }
    
    // trigger type
    case 183 :   {
      
                   if (menu_editing &&
                      (isBit(key, KEY_UP) || isBit(key, KEY_DOWN))) {
                     
                     if (trigger_triggers[trigger_selected].type == FALLING) {
                       trigger_triggers[trigger_selected].type = RISING;
                     } else {
                       trigger_triggers[trigger_selected].type = FALLING;
                     }
                                          
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   } 
                   
                   if (trigger_triggers[trigger_selected].type == FALLING) {
                     strcpy(data_line, string_186_short);   // FALLING  
                   } else {
                     strcpy(data_line, string_185_short);   // RISING   
                   }                   
                   
                   
                   break;  
    }
    
    
    // trigger debounce
    case 187 :   {
      
                   if (menu_editing &&
                      (isBit(key, KEY_UP) || isBit(key, KEY_DOWN))) {
                     
                     // flip the enabled bit
                     trigger_setDebounce(trigger_selected, !trigger_triggers[trigger_selected].debounce);
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   } 
                   
                   if (trigger_triggers[trigger_selected].debounce) {
                     strcpy(data_line, string_3_short);   // enabled 
                   } else {
                     strcpy(data_line, string_4_short);   // disabled 
                   }  
                  
                   break;  
    }
    
    
    // motor total distance
    case 200 :   {
                   if (menu_editing) {
                     
                     uicore_changeValueFloat(&motor_total_distance[motor_selected], 
                                             ((float)rotary.getLowVelocity() / (float) edit_granularity),
                                             0.0, 
                                             65000.0); 
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                     
                                          
                     if      (edit_granularity ==   1) sprintf(data_line, "%.0f", motor_total_distance[motor_selected]);  
                     else if (edit_granularity ==  10) sprintf(data_line, "%.1f", motor_total_distance[motor_selected]);  
                     else if (edit_granularity == 100) sprintf(data_line, "%.2f", motor_total_distance[motor_selected]);  
                   
                   } else {
                     sprintf(data_line,"%.1f", motor_total_distance[motor_selected]);
                   }         
                        
                   
                   if (motors[motor_selected].getType() == MOTOR_TYPE_LINEAR) {
                     strcat(data_line, string_5_short); // cm
                   } else {
                     strcat(data_line, string_22_short); // °
                   } 
                     
                   break; 
    }    
    
    // direction
    case 201 :   {
                   if ((menu_editing) &&
                       (isBit(key, KEY_UP) || isBit(key, KEY_DOWN))) {
                     // flip the motor direction
                     motor_program_direction[motor_selected] = !motor_program_direction[motor_selected];
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                   
                   if (motor_program_direction[motor_selected]) {
                     strcpy(data_line, string_8_short);  // CCW
                   } else {
                     strcpy(data_line, string_7_short);  // CW
                   }
                   
                   break;
    }
        
    // camera exposure
    case 202 :   {
                   if (menu_editing) {
                     uicore_changeValueUInt(&cam_exposure_index, rotary.getLowVelocity(), 0, UINT_MAX);
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                   
                   // show special strings for the exposures from 1/50 -> 2 seconds (17 values)
                   if (cam_exposure_index < CAM_EXPOSURE_COUNT) {
                     
                     // generate the display value
                     strcpy(data_line, cam_exposure_values[cam_exposure_index].name);
                     // set the actual system var    
                     cam_exposure = cam_exposure_values[cam_exposure_index].exposure;

                   }
                     
                   else {
                     // get the exposure value for the current exp-position
                     // (value 16 = 2 sec --> offset of 12 (12 = 0; 13 = 0.5"; 14 = 1.0"; 15 = 1.5", 16 = 2.0)
                     cam_exposure = (cam_exposure_index - 12) * 500;
                     
                     // generate the display value
                     float exposureSec = (float)cam_exposure / 1000.0;
                     sprintf(data_line,"%.1f", exposureSec);     
                     strcat(data_line, string_6_short); // "
                     
                   }
      
                   break;    
    }
  
    // record time
    case 203 :   {
                     if (menu_editing) {
                     
                     
                     int stepSize;
                  
                     if (isBit(key, KEY_UP)) {
                    
                       if   (setup_record_time <  60000)  stepSize = 1000;
                       else                               stepSize = 10000;
                        
                     } else if(isBit(key, KEY_DOWN)) {
                      
                       if   (setup_record_time >  60000)  stepSize = 10000;
                       else                               stepSize = 1000;
                    
                     }
                     
                     uicore_changeValueULong(&setup_record_time, rotary.getLowVelocity() * stepSize, 1000, ULONG_MAX);
                     
                     
                     // it needs to be at least 1 second
                     if (setup_record_time < 1000) {
                       setup_record_time = 1000;
                     }
                     
                     // update the set up vlaues
                     core_checkValues();
                     // send the new values to the clients if we have some
                     com_sendRunData();
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   } 
                   
                   uint32_t hours   = setup_record_time / MILLIS_PER_HOUR;
                   uint32_t minutes = (setup_record_time - 
                                       (hours * MILLIS_PER_HOUR)
                                      ) / MILLIS_PER_MINUTE;
                   uint32_t seconds = (setup_record_time - 
                                       (hours * MILLIS_PER_HOUR) -
                                       (minutes * MILLIS_PER_MINUTE)
                                      ) / MILLIS_PER_SECOND;                   
                   
                   // hours
                   itoa(hours, data_line, 10);
                   // double punkt
                   strcat(data_line, STR_DPOINT);
                   // minutes
                   itoa(minutes, temp, 10);
                   if (minutes < 10) strcat(data_line, STR_NULL); 
                   strcat(data_line, temp); 
                   // double punkt 
                   strcat(data_line, STR_DPOINT); 
                   // seconds
                   itoa(seconds, temp, 10);
                   if (seconds < 10) strcat(data_line, STR_NULL); 
                   strcat(data_line, temp); 
                   // unit
                   strcat(data_line, string_6_short); // "
                   
                   break; 
    }
    
    // playback time
    case 204 :   {
                   if (menu_editing) {
                     uicore_changeValueULong(&setup_play_time, rotary.getLowVelocity() * 1000, 1000, ULONG_MAX);
                     
                     // it needs to be at least one second
                     if (setup_play_time < 1000) {
                       setup_play_time = 1000;
                     }
                     
                     // update the set up vlaues
                     core_checkValues();
                     // send the new values to the clients if we have some
                     com_sendRunData();
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   } 
                   
                   uint32_t hours   = setup_play_time / MILLIS_PER_HOUR;
                   uint32_t minutes = (setup_play_time - 
                                       (hours * MILLIS_PER_HOUR)
                                      ) / MILLIS_PER_MINUTE;
                   uint32_t seconds = (setup_play_time - 
                                       (hours * MILLIS_PER_HOUR) -
                                       (minutes * MILLIS_PER_MINUTE)
                                      ) / MILLIS_PER_SECOND;                   
                   
                   // hours
                   itoa(hours, data_line, 10);
                   // double punkt
                   strcat(data_line, STR_DPOINT);
                   // minutes
                   itoa(minutes, temp, 10);
                   if (minutes < 10) strcat(data_line, STR_NULL); 
                   strcat(data_line, temp); 
                   // double punkt 
                   strcat(data_line, STR_DPOINT); 
                   // seconds
                   itoa(seconds, temp, 10);
                   if (seconds < 10) strcat(data_line, STR_NULL); 
                   strcat(data_line, temp); 
                   // unit
                   strcat(data_line, string_6_short); // "
                   
                   break; 
    }
    
    // camera fps
    case 205 :   {
                   if (menu_editing) {
                     uicore_changeValueUByte(&cam_fps, 1, 1, 255, false);
                     
                     // update the set up vlaues
                     core_checkValues();
                     // send the new values to the clients if we have some
                     com_sendRunData();
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                   
                   itoa(cam_fps, data_line, 10);
                   strcat(data_line, string_18_short); // fps
                  
                   break; 
    }
    
    // global run-ramp in
    case 206 :   {
                   
                   if (menu_editing) {
                     uint8_t upperLimit = 100 - setup_run_ramp_out[motor_selected];
                     uicore_changeValueUByte(&setup_run_ramp_in[motor_selected], 1, 0, upperLimit, false);
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                   
                   itoa(setup_run_ramp_in[motor_selected], data_line, 10);
                   strcat(data_line, STR_PERCENT); // %
                  
                   break; 
    }
    
    
    // global run-ramp out
    case 207 :   {
                   if (menu_editing) {
                     uint8_t upperLimit = 100 - setup_run_ramp_in[motor_selected];
                     uicore_changeValueUByte(&setup_run_ramp_out[motor_selected], 1, 0, upperLimit, false);
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                   
                   itoa(setup_run_ramp_out[motor_selected], data_line, 10);
                   strcat(data_line, STR_PERCENT); // %
                  
                   break; 
    }


    // motor delay
    case 208 :   {
                   if (menu_editing) {
                     
                     uicore_changeValueUByte(&setup_run_move_delay[motor_selected], 1, 0, 99, false);
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                   
                   itoa(setup_run_move_delay[motor_selected], data_line, 10);
                   strcat(data_line, STR_PERCENT); // %
                  
                   break; 
    }
    

    // panorama pictures motor 1
    case 210 :   {
                   if (menu_editing) {
                     
                     uicore_changeValueUByte(&panorama_pics_m1, 1, 1, 255, false);
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                   
                   itoa(panorama_pics_m1, data_line, 10);
                   //strcat(data_line, STR_PERCENT); // %
                  
                   break; 
    }  


    // panorama pictures motor 2
    case 211 :   {
                   if (menu_editing) {
                     
                     uicore_changeValueUByte(&panorama_pics_m2, 1, 1, 255, false);
                     
                     // set the flag that settings were changed
                     sd_setSettingsChangedFlag();
                   }
                   
                   itoa(panorama_pics_m2, data_line, 10);
                   //strcat(data_line, STR_PERCENT); // %
                  
                   break; 
    }  
    


    
    
           
  }
   
}


// ===================================================================================
boolean uicore_doAction(uint16_t line_code) {
  
  // go through all options and create the data string
  switch (line_code) {
    
    
    // camera test shot
    case 121 : {
                  cam_trigger();
                  core_delay(200);
                  cam_stop();
    
                  return true;
    }  
    
    // motor (selected) go home
    case 141 : {
      
                  // define the move to the home position
                  motor_defineMoveToPosition(motor_selected, 0, true);  
                  // start the move
                  motor_startMovesToPosition();
    
                  return true;
    }  
    
    // motor (selected) set home position
    case 142 : {
                  // define the move to the home position
                  motors[motor_selected].setMotorPosition(0);  
    
                  return true;
    }  
    
    // jog motor 1
    case 148 :  {
                   // enable the jog-mode  
                   core_setJogModeFlag(); 
                   // select motor 1
                   motor_selectMotor(0);  
                   
                   // set the rotary to the regular rotary mode
                   rotary.setKeyMode(false);
                   // set a damp zone to the rotary encoder
                   rotary.setStepDampZone(-1, 1, 0.2);
                   // reset the rotary encoder's position
                   rotary.setPosition(0);
                   
                   // get the max position for the rotary (based on the max speed)
                   int maxPos = ((uint16_t)(sqrt(motors[motor_selected].getMaxSpeed() * 2.0) * 8.0)) + 1;
                   rotary.setPositionMax(maxPos);
                   rotary.setPositionMin(-maxPos);
                                                         
                   // Start the repaint timer every 250ms
                   Timer4.start(TIMER_CYCLE * 1000);
                   
                   return true; 
    }
    
    // jog motor 2
    case 149 :  {
                   // enable the jog-mode 
                   core_setJogModeFlag();
                   // select motor 2
                   motor_selectMotor(1);  
                                      
                   // set the rotary to the regular rotary mode
                   rotary.setKeyMode(false);
                   // set a damp zone to the rotary encoder
                   rotary.setStepDampZone(-1, 1, 0.2);
                   // reset the rotary encoder's position
                   rotary.setPosition(0);
                   
                   // get the max position for the rotary (based on the max speed)
                   int maxPos = ((uint16_t)(sqrt(motors[motor_selected].getMaxSpeed() * 2.0) * 8.0)) + 1;
                   rotary.setPositionMax(maxPos);
                   rotary.setPositionMin(-maxPos);
                   
                   // Start the repaint timer every 250ms
                   Timer4.start(TIMER_CYCLE * 1000);
                                      
                   return true; 
    }
    
    // move all motors home
    case 152 :  {
                    
                   // set up the motors for move to home
                   for (int i=0; i<DEF_MOTOR_COUNT; i++) {
                     motor_defineMoveToPosition(i, 0, true);  
                   }
                   
                   // start the moves
                   motor_startMovesToPosition();
                     
                   return true; 
    }
    
    
    // set Start
    case 156 :  {
                   
                   // loop all motors
                   for (byte i=0; i<DEF_MOTOR_COUNT; i++) {
                     
                     float dist = 0;
                     
                     // caclulate the distance to be moved (basd on the old end point
                     // which is defined by total move distance and direction)
                     if (motor_program_direction[i]) {
                       dist = -motor_total_distance[i] - motors[i].getMotorPosition();
                     } else {
                       dist = motor_total_distance[i] - motors[i].getMotorPosition();
                     }
                     
                     // set the new values (total move distance and direction)
                     if (dist < 0) {
                       motor_program_direction[i] = true; 
                       motor_total_distance[i] = -dist;
                     } else {
                       motor_program_direction[i] = false; 
                       motor_total_distance[i] = dist;  
                     }
                     
                     // now set the motor position to zero for the current motor
                     // because this is our new start point
                     motors[i].setMotorPosition(0);
                     
                   }
                   
                   return true; 
    }
    
    
    // set End
    case 157 :  {
                   // loop all motors
                   for (byte i=0; i<DEF_MOTOR_COUNT; i++) {
                     
                     float dist = motors[i].getMotorPosition();
                     
                     // set the new values (total move distance and direction)
                     if (dist < 0) {
                       motor_program_direction[i] = true; 
                       motor_total_distance[i] = -dist;
                     } else {
                       motor_program_direction[i] = false; 
                       motor_total_distance[i] = dist;  
                     }
                     
                   }
                   return true; 
    }
    
    // Preview
    case 160 :  {
                   
                   // print a "moving motor to home" message
                   uicore_showMessage(225, 226, 226, 1);
                   
                   // define the curves (this needs to be done before the speed-check 
                   // and before the move-to-start becuase otherwise the max speed would be zero
                   // and the start position not defined
                   motor_checkKeyframes(); 
                   
                   // move all motors to their start position
                   if (motor_moveToStartKeyframe()) {
                     
                     // set the time-factor now
                     // (it will be needed for the max-speed-check that follows)
                     motor_time_factor = (float)((double) setup_record_time / (double) setup_play_time);
                     
                     // check if the planned move would exceed the max speed of any motor                 
                     if (motor_checkCurvesMaxSpeedOk()) {
                       
                       // message to the user that the preview is executed now
                       uicore_showMessage(230, 231, 232, 0);
                       
                       // Execute Preview
                       core_doPreview();
                       
                       // remove the meessages from the screen
                       uicore_deleteMessageOnScreenFlag();
                       
                     } else {
                       
                       // message to the user that this exceeds the max speed of the motors
                       uicore_showMessage(227, 228, 229, 3000);
                       
                       // re-set the time-factor
                       motor_time_factor = 1.0;
                       
                       // leave 
                       return false;
                     }
                   
                     // re-set the time-factor
                     motor_time_factor = 1.0;
                   
                     return true; 
                     
                   } else {
                     
                     return false; 
                   }
                   
    }               
      
      
    // Update daisy chain
    case 168 :  {   
                    if (com.isMaster()) {              
      
                      // paint the message that shows that the chain is updated...
                      uicore_showMessage(233, 234, 235, 0);
                        
                      // deselecct the currently selected slave
                      com.deselectSlave();
                      // registering the slaves
                      com.registerSlaves();  
                      
                      // send the new mode to all slaves (if we have any)
                      com_sendSystemMode(true);
                      // send the run-data
                      com_sendRunData();
                      // send the loop information
                      com_sendLoopData();
                                            
                      
                      // remove all possible meessages from the screen
                      uicore_deleteMessageOnScreenFlag();
                      // do a full repaint to have a fresh screen
                      uicore_repaint(true);
                      
                    }
                    
                    return true;
    }      
    
    
  } // end: switch  


  // no action executed
  return false;

}




// ===================================================================================
// This function gathers all strings for the current menu screen
// ===================================================================================
void uicore_loadMenuStrings() {
   
  // reset the menu length
  menu_length = 0;
  
  // flag for hiding a menu line
  boolean disable_line;
    
  
  // loop all menu-content relations we have
  for (int i=0; i<uicore_content_relation_count; i++) {
    
    disable_line = false;
    
    // if we found an entry that is on the current screen
    if (ui_content_relations[i].screen == screen_code) {
      
      // store the corresponding string to the lines array
      uicore_getShortString(ui_content_relations[i].menu_item, menu_length);
        
      // store the line_code
      line_codes[menu_length] = ui_content_relations[i].menu_item;
         
      ////////////////////////////////////////////////   
      // check if the current item should be disabled
      
      // "empty" items are always disabled
      if (ui_content_relations[i].menu_item == 23) {
        disable_line = true;   
      }

      
            
      // is video mode?
      if (core_mode == MODE_VIDEO &&
          (
            (ui_content_relations[i].menu_item == 109) ||     // Setup Style
            (ui_content_relations[i].menu_item == 111)        // Move Style
          )) {
        
        disable_line = true;   
      }   



      // is video mode and on setting endless shoot?
      if (core_mode == MODE_PANORAMA &&
          (
           (ui_content_relations[i].menu_item == 109) ||      // Setup Style
           (ui_content_relations[i].menu_item == 111) ||      // Move Style
           (ui_content_relations[i].menu_item == 113)         // Loop 
          )) {
            
        disable_line = true;   
      }   

      
      // if we are a slave device
      if (!com.isMaster() &&
         (ui_content_relations[i].menu_item == 168)) { // Update chain
        disable_line = true;            
      }
      
      // if we are a registered slave device
      if (com_isRegisteredSlaveFlag()) {
            
        if (
            (ui_content_relations[i].menu_item == 203) ||    // Record time
            (ui_content_relations[i].menu_item == 204) ||    // Playback time
            (ui_content_relations[i].menu_item == 205) ||    // Playback FPS
            
            (ui_content_relations[i].menu_item == 108) ||    // System Mode
            (ui_content_relations[i].menu_item == 109) ||    // Setup Style
            (ui_content_relations[i].menu_item == 111) ||    // Move Style
            
            (ui_content_relations[i].menu_item == 113)       // Loop
            
           ) {        
        
          disable_line = true;  
        }   
        
      } // end: we are registered slave device
       
       
      // set the status to the helper array 
      lines_disabled[menu_length] = disable_line;   
         
      // increment the menu length
      menu_length++;
      
    }  
    
  } 
  
}



// ===================================================================================
// This function gathers all strings for the popup menu screen
// ===================================================================================
void uicore_loadPopupMenuStrings(uint8_t menu_code) {
   
  // reset the menu length
  popup_menu_length = 0;
  bool show;
  
  
  // loop all menu-content relations we have
  for (int i=0; i<uicore_content_relation_count; i++) {
    
    // if we found an entry that is on the current screen
    if (ui_content_relations[i].screen == menu_code) {
      
      show = true;
      
      // if we are not in timelapse mode,
      // do now show the "preview" menu point 
      if (
          (! isBit(core_mode, MODE_TIMELAPSE)) &&
          (ui_content_relations[i].menu_item == 160)
         ){
        show = false;
      }
      
      
      if (show) {
      
        // store the corresponding string to the lines array
        uicore_getShortString(ui_content_relations[i].menu_item, popup_menu_length);
          
        // store the line_code
        line_codes[popup_menu_length] = ui_content_relations[i].menu_item;
           
        // increment the menu length
        popup_menu_length++;
        
      }

    }  
    
  } 
  
}



// ======================================================================================
// returns the main position code based on the system mode / setup mode
// ======================================================================================
byte uicore_checkScreenPos() {
  
  // 10:  SMS            run setup         >> Timelapse
  // 11:  SMS            keyframes         >> Timelapse
  // 20:  Continuous     run setup         >> Timelapse
  // 21:  Continuous     keyframes         >> Timelapse
  // 30:  Video          run setup         >> Video
  // 31:  Video          keyframes         >> Video
  // 40:  Panorama       n.a.              >> Panorama
  
  
  ///////////////////////////////
  // TIMELPASE
  if (isBit(core_mode, MODE_TIMELAPSE)) {
    
    if (isBit(core_setup_style, SETUP_STYLE_RUN)) {
    
      if (isBit(core_move_style, MOVE_STYLE_SMS))        return 10;  
      if (isBit(core_move_style, MOVE_STYLE_CONTINUOUS)) return 20;  
        
    } else if (isBit(core_setup_style, SETUP_STYLE_KEYFRAMES)) {
      
      if (isBit(core_move_style, MOVE_STYLE_SMS))        return 11;  
      if (isBit(core_move_style, MOVE_STYLE_CONTINUOUS)) return 21;  
    
    }
    
  } // end: timelapse
  
  ///////////////////////////////
  // VIDEO
  else if (isBit(core_mode, MODE_VIDEO)) {
    
    if (isBit(core_setup_style, SETUP_STYLE_RUN)) {
    
      return 30;  
        
    } else if (isBit(core_setup_style, SETUP_STYLE_KEYFRAMES)) {
      
      return 31;
    
    }
    
  } // end: video
  
  
  ///////////////////////////////
  // PANORAMA
  else if (isBit(core_mode, MODE_PANORAMA)) {
    
    return 40;
    
  } // end: panorama
 
    
    
  // default return = SMS with RUN Setup  
  //return 10;    
}


// ============================================================================
// sets the backlight level to be used from now on
// ============================================================================
void uicore_setBacklightLevel(uint8_t value) {
  
  // check if the value is too big
  if (value > BACKLIGHT_MAX_PROCENT) value = BACKLIGHT_MAX_PROCENT;
  
  // store the used level
  uicore_backlight_level = value;

}



// ============================================================================
// sets the backlight level on or off
// ============================================================================
void uicore_setBacklight(boolean state) {
  
  // calculate the actual PWM value we need to set
  uint8_t light_val = BACKLIGHT_NORM * uicore_backlight_level / 100;
  
  
  // SET BACKLIGHT "ON"  
  if (state) {
    
    // activate the light
    analogWrite(PIN_BACKLIGHT, light_val);
    // set the backlight flag
    setBit(uicore_status, BIT_0);
    // remember the time when we activated the light
    uicore_action_time = millis();
    
  
  } 
  
  // SET BACKLIGHT "OFF" if it is on 
  else {
    
    // activate the light
    analogWrite(PIN_BACKLIGHT, 0);
    // delete the backlight flag
    deleteBit(uicore_status, BIT_0);
        
  }
  
} 



// ============================================================================
// fades the backlight in
// ============================================================================
void uicore_fadeInBacklight(byte delayTime) {
  
  // calculate the actual PWM value we need to set
  byte maxVal = BACKLIGHT_NORM * uicore_backlight_level / 100;
  
  for (int i=0; i<=maxVal; i++) {
    
    // set the backlight value
    analogWrite(PIN_BACKLIGHT, i);
    
    // abort if something happened
    if (input_isEvent()) {
      // remeber the 
      uicore_action_time = now;
      return;
    }
    
    // small delay
    core_delay(delayTime);
  }   
  
  // remember the time when we activated the light
  uicore_action_time = millis();
  // set the backlight flag
  setBit(uicore_status, BIT_0);
} 



// ============================================================================
// fades the backlight out
// ============================================================================
void uicore_fadeOutBacklight(byte delayTime) {
  
  // delete the backlight flag
  deleteBit(uicore_status, BIT_0);
  
  // calculate the actual PWM value we need to set
  byte maxVal = BACKLIGHT_NORM * uicore_backlight_level / 100;
  
  for (int i=maxVal; i>=0; i--) {
    
    // set the backlight value
    analogWrite(PIN_BACKLIGHT, i);
    
    // abort if something happened
    if (input_isEvent()) return;
    
    // small delay
    core_delay(delayTime);
  }   
   
} 





// ============================================================================
void uicore_fadeInString(String str, int x, int y, int stp, byte r, byte g, byte b, int dly) {
  
  for (int i = 0; i<256; i+=stp) {
    
    float fac = (float)i / 255.0;
    tft.setColor(fac * r, fac * g, fac * b);
    tft.print(str, x, y);
    
    if (dly > 0) delay(dly);
    
    if (input_isKeyEvent()) return;
  }  
  
}


// ============================================================================
// converts RGB to RGB565
// ============================================================================
uint16_t uicore_getRGB565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);  
}


// ============================================================================
// are we standing on a settings screen?
// ============================================================================
boolean uicore_isSettingsScreen() {

  return (screen_code >= 100) && (screen_code <= 104);  
  
}





// ======================================================================================
// this function changes a numeric byte value
// ======================================================================================
void uicore_changeValueUByte(byte *value, byte stepSize, byte minimum, byte maximum, boolean loopValues) {
 
  // if key pressed
  if ( isBit(key, KEY_UP) &&  
      ((*value <= maximum) || (maximum == 0))
     ) {
    
    // no maximum
    if (maximum == 0) {
      
      *value += stepSize; //+ (stepSize * key_count);   
      
    } 
    // maximum exists
    else {
    
      // would our next step exceed the maximum limit?           
      //if ((maximum - *value) > (key_count * stepSize)) {
      if ((maximum - *value) >= stepSize) {
        
        *value += stepSize; // + ((float) stepSize * (float) key_count);
        
      } else if (loopValues) {
        *value = minimum; 
      }  else {
        *value = maximum; 
      }
    }
    
    // cut the rest (only granularity as big as the stepSize)
    *value = ((unsigned byte) *value / (unsigned byte) stepSize) * (unsigned byte) stepSize;
        
    // set the settings were changed flag
    sd_setSettingsChangedFlag();
    
    
  } else if ( isBit(key, KEY_DOWN) &&
             (*value >= minimum)) {
    
    // would our next step exceed the minimum limit?           
    //if ((*value - minimum) > (key_count * stepSize)) {
    if ((*value - minimum) >= stepSize) {
      
      *value -= stepSize; //n+ ((float) stepSize * (float) key_count);
      
    } else if (loopValues) {
      *value = maximum;
    } else {
      *value = minimum;  
    }
    
    // cut the rest (only granularity as big as the stepSize)
    *value = ((unsigned byte) *value / (unsigned byte) stepSize) * (unsigned byte) stepSize;
    
    // set the settings were changed flag
    sd_setSettingsChangedFlag();
  }
  
}


// ======================================================================================
// this function changes a numeric int value
// ======================================================================================
void uicore_changeValueUInt(unsigned int *value, int stepSize, unsigned int minimum, unsigned int maximum) {
 
  
  // if key pressed
  if ( isBit(key, KEY_UP) &&  
      ((*value < maximum) || (maximum == 0))
     ) {
    
    // no maximum
    if (maximum == 0) {
      
      *value += stepSize; // + (stepSize * key_count);   
      
    } 
    // maximum exists
    else {
    
      // would our next step exceed the maximum limit?           
      //if ((maximum - *value) > (key_count * stepSize)) {
      if ((maximum - *value) >= stepSize) {  
        *value += stepSize; // + ((float) stepSize * (float) key_count);
      } else {
        *value = maximum;  
      }
    }
    
    // cut the rest (only granularity as big as the stepSize)
    *value = ((unsigned int) *value / (unsigned int) stepSize) * (unsigned int) stepSize;
    
    // set the settings were changed flag
    sd_setSettingsChangedFlag();
    
  } else if ( isBit(key, KEY_DOWN) &&
             (*value > minimum)) {
    
    // would our next step exceed the minimum limit?  
    //if ((*value - minimum) > (key_count * stepSize)) {    
    if ((*value - minimum) >= stepSize) {
      *value -= stepSize; // + ((float) stepSize * (float) key_count);
    } else {
      *value = minimum;  
    }
    
    // cut the rest (only granularity as big as the stepSize)
    *value = ((unsigned int) *value / (unsigned int) stepSize) * (unsigned int) stepSize;
    
    // set the settings were changed flag
    sd_setSettingsChangedFlag();
  }
  
}

// ======================================================================================
// this function changes a numeric long value
// ======================================================================================
void uicore_changeValueULong(unsigned long *value, long stepSize, unsigned long minimum, unsigned long maximum) {
 
  // if key pressed
  if ( isBit(key, KEY_UP) &&  
      ((*value < maximum) || (maximum == 0))
     ) {
       
    // no maximum
    if (maximum == 0) {
      
      *value += stepSize; // + (stepSize * key_count);   
      
    } 
    // maximum exists
    else {
    
      // would our next step exceed the maximum limit?  
      //if ((maximum - *value) > (key_count * stepSize)) {      
      if ((maximum - *value) >= stepSize) {
        *value += stepSize; // + ((float) stepSize * (float) key_count);
      } else {
        *value = maximum;  
      }
    }
    
    // cut the rest (only granularity as big as the stepSize)
    *value = ((unsigned long) *value / (unsigned long) stepSize) * (unsigned long) stepSize;
    
    // set the settings were changed flag
    sd_setSettingsChangedFlag();
    
  } else if ( isBit(key, KEY_DOWN) &&
             (*value > minimum)) {
               
    // would our next step exceed the minimum limit?           
    //if ((*value - minimum) > (key_count * stepSize)) {
    if ((*value - minimum) >= stepSize) {
      *value -= stepSize; // + ((float) stepSize * (float) key_count);
    } else {
      *value = minimum;  
    }
    
    // cut the rest (only granularity as big as the stepSize)
    *value = ((unsigned long) *value / (unsigned long) stepSize) * (unsigned long) stepSize;
    
    // set the settings were changed flag
    sd_setSettingsChangedFlag();
  }

}




// ======================================================================================
// this function changes a numeric float value
// ======================================================================================
void uicore_changeValueFloat(float *value, float stepSize, float minimum, float maximum) {
  
  // if key pressed
  if ( isBit(key, KEY_UP) &&  
      ((*value < maximum) || (maximum == 0))
     ) {
    
    // no maximum
    if (maximum == 0) {
      
      *value += stepSize; // + (stepSize * key_count);   
      
    } 
    // maximum exists
    else {
    
      // would our next step exceed the maximum limit?           
      //if ((maximum - *value) > (key_count * stepSize)) {
      if ((maximum - *value) >= stepSize) {
        *value += stepSize; // + ((float) stepSize * (float) key_count);
      } else {
        *value = maximum;  
      }
    }
    
    // set the settings were changed flag
    sd_setSettingsChangedFlag();
    
  } else if ( isBit(key, KEY_DOWN) &&
             (*value > minimum)) {
    
    // would our next step exceed the minimum limit?           
    //if ((*value - minimum) > (key_count * stepSize)) {
    if ((*value - minimum) >= stepSize) {
      *value -= stepSize; // + ((float) stepSize * (float) key_count);
    } else {
      *value = minimum;  
    }
    
    // Avoid values just a little bit bigger than the minimum
    if (*value < (minimum + stepSize)) *value = minimum;
        
    // set the settings were changed flag
    sd_setSettingsChangedFlag();
  }

}


// ======================================================================================
uint8_t uicore_getBacklightLevel() {
  return uicore_backlight_level;  
}

// ======================================================================================
uint32_t uicore_getBacklightTime() {
  return uicore_backlight_time;  
}

// ======================================================================================
void uicore_setBacklightTime(uint32_t value) {
  uicore_backlight_time = value;  
}

// ======================================================================================
uint8_t uicore_getColorScheme() {
  return uicore_col_scheme;
}

// ======================================================================================
void uicore_setColorScheme(uint8_t value) {
  uicore_col_scheme = value;
}

// ======================================================================================
uint8_t uicore_getFont() {
  return uicore_font_index;
}

// ======================================================================================
void uicore_setFont(uint8_t value) {
  uicore_font_index = value;
}

// ======================================================================================
void uicore_endPopupMenu() {
  
  deleteBit(uicore_status, BIT_7);
  popup_menu_pos = 0;
  popup_menu_length = 0;
  
}



// ============================================================================
// this function is called by a timer interrupt
// ============================================================================
void uicore_handleTimer() {
  // set the repaint flag to force a repaint;
  uicore_setRepaintFlag();
}



// ============================================================================
// stop all the jog stuff
// ============================================================================
void uicore_stopJog() {
  
  // stop the jog-mode
  core_deleteJogModeFlag();
  // set the rotary to the key-simulation mode
  rotary.setKeyMode(true);
  // remove the damp zone
  rotary.disableStepDampZone();
  // clear the rotary position limits
  rotary.clearPositionLimits();
  // set rotary to pos 0
  rotary.setPosition(0);
  
  // Stop the timer
  Timer4.stop();
   
}






