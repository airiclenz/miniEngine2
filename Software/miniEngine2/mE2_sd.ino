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


#define CONFIG_FILE    "miniE.cnf"
#define BATTERY_FILE   "battery.cal"



////////////////////////////////////////////////////////
//                                                    //
//  V A R I A B L E S                                 //
//                                                    //
////////////////////////////////////////////////////////


// our file object to write to
File sd_file;
File sd_battery_file;

// general SD routines status flags
// B0 = sd ok & config file existent
// B1 = settings were changed
// B2 = save power calibration data
// B3 = sd ok & battery file existant
// B4 = 
// B5 = 
// B6 = 
// B7 = 
uint8_t sd_status = 0;

// the address where the data starts...
uint16_t sd_data_start;


byte sd_file_version;
byte sd_file_subversion;
byte sd_file_subsubversion;


boolean sddebugbool = true;
uint8_t sddebugbyte = 255;


// ============================================================================
void    sd_setSD_OK()                        { setBit(sd_status, BIT_0); }
void    sd_deleteSD_OK()                     { deleteBit(sd_status, BIT_0); }
boolean sd_isSD_OK()                         { return isBit(sd_status, BIT_0); }

// ============================================================================
void    sd_setSettingsChangedFlag()          { setBit(sd_status, BIT_1); }
void    sd_deleteSettingsChangedFlag()       { deleteBit(sd_status, BIT_1); }
boolean sd_isSettingsChangedFlag()           { return isBit(sd_status, BIT_1); }

// ============================================================================
void    sd_setSavePowerCalibrationFlag()     { setBit(sd_status, BIT_2); }
void    sd_deleteSavePowerCalibrationFlag()  { deleteBit(sd_status, BIT_2); }
boolean sd_isSavePowerCalibrationFlag()      { return isBit(sd_status, BIT_2); }

// ============================================================================
void    sd_setSDBat_OK()                     { setBit(sd_status, BIT_3); }
void    sd_deleteSDBat_OK()                  { deleteBit(sd_status, BIT_3); }
boolean sd_isSDBat_OK()                      { return isBit(sd_status, BIT_3); }



// ============================================================================
void sd_init() {
  
  // initialize the SD card 
  if (sd_initCard()) {
    // set the sd-is-ok flag
    sd_setSD_OK();
    // load the configuration
    sd_loadConfig();
  } else {
    sd_deleteSD_OK();   
  } 
  
}



// ============================================================================
void sd_process() {
  
  // only save the settings if the program is not running...
  
  if (!core_isProgramRunningFlag()) {
    
    
    
    
    if (!uicore_isEditing() &&
        sd_isSettingsChangedFlag() &&
        sd_isSD_OK()) {
          
      // save the settings
      sd_saveConfig();
      // delete the flag 
      sd_deleteSettingsChangedFlag();
    }  
    
  } // end: program running?
  
  
  // are we supposed to save the calibration data?
  // save this even if the program is running:
  
  if (sd_isSavePowerCalibrationFlag() && 
      sd_isSDBat_OK()) {

    // save the new power calibration data    
    sd_saveBatteryCalibration();
        
    // remove the flag that we need to save power data...    
    sd_deleteSavePowerCalibrationFlag();      
          
  }
  
}



// ============================================================================
boolean sd_initCard() {
  
  pinMode(PIN_SD_SS, OUTPUT);
  
  
  // can we access the SD card?
  if (SD.begin(PIN_SD_SS)) {
    
    
    /////////////////////////////////////////
    // check the battery file
    // does the battery calibration file exist?
    if (SD.exists(BATTERY_FILE)) {
      
      sd_battery_file = SD.open(BATTERY_FILE, FILE_READ);
      
      // could we open the battery calibration file?  
      if (sd_battery_file) {
        
        // load the battery calibration
        if (sd_loadBatteryCalibration()) {
          
          // set the OK flag for the battery file
          sd_setSDBat_OK();
          
        } else {
          
          // create a new file
          if (sd_saveBatteryCalibration()) {
            
            // set the OK flag for the battery file
            sd_setSDBat_OK();  
            
          } else {
            sd_deleteSDBat_OK();
          }
          
        }
        
        
      } else {
        sd_deleteSDBat_OK();
      }
      
    } else {
      
      // create a new battery calibration file
      if (sd_saveBatteryCalibration()) {
        
        // set the OK flag for the battery file
        sd_setSDBat_OK();
        
      } else {
        sd_deleteSDBat_OK();
      }
      
    }
      
    
    /////////////////////////////////////////
    // check the regular settings file
    // does the configuration file exist?
    if (SD.exists(CONFIG_FILE)) {
      
      sd_file = SD.open(CONFIG_FILE, FILE_READ);
      
      // could we open the config file?  
      if (sd_file) {
        
        // is the file version ok?
        if (sd_isVersionOK()) {
          
          return true;
          
        } else {
          
          // load the older version of the config
          sd_loadOldVersionSettings();
          
          // re create the new config file
          if (sd_saveConfig()) {
                        
            return true;
          }
          
        } // end: file version is not ok
                  
      } // end: config file could be opened
            
    } // end: config file does exist 
    
    // create the config file
    else {
      
      if (sd_saveConfig()) {
        
        return true;
      }
      
    } // end: file did not exist
    
  } // end: SD-reader init
  
  
  // if we land here, something went really wrong  
  return false;  
}



