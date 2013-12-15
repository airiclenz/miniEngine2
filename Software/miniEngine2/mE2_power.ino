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



#define BATTERY_AVG_BUF_LEN   10               // length of the average-value buffer (longer = slower changes)
#define BATTERY_DELAY         3000             // measure every X milliseconds



// array for storing the last battery values and calculating an average value  
// to filter out voltage drops created by short heavy loads
uint8_t  battery_avg_buffer[BATTERY_AVG_BUF_LEN]; 
// current position in the array
uint8_t  battery_avg_pos = 0;
// the time we measured the power supply the last time


uint32_t power_measure_time;
uint16_t battery_val_empty = 1023;        // initial value for auto-battery-calibration
uint16_t battery_val_full  = 0;           // initial value for auto-battery-calibration 
boolean  battery_show;                    // show battery information

// the battery level
uint16_t  battery_level;
uint16_t  battery_level_old;


// ============================================================================
// initializes the power / battery management
// ============================================================================
void power_init() {
  
  pinMode(PIN_BATTERY, INPUT);
  
  // init the battery average-buffer
  power_initBatteryBuffer();
  
  // show the battery information?
  battery_show = true;
}


// ============================================================================
// this function reads the power states every 3 seconds
// ============================================================================
void power_process() {
      
  if (power_measure_time + (unsigned long) BATTERY_DELAY <= now) {  
    
    // set the time we did the measurement the last time
    power_measure_time = now;  
    
    // read the charging level of the battery
    power_readBatteryLevel();
    
    // calculate the newest (averaged) power level
    power_calculateAvgBatteryLevel();
    
    // repaint the battery level
    if (battery_level_old != battery_level) {
      
      // set the battery repaint flag
      uicore_setRepaintBatteryFlag();
          
    }
    
  }
    
}



// ============================================================================
boolean power_isShowBattery() {
  return battery_show;
}

// ============================================================================
void power_setShowBattery(boolean value) {
   battery_show = value;
}

// ============================================================================
uint8_t power_getBatteryLevel() {
  return battery_level;  
}



// ===================================================================================
// this function returns the average battery level (value 0-7)
// ===================================================================================
void power_calculateAvgBatteryLevel() {
  
  float res = 0;
  
  for (uint8_t i=0; i<BATTERY_AVG_BUF_LEN; i++) {
    res += battery_avg_buffer[i];  
  }
  
  battery_level = round(res / (float) BATTERY_AVG_BUF_LEN);
  
}


// ============================================================================
// reads the power value from the battery
// ============================================================================
uint8_t power_readBatteryLevel() {
  
  // read the analog value
  uint16_t val = analogRead(PIN_BATTERY);

  boolean do_calibration = false;
    
  // do we have a new min value?
  if ((val < battery_val_empty) &&
      (val >= 300)) {
      
    battery_val_empty = val; 
    do_calibration = true;  
    
    // make that the new calibration data gets saved
    sd_setSavePowerCalibrationFlag();
  }
    
  // do we have a new max value?
  if (val > battery_val_full) {
      
    battery_val_full = val;
    do_calibration = true;
    
    // make that the new calibration data gets saved
    sd_setSavePowerCalibrationFlag();
  }
  
  
  /*
  // save the new battery calibration values  
  if (do_calibration) {
    eeprom_write_battery_calibration();      
  }
  */

  
  // map the measured value to our 8 displayable battery levels
  val = map(val, 
            battery_val_empty, battery_val_full,
            0, 100); 
    
  // constraint to these 100 (percent) levels  
  val = constrain(val, 0, 100); 

  // store the val in the battery average buffer array
  battery_avg_pos++;
  if (battery_avg_pos == BATTERY_AVG_BUF_LEN) battery_avg_pos = 0;
  battery_avg_buffer[battery_avg_pos] = val;  
 
  // set the new battery level 
  return val;   
    
}



// ============================================================================
// initializes the average buffer array with the first measured value to have 
// a fast battery status display
// ============================================================================
void power_initBatteryBuffer() {
  
  // read the battery
  byte curVal = power_readBatteryLevel();
      
  // set the whole array with this initial value
  for (int i=0; i<BATTERY_AVG_BUF_LEN; i++) {
    battery_avg_buffer[i] = curVal;  
  }
    
}



// ============================================================================
// sets the battery calibration values
// ============================================================================
void power_setCalibrationEmpty(uint16_t value) {
  battery_val_empty = value; 
}
void power_setCalibrationFull(uint16_t value) {
  battery_val_full = value; 
}


// ============================================================================
// returns the battery calibration values
// ============================================================================
uint16_t power_getCalibrationEmpty() {
  return battery_val_empty; 
}
uint16_t power_getCalibrationFull() {
  return battery_val_full; 
}

  
  
  
  
  