// ============================================================================
boolean sd_isVersionOK() {
  
  int i=0;
  byte lenght = 10;
  byte version[lenght];
    
  // read the first few bytes from the file
  while (sd_file.available() && (i < lenght)) {
    version[i] = sd_file.read();
    i++;
  }
  
  
  // remember the old version of the settings file
  sd_file_version =       version[1] - 48;
  sd_file_subversion =    version[3] - 48;
  sd_file_subsubversion = version[5] - 48;
    
  
  // check if the file content is as expected
  if ((version[0] == 118)                  &&  // v
      (version[1] == (VERSION + 48))       &&  // ASCII numbers start at 48
      (version[2] == 46)                   &&  // .
      (version[3] == (SUBVERSION + 48))    &&  // ASCII numbers start at 48
      (version[4] == 46)                   &&  // .
      (version[5] == (SUBSUBVERSION + 48)) &&  // ASCII numbers start at 48
      (version[6] == 13)                   &&  // LF
      (version[7] == 10)) {                    // CR
    
    return true;    
  }
  
  
  
  return false;
}



// ============================================================================
boolean sd_saveBatteryCalibration() {
    
  // if the file is open, close it
  if (sd_battery_file) {
    sd_battery_file.close();  
  }
  
  // delete the old config file
  SD.remove(BATTERY_FILE);
  
  // create a new file and open it
  sd_battery_file = SD.open(BATTERY_FILE, FILE_WRITE);
  
  if (sd_battery_file) {
    
    uint16_t value1 = power_getCalibrationEmpty(); 
    uint16_t value2 = power_getCalibrationFull(); 
    
    // create a byte array pointer to the value  
    byte* ptr;
    ptr = (byte*) &value1;  
    // save the content of the byte array value to the SD 
    // (basically it saves the value cut into bytes)
    sd_battery_file.write(ptr, 2);  
    
    // do the same for the other value
    ptr = (byte*) &value2;  
    sd_battery_file.write(ptr, 2);  
    
    // close the file
    sd_battery_file.close();
    
    return true;
  }
  
  return false;
  
}

// ============================================================================
boolean sd_saveConfig() {
    
  // if the file is open, close it
  if (sd_file) {
    sd_file.close();  
  }
  
  // delete the old config file
  SD.remove(CONFIG_FILE);
  
  // create a new file and open it
  sd_file = SD.open(CONFIG_FILE, FILE_WRITE);
  
  if (sd_file) {
    
    // save the version information
    sd_file.print(F("v"));
    sd_file.print(VERSION);
    sd_file.print(F("."));
    sd_file.print(SUBVERSION);
    sd_file.print(F("."));
    sd_file.print(SUBSUBVERSION);
    sd_file.write(13);
    sd_file.write(10);

        
    // backlight and UI layout    
    sd_writeData((byte)       uicore_getBacklightLevel());      // backlight power
    sd_writeData((uint32_t)   uicore_getBacklightTime());       // backlight time
    sd_writeData((byte)       uicore_getColorScheme());         // color scheme No.
    sd_writeData((byte)       uicore_getFont());                // the used font
    
    // core variables
    sd_writeData((byte)       core_mode);                       // the core mode
    sd_writeData((byte)       core_setup_style);                // the setup style
    sd_writeData((byte)       core_move_style);                 // the move style
    sd_writeData((byte)       core_settings);                   // some core settings 
    
    // camera data
    sd_writeData((byte)     isBit(cam_status, BIT_7));          // camera type
    sd_writeData((byte)       cam_fps);                         // clip fps value
    sd_writeData((uint32_t)   cam_exposure);                    // camera exposure     
    sd_writeData((byte)       cam_exposure_index);              // camera exposure  
    sd_writeData((uint32_t)   cam_focus);                       // camera focus time        
    sd_writeData((uint32_t)   cam_post_delay);                  // camera post delay time

    // setup data
    sd_writeData((uint32_t)   setup_record_time);               // the duration of the record period
    sd_writeData((uint32_t)   setup_play_time);                 // the length of the final clip 
          
            
    // motor data
    for (int i=0; i<DEF_MOTOR_COUNT; i++) {
      
      sd_writeData((byte)     motors[i].getType());             // motor 1 type
      sd_writeData((float)    motors[i].getCalibration());      // motor calibration  
      sd_writeData((float)    motors[i].getMaxSpeed());         // motor max speed  
      sd_writeData((float)    motor_ramp_time[i]);              // ramping time of the motor
      sd_writeData((uint32_t) motors[i].getPostDelay());        // motor post delay
      sd_writeData((byte)     motor_program_direction[i]);      // motor direction
      
      sd_writeData((float)    motor_total_distance[i]);         // the run-setup move dist 
      sd_writeData((byte)     motor_sleep[i]);                  // motor sleep after moves
      sd_writeData((byte)     motors[i].isDirectionFlipped());  // motor direction flipped
      sd_writeData((byte)     motors[i].isKeepPowered());       // keep the motor powered
      sd_writeData((byte)     motor_check_speed[i]);            // check the motor speed or not
        
        
      // a lille bit of motor specific run-setup-data
      sd_writeData((uint8_t)  setup_run_ramp_in[i]);            // the ramp-in-amount in percent for run setup
      sd_writeData((uint8_t)  setup_run_ramp_out[i]);           // the ramp-out-amount in percent for run setup
      
    }
    
    
    // trigger data
    for (int i=0; i<trigger_getTriggercount(); i++) {
      
      sd_writeData((byte)     trigger_isTriggerEnabled(i));     // trigger enabled status
      sd_writeData((byte)     trigger_getTriggerAction(i));     // trigger action
      sd_writeData((byte)     trigger_getTriggerType(i));       // trigger type
      sd_writeData((byte)     trigger_isTriggerDebounce(i));    // trigger type
    }
    
    
    // daisy chaining data
    sd_writeData((uint8_t)    com.getID());                     // the device ID in the daisy chain
    
    
    sd_file.close();
    
    return true;
  }
  
  return false;
  
}




// ============================================================================
boolean sd_loadBatteryCalibration() {
  
  // open the config file
  sd_battery_file = SD.open(BATTERY_FILE, FILE_READ);
    
  // if openeing the file succeeded
  if (sd_battery_file) {
    
    uint32_t fileSize = sd_battery_file.size();
    
    // create a byte buffer for storing the file in the RAM
    byte buffer[fileSize];
    
    // read the file into the buffer
    for (uint16_t i=0; i<fileSize; i++) {
      buffer[i] = sd_battery_file.read();
    }
    
    sd_battery_file.close();

        
    // has the file the right size of 4 byte?
    if (fileSize == 4) {
    
      // set the battery calibration
      power_setCalibrationEmpty( sd_readUInt(buffer, 0) );
      power_setCalibrationFull(  sd_readUInt(buffer, 2) );
      
      return true;
    
    }
  
  }
  
  return false;
  
}



// ============================================================================
boolean sd_loadConfig() {
  
  // open the config file
  sd_file = SD.open(CONFIG_FILE, FILE_READ);
    
  // if openeing the file succeeded
  if (sd_file) {
  
    uint32_t fileSize = sd_file.size();
    
    // create a byte buffer for storing the file in the RAM
    byte buffer[fileSize];
    
    // read the file into the buffer
    for (uint16_t i=0; i<fileSize; i++) {
      buffer[i] = sd_file.read();
    }
    
    sd_file.close();
    
    uint16_t address = 0;
    boolean dataFound = false;
    // look for the first line break
    while ((address < fileSize) && !dataFound) {
      
      if ((buffer[address + 0] == 13) && 
          (buffer[address + 1] == 10)) {
        
        address++;
        sd_data_start = address + 1;
        dataFound = true;
          
      }
      // go to the next charakter
      address++;  
    }
        
    
     
    // backlight and UI layout    
    uicore_setBacklightLevel(       sd_readByte  (buffer, address));            address += 1;
    uicore_setBacklightTime(        sd_readULong (buffer, address));            address += 4;
    uicore_setColorScheme(          sd_readByte  (buffer, address));            address += 1;
    uicore_setFont(                 sd_readByte  (buffer, address));            address += 1;
        
    // core variables
    core_mode =                     sd_readByte(buffer, address);               address += 1; 
    core_setup_style =              sd_readByte(buffer, address);               address += 1; 
    core_move_style =               sd_readByte(buffer, address);               address += 1; 
    core_settings =                 sd_readByte(buffer, address);               address += 1; 
    
    
    // camera data
    boolean camType =               (boolean) sd_readByte(buffer, address);     address += 1;
    if (camType) setBit(cam_status, BIT_7);
    else         deleteBit(cam_status, BIT_7);    
    
    cam_fps =                       sd_readByte(buffer, address);               address += 1;
    cam_exposure =                  (uint32_t) sd_readULong(buffer, address);   address += 4;
    cam_exposure_index =            sd_readByte(buffer, address);               address += 1;
    cam_focus =                     (uint32_t) sd_readULong(buffer, address);   address += 4;
    cam_post_delay =                (uint32_t) sd_readULong(buffer, address);   address += 4; 
    
    
    // setup data
    setup_record_time =             (uint32_t) sd_readULong(buffer, address);   address += 4; 
    setup_play_time =               (uint32_t) sd_readULong(buffer, address);   address += 4; 
       
        
    // motor data
    for (int i=0; i<DEF_MOTOR_COUNT; i++) {
      
      motors[i].setType(            sd_readByte  (buffer, address));            address += 1;
      motors[i].setCalibration(     sd_readFloat (buffer, address));            address += 4;
      motors[i].setMaxSpeed(        sd_readFloat (buffer, address));            address += 4;
      motor_ramp_time[i] =          sd_readFloat (buffer, address) ;            address += 4;
      motors[i].setPostDelay(       sd_readULong (buffer, address));            address += 4;
      motor_program_direction[i] =  (boolean) sd_readByte(buffer, address);     address += 1;
      
      motor_total_distance[i] =     sd_readFloat (buffer, address);             address += 4;
      motor_sleep[i] =              (boolean) sd_readByte(buffer, address);     address += 1; 
      motors[i].setDirectionFlipped((boolean) sd_readByte(buffer, address));    address += 1; 
      motors[i].setKeepPowered(     (boolean) sd_readByte(buffer, address));    address += 1;       
      motor_check_speed[i] =        (boolean) sd_readByte(buffer, address);     address += 1;
      
            
      // a lille bit of motor specific run-setup-data
      setup_run_ramp_in[i] =        sd_readByte(buffer, address);               address += 1; 
      setup_run_ramp_out[i] =       sd_readByte(buffer, address);               address += 1; 
      
    }
    
    
    // trigger data
    for (int i=0; i<trigger_getTriggercount(); i++) {
      
      boolean enabled =             (boolean) sd_readByte(buffer, address);     address += 1;
      byte action =                 sd_readByte(buffer, address);               address += 1;
      byte type =                   sd_readByte(buffer, address);               address += 1;
      boolean debounce =            (boolean) sd_readByte(buffer, address);     address += 1;
      
      trigger_setTriggerType(i, type);
      trigger_setTriggerAction(i, action);
      trigger_setDebounce(i, debounce);
      trigger_setEnabled(i, enabled);
      
    }
    
    // daisy chaining data
    uint8_t comId =                  sd_readByte(buffer, address);              address += 1;
    com.setID(comId);
    
    return true;
  }
  
  return false;
}



// ============================================================================
void sd_loadOldVersionSettings() {
    
  // open the config file
  sd_file = SD.open(CONFIG_FILE, FILE_READ);
    
  // if openeing the file succeeded
  if (sd_file) {
  
    uint32_t fileSize = sd_file.size();
    
    // create a byte buffer for storing the file in the RAM
    byte buffer[fileSize];
    
    // read the file into the buffer
    for (uint16_t i=0; i<fileSize; i++) {
      buffer[i] = sd_file.read();
    }
    
    sd_file.close();
    
    uint16_t address = 0;
    boolean dataFound = false;
    // look for the first line break
    while ((address < fileSize) && !dataFound) {
      
      if ((buffer[address + 0] == 13) && 
          (buffer[address + 1] == 10)) {
        
        address++;
        sd_data_start = address + 1;
        dataFound = true;
          
      }
      // go to the next charakter
      address++;  
    }
    
    if ((sd_file_version       == 2) &&
        (sd_file_subversion    == 0) &&
        (sd_file_subsubversion == 4)) {
          
      sd_loadVersion_2_0_4(buffer, address);        
    }
    
    if ((sd_file_version       == 2) &&
        (sd_file_subversion    == 0) &&
        (sd_file_subsubversion == 5)) {
          
      sd_loadVersion_2_0_5(buffer, address);        
    }
    
    if ((sd_file_version       == 2) &&
        (sd_file_subversion    == 0) &&
        (sd_file_subsubversion == 6)) {
          
      sd_loadVersion_2_0_6(buffer, address);        
    }

    if ((sd_file_version       == 2) &&
        (sd_file_subversion    == 0) &&
        (sd_file_subsubversion == 7)) {
          
      sd_loadVersion_2_0_7(buffer, address);        
    }
    
    
    // save the loaded battery calibration to the new separate battery file
    sd_saveBatteryCalibration();
    
  }
  
}


// ============================================================================
boolean sd_loadVersion_2_0_7(byte* buffer, uint16_t address) {
    
     
  // backlight and UI layout    
  uicore_setBacklightLevel(       sd_readByte  (buffer, address));            address += 1;
  uicore_setBacklightTime(        sd_readULong (buffer, address));            address += 4;
  uicore_setColorScheme(          sd_readByte  (buffer, address));            address += 1;
  uicore_setFont(                 sd_readByte  (buffer, address));            address += 1;
        
  // core variables
  core_mode =                     sd_readByte(buffer, address);               address += 1; 
  core_setup_style =              sd_readByte(buffer, address);               address += 1; 
  core_move_style =               sd_readByte(buffer, address);               address += 1; 
  core_settings =                 sd_readByte(buffer, address);               address += 1; 
  
  
  // camera data
  boolean camType =               (boolean) sd_readByte(buffer, address);     address += 1;
  if (camType) setBit(cam_status, BIT_7);
  else         deleteBit(cam_status, BIT_7);    
  
  cam_fps =                       sd_readByte(buffer, address);               address += 1;
  cam_exposure =                  (uint32_t) sd_readULong(buffer, address);   address += 4;
  cam_exposure_index =            sd_readByte(buffer, address);               address += 1;
  cam_focus =                     (uint32_t) sd_readULong(buffer, address);   address += 4;
  cam_post_delay =                (uint32_t) sd_readULong(buffer, address);   address += 4; 
  
  
  // setup data
  setup_record_time =             (uint32_t) sd_readULong(buffer, address);   address += 4; 
  setup_play_time =               (uint32_t) sd_readULong(buffer, address);   address += 4; 
     
      
  // motor data
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
    
    motors[i].setType(            sd_readByte  (buffer, address));            address += 1;
    motors[i].setCalibration(     sd_readFloat (buffer, address));            address += 4;
    motors[i].setMaxSpeed(        sd_readFloat (buffer, address));            address += 4;
    motor_ramp_time[i] =          sd_readFloat (buffer, address) ;            address += 4;
    motors[i].setPostDelay(       sd_readULong (buffer, address));            address += 4;
    motor_program_direction[i] =  (boolean) sd_readByte(buffer, address);     address += 1;
    
    motor_total_distance[i] =     sd_readFloat (buffer, address);             address += 4;
    motor_sleep[i] =              (boolean) sd_readByte(buffer, address);     address += 1; 
    motors[i].setDirectionFlipped((boolean) sd_readByte(buffer, address));    address += 1; 
    motors[i].setKeepPowered(     (boolean) sd_readByte(buffer, address));    address += 1;       
    motor_check_speed[i] =        (boolean) sd_readByte(buffer, address);     address += 1;
    
          
    // a lille bit of motor specific run-setup-data
    setup_run_ramp_in[i] =        sd_readByte(buffer, address);               address += 1; 
    setup_run_ramp_out[i] =       sd_readByte(buffer, address);               address += 1; 
    
  }
  
  
  // trigger data
  for (int i=0; i<trigger_getTriggercount(); i++) {
    
    boolean enabled =             (boolean) sd_readByte(buffer, address);     address += 1;
    byte action =                 sd_readByte(buffer, address);               address += 1;
    byte type =                   sd_readByte(buffer, address);               address += 1;
    boolean debounce =            (boolean) sd_readByte(buffer, address);     address += 1;
    
    trigger_setTriggerType(i, type);
    trigger_setTriggerAction(i, action);
    trigger_setDebounce(i, debounce);
    trigger_setEnabled(i, enabled);
    
  }

}



// ============================================================================
boolean sd_loadVersion_2_0_6(byte* buffer, uint16_t address) {
    
     
  // backlight and UI layout    
  uicore_setBacklightLevel(       sd_readByte  (buffer, address));            address += 1;
  uicore_setBacklightTime(        sd_readULong (buffer, address));            address += 4;
  uicore_setColorScheme(          sd_readByte  (buffer, address));            address += 1;
  uicore_setFont(                 sd_readByte  (buffer, address));            address += 1;
      
  // core variables
  core_mode =                     sd_readByte(buffer, address);               address += 1; 
  core_setup_style =              sd_readByte(buffer, address);               address += 1; 
  core_move_style =               sd_readByte(buffer, address);               address += 1; 
  core_settings =                 sd_readByte(buffer, address);               address += 1; 
  
  
  // camera data
  boolean camType =               (boolean) sd_readByte(buffer, address);     address += 1;
  if (camType) setBit(cam_status, BIT_7);
  else         deleteBit(cam_status, BIT_7);    
  
  cam_fps =                       sd_readByte(buffer, address);               address += 1;
  cam_exposure =                  (uint32_t) sd_readULong(buffer, address);   address += 4;
  cam_exposure_index =            sd_readByte(buffer, address);               address += 1;
  cam_focus =                     (uint32_t) sd_readULong(buffer, address);   address += 4;
  cam_post_delay =                (uint32_t) sd_readULong(buffer, address);   address += 4; 
  
  
  // setup data
  setup_record_time =             (uint32_t) sd_readULong(buffer, address);   address += 4; 
  setup_play_time =               (uint32_t) sd_readULong(buffer, address);   address += 4; 
     
      
  // motor data
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
    
    motors[i].setType(            sd_readByte  (buffer, address));            address += 1;
    motors[i].setCalibration(     sd_readFloat (buffer, address));            address += 4;
    motors[i].setMaxSpeed(        sd_readFloat (buffer, address));            address += 4;
    motor_ramp_time[i] =          sd_readFloat (buffer, address) ;            address += 4;
    motors[i].setPostDelay(       sd_readULong (buffer, address));            address += 4;
    motor_program_direction[i] =  (boolean) sd_readByte(buffer, address);     address += 1;
    
    motor_total_distance[i] =     sd_readFloat (buffer, address);             address += 4;
    motor_sleep[i] =              (boolean) sd_readByte(buffer, address);     address += 1; 
    motors[i].setDirectionFlipped((boolean) sd_readByte(buffer, address));    address += 1; 
    motors[i].setKeepPowered(     (boolean) sd_readByte(buffer, address));    address += 1;       
    motor_check_speed[i] =        (boolean) sd_readByte(buffer, address);     address += 1;
    
          
    // a lille bit of motor specific run-setup-data
    setup_run_ramp_in[i] =        sd_readByte(buffer, address);               address += 1; 
    setup_run_ramp_out[i] =       sd_readByte(buffer, address);               address += 1; 
    
  }
  
  
  // trigger data
  for (int i=0; i<trigger_getTriggercount(); i++) {
    
    boolean enabled =             (boolean) sd_readByte(buffer, address);     address += 1;
    byte action =                 sd_readByte(buffer, address);               address += 1;
    byte type =                   sd_readByte(buffer, address);               address += 1;
    boolean debounce =            (boolean) sd_readByte(buffer, address);     address += 1;
    
    trigger_setTriggerType(i, type);
    trigger_setTriggerAction(i, action);
    trigger_setDebounce(i, debounce);
    trigger_setEnabled(i, enabled);
    
  }

}




// ============================================================================
boolean sd_loadVersion_2_0_5(byte* buffer, uint16_t address) {
     
  // backlight and UI layout    
  uicore_setBacklightLevel(       sd_readByte  (buffer, address));            address += 1;
  uicore_setBacklightTime(        sd_readULong (buffer, address));            address += 4;
  uicore_setColorScheme(          sd_readByte  (buffer, address));            address += 1;
  uicore_setFont(                 sd_readByte  (buffer, address));            address += 1;
      
  // core variables
  core_mode =                     sd_readByte(buffer, address);               address += 1; 
  core_setup_style =              sd_readByte(buffer, address);               address += 1; 
  core_move_style =               sd_readByte(buffer, address);               address += 1; 
  core_settings =                 sd_readByte(buffer, address);               address += 1; 
  
  
  // camera data
  boolean camType =               (boolean) sd_readByte(buffer, address);     address += 1;
  if (camType) setBit(cam_status, BIT_7);
  else         deleteBit(cam_status, BIT_7);    
  
  cam_fps =                       sd_readByte(buffer, address);               address += 1;
  cam_exposure =                  (uint32_t) sd_readULong(buffer, address);   address += 4;
  cam_exposure_index =            sd_readByte(buffer, address);               address += 1;
  cam_focus =                     (uint32_t) sd_readULong(buffer, address);   address += 4;
  cam_post_delay =                (uint32_t) sd_readULong(buffer, address);   address += 4; 
  
  
  // setup data
  setup_record_time =             (uint32_t) sd_readULong(buffer, address);   address += 4; 
  setup_play_time =               (uint32_t) sd_readULong(buffer, address);   address += 4; 
     
      
  // motor data
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
    
    motors[i].setType(            sd_readByte  (buffer, address));            address += 1;
    motors[i].setCalibration(     sd_readFloat (buffer, address));            address += 4;
    motors[i].setMaxSpeed(        sd_readFloat (buffer, address));            address += 4;
    motor_ramp_time[i] =          sd_readFloat (buffer, address) ;            address += 4;
    motors[i].setPostDelay(       sd_readULong (buffer, address));            address += 4;
    motor_program_direction[i] =  (boolean) sd_readByte(buffer, address);     address += 1;
    
    motor_total_distance[i] =     sd_readFloat (buffer, address);             address += 4;
    motor_sleep[i] =              (boolean) sd_readByte(buffer, address);     address += 1; 
    motors[i].setDirectionFlipped((boolean) sd_readByte(buffer, address));    address += 1; 
    motors[i].setKeepPowered(     (boolean) sd_readByte(buffer, address));    address += 1;       
          
    // a lille bit of motor specific run-setup-data
    setup_run_ramp_in[i] =        sd_readByte(buffer, address);               address += 1; 
    setup_run_ramp_out[i] =       sd_readByte(buffer, address);               address += 1; 
    
  }
  
  
  // trigger data
  for (int i=0; i<trigger_getTriggercount(); i++) {
    
    boolean enabled =             (boolean) sd_readByte(buffer, address);     address += 1;
    byte action =                 sd_readByte(buffer, address);               address += 1;
    byte type =                   sd_readByte(buffer, address);               address += 1;
    boolean debounce =            (boolean) sd_readByte(buffer, address);     address += 1;
    
    trigger_setTriggerType(i, type);
    trigger_setTriggerAction(i, action);
    trigger_setDebounce(i, debounce);
    trigger_setEnabled(i, enabled);
    
  }
  
}



// ============================================================================
void sd_loadVersion_2_0_4(byte* buffer, uint16_t address) {
  
  // Battery calibration
  power_setCalibrationEmpty(      sd_readUInt  (buffer, address));            address += 2;
  power_setCalibrationFull(       sd_readUInt  (buffer, address));            address += 2;
   
  // backlight and UI layout    
  uicore_setBacklightLevel(       sd_readByte  (buffer, address));            address += 1;
  uicore_setBacklightTime(        sd_readULong (buffer, address));            address += 4;
  uicore_setColorScheme(          sd_readByte  (buffer, address));            address += 1;
  uicore_setFont(                 sd_readByte  (buffer, address));            address += 1;
      
  // core variables
  core_mode =                     sd_readByte(buffer, address);               address += 1; 
  core_settings =                 sd_readByte(buffer, address);               address += 1; 
  
  
  // camera data
  boolean camType =               (boolean) sd_readByte(buffer, address);     address += 1;
  if (camType) setBit(cam_status, BIT_7);
  else         deleteBit(cam_status, BIT_7);    
  
  cam_fps =                       sd_readByte(buffer, address);               address += 1;
  cam_exposure =                  (uint32_t) sd_readULong(buffer, address);   address += 4;
  cam_focus =                     (uint32_t) sd_readULong(buffer, address);   address += 4;
  cam_post_delay =                (uint32_t) sd_readULong(buffer, address);   address += 4; 
  
  
  // setup data
  setup_record_time =             (uint32_t) sd_readULong(buffer, address);   address += 4; 
  setup_play_time =               (uint32_t) sd_readULong(buffer, address);   address += 4; 
     
      
  // motor data
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
    
    motors[i].setType(            sd_readByte  (buffer, address));            address += 1;
    motors[i].setCalibration(     sd_readFloat (buffer, address));            address += 4;
    motors[i].setMaxSpeed(        sd_readFloat (buffer, address));            address += 4;
    motor_ramp_time[i] =          sd_readFloat (buffer, address) ;            address += 4;
    motors[i].setPostDelay(       sd_readULong (buffer, address));            address += 4;
    motor_program_direction[i] =  (boolean) sd_readByte(buffer, address);     address += 1;
    
    motor_total_distance[i] =     sd_readFloat (buffer, address);             address += 4;
    motor_sleep[i] =              (boolean) sd_readByte(buffer, address);     address += 1; 
    motors[i].setDirectionFlipped((boolean) sd_readByte(buffer, address));    address += 1; 
          
    // a lille bit of motor specific run-setup-data
    setup_run_ramp_in[i] =        sd_readByte(buffer, address);               address += 1; 
    setup_run_ramp_out[i] =       sd_readByte(buffer, address);               address += 1; 
    
  }
  
  
  // trigger data
  for (int i=0; i<trigger_getTriggercount(); i++) {
    
    boolean enabled =             (boolean) sd_readByte(buffer, address);     address += 1;
    byte action =                 sd_readByte(buffer, address);               address += 1;
    byte type =                   sd_readByte(buffer, address);               address += 1;
    boolean debounce =            (boolean) sd_readByte(buffer, address);     address += 1;
    
    trigger_setTriggerType(i, type);
    trigger_setTriggerAction(i, action);
    trigger_setDebounce(i, debounce);
    trigger_setEnabled(i, enabled);
    
  }
  
  
}



// ============================================================================
void sd_writeData(byte value) {
  sd_file.write(value);   
}

// ============================================================================
void sd_writeData(uint16_t value) {

  // create a byte array pointer to the value  
  byte* ptr;
  ptr = (byte*) &value;  
  // save the content of the byte array value to the SD 
  // (basically it saves the value cut into bytes)
  sd_file.write(ptr, 2);  
}

// ============================================================================
void sd_writeData(int16_t value) {
  
  // create a byte array pointer to the value  
  byte* ptr;
  ptr = (byte*) &value;  
  // save the content of the byte array value to the SD 
  // (basically it saves the value cut into bytes)
  sd_file.write(ptr, 2);  
}


// ============================================================================
void sd_writeData(uint32_t value) {
  
  // create a byte array pointer to the value  
  byte* ptr;
  ptr = (byte*) &value;  
  // save the content of the byte array value to the SD 
  // (basically it saves the value cut into bytes)
  sd_file.write(ptr, 4);  
}


// ============================================================================
void sd_writeData(int32_t value) {
  
  // create a byte array pointer to the value  
  byte* ptr;
  ptr = (byte*) &value;  
  // save the content of the byte array value to the SD 
  // (basically it saves the value cut into bytes)
  sd_file.write(ptr, 4);  
}



// ============================================================================
void sd_writeData(float value) {

  byte* floatPtr;
  floatPtr = (byte*) &value;  
  
  sd_file.write(floatPtr[0]);
  sd_file.write(floatPtr[1]);
  sd_file.write(floatPtr[2]);
  sd_file.write(floatPtr[3]);
   
}





// ============================================================================
uint8_t sd_readByte(byte* buf, uint16_t index) {

  return buf[index];
}


// ============================================================================
uint16_t sd_readUInt(byte* buf, uint16_t index) {

  return ((uint16_t) buf[index + 0] << 0) + 
         ((uint16_t) buf[index + 1] << 8); 
}


// ============================================================================
int16_t sd_readInt(byte* buf, uint16_t index) {

  return ((int16_t) buf[index + 0] << 0) + 
         ((int16_t) buf[index + 1] << 8); 
}


// ============================================================================
uint32_t sd_readULong(byte *buf, uint16_t index) {

  return ((uint32_t) buf[index + 0] <<  0) + 
         ((uint32_t) buf[index + 1] <<  8) +
         ((uint32_t) buf[index + 2] << 16) +
         ((uint32_t) buf[index + 3] << 24) ;  
}


// ============================================================================
long sd_readLong(byte* buf, uint16_t index) {

  return ((int32_t) buf[index + 0] <<  0) + 
         ((int32_t) buf[index + 1] <<  8) +
         ((int32_t) buf[index + 2] << 16) +
         ((int32_t) buf[index + 3] << 24) ;  
}
  
  
// ============================================================================
float sd_readFloat(byte* buf, uint16_t index) {
  
  float result;
  //uint8_t tmp;
	
  byte* floatPtr;
  floatPtr = (byte*) &result;
  
  for (uint16_t i=index; i < (index + 4);  i++) {
		
    // tmp = (uint8_t) buf[i];
    floatPtr[i - index] = (uint8_t) buf[i];
  }  
  
  return result;
  
}


// ============================================================================
void sd_deleteSettings() {
  
  if (sd_file) {
    sd_file.close();  
  }
  
  // delete the old config file
  SD.remove(CONFIG_FILE);
  
}




