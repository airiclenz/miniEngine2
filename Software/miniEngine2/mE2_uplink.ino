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




#define CMD_EngineStart               1
#define CMD_EngineStop                2
#define CMD_Repaint                   3



#define CMD_SetSetupStyleKeyframe     5
#define CMD_GetCurveLimit             6
#define CMD_GetMotorCount             7


#define CMD_SetActiveMotor            10
#define CMD_GetMotorType              11
#define CMD_SetMotorDirection         12
#define CMD_GetMotorPosition          13
#define CMD_MoveMotorToPosition       14
#define CMD_ClearMotorCurves          15
#define CMD_AddMotorCurve             16 
#define CMD_CheckCurves               17
#define CMD_GetMotorCalibration       18 
#define CMD_SetMotorCalibration       19


#define DEF_UPLINK_BUFFER_SIZE        64 
#define DEF_UPLINK_TIMEOUT            400


byte uplink_Buffer[DEF_UPLINK_BUFFER_SIZE];
byte uplink_received_count = 0;


// the variable that holds the last time something 
// happend on the com port
uint32_t  uplink_action_time = 0;





const char* uplink_handshake_ack = "mEStartAck";
const char* uplink_ack = "Ack";
const char* uplink_err = "Err";



byte xpos = 0;
byte ypos = 0;
byte lastbyte;


// ======================================================================================
// uplink processing
// ======================================================================================
void uplink_process() {
  
  
  // reset the timer and clear the buffer if nothing happend for to long
  if (((uplink_action_time + DEF_UPLINK_TIMEOUT) < millis()) &&
      (uplink_received_count > 0))  {
      uplink_received_count = 0;
  }
  
  
  
  // is there something in the inbox?
  if (Serial.available() > 0) {
    
    // remember when the last thing happened here
    uplink_action_time = millis();
    
    // as long as data is available and the buffer is not yet full:
    // recieve it
    while (Serial.available() &&
          (uplink_received_count < DEF_UPLINK_BUFFER_SIZE)) {
      
      byte data = (byte) Serial.read();
      uplink_Buffer[uplink_received_count] = data;
      uplink_received_count++;    
      
      //paintByte( data );     
        
    }
        
    // decode what we received;
    uplink_decode();
    
  }
  
}


// ======================================================================================
void uplink_decode() {
  
  // mEStart?
  if (uplink_received_count >= 9) {
    
    if ((uplink_Buffer[0] == 109) &&   // m
        (uplink_Buffer[1] ==  69) &&   // E
        (uplink_Buffer[2] ==  83) &&   // S
        (uplink_Buffer[3] == 116) &&   // t
        (uplink_Buffer[4] ==  97) &&   // a
        (uplink_Buffer[5] == 114) &&   // r
        (uplink_Buffer[6] == 116) &&   // t
        (uplink_Buffer[7] ==  13) &&   // line-
        (uplink_Buffer[8] ==  10)) {   // break
       
      Serial.println(uplink_handshake_ack);
      
      uplink_received_count = 0;    
    } 

  } // end: initial handshake
  
  
  ////////////////////////////////////
  // if we received the shortest possible messgae
  if (uplink_received_count == 3) {
    
    if ((uplink_Buffer[1] == 13) &&  // linebreak
        (uplink_Buffer[2] == 10)) {
          
      // Start
      if (uplink_Buffer[0] == CMD_EngineStart) {
        Serial.println(uplink_ack);
        core_startProgram();
        uplink_received_count = 0;
        return;
      }    
          
      // Stop
      if (uplink_Buffer[0] == CMD_EngineStop) {
        Serial.println(uplink_ack);
        core_stopProgram(com.isMaster() && (com.getSlaveCount() > 0));
        uplink_received_count = 0;
        return;
      }        
        
        
      // Repaint
      if (uplink_Buffer[0] == CMD_Repaint) {
        Serial.println(uplink_ack);
        uicore_setRepaintFlag();
        uplink_received_count = 0;
        return;
      }
          
      
      // Set Mode to Keyframes
      if (uplink_Buffer[0] == CMD_SetSetupStyleKeyframe) {
        Serial.println(uplink_ack);
        if (core_mode == MODE_PANORAMA) {
          core_mode = MODE_TIMELAPSE;  
        }
        
        //core_setup_style = SETUP_STYLE_KEYFRAMES;
        core_setup_style = SETUP_STYLE_REMOTE;
                
        // send the new mode to the clients i we have some;
        com_sendSystemMode(true);
        // reset the buffer
        uplink_received_count = 0;
        return;
      }
       
      // Get Curve Limit
      if (uplink_Buffer[0] == CMD_GetCurveLimit) {
        uplink_sendInteger(CURVE_COUNT);
        Serial.println();
        uplink_received_count = 0;
        return;
      } 
       
          
      // Get Axis Count
      if (uplink_Buffer[0] == CMD_GetMotorCount) {
        uplink_sendInteger(DEF_MOTOR_COUNT);
        Serial.println();
        uplink_received_count = 0;
        return;
      }
    
    
      // Get Axis Type
      if (uplink_Buffer[0] == CMD_GetMotorType) {
        Serial.write(motors[motor_selected].getType());
        Serial.println();
        uplink_received_count = 0;
        return;
      }
      
      
      // Clear Axis Curves
      if (uplink_Buffer[0] == CMD_ClearMotorCurves) {
        Serial.println(uplink_ack);
        motor_freeAllCurves(motor_selected);
        uplink_received_count = 0;
        return;
      }
      
      
      // Get Motor Position
      if (uplink_Buffer[0] == CMD_GetMotorPosition) {
        uplink_sendFloat(motors[motor_selected].getMotorPosition());
        Serial.println();
        uplink_received_count = 0;
        return;
      }
      
      
      // Check Curves
      if (uplink_Buffer[0] == CMD_CheckCurves) {
        Serial.println(uplink_ack);
        motor_checkKeyframes();
        uplink_received_count = 0;
        return;
      }
      
      
      // Get Motor Calibration
      if (uplink_Buffer[0] == CMD_GetMotorCalibration) {
        uplink_sendFloat(motors[motor_selected].getCalibration());
        Serial.println();
        uplink_received_count = 0;
        return;
      }
      
      
    } // end: linebreak
    
  } // end: packet size = 3
  
  
  
  
  ////////////////////////////////////
  // if we received a packet with 4 bytes
  else if (uplink_received_count == 4) {
    
    if ((uplink_Buffer[2] == 13) &&  // linebreak
        (uplink_Buffer[3] == 10)) {
          
          
      // set the active axis
      if (uplink_Buffer[0] == CMD_SetActiveMotor) {  
        
        if ((uplink_Buffer[1] < DEF_MOTOR_COUNT) &&
            (uplink_Buffer[1] >= 0)) {
          motor_selected = uplink_Buffer[1];
          Serial.println(uplink_ack);
        }
        
        uplink_received_count = 0;
        return;
      }
          
          
    } // end: linebreak
        
  } // end: packet size = 4
  
  
  
  
  
  ////////////////////////////////////
  // if we received a packet with 7 bytes
  else if (uplink_received_count == 7) {
  
    
    if ((uplink_Buffer[5] == 13) &&  // linebreak
        (uplink_Buffer[6] == 10)) {
  
          
      // Move the Motor to a specific position
      if (uplink_Buffer[0] == CMD_MoveMotorToPosition) {  
        
        // receive the float value from the buffer,
        // starting at position 1
        float pos = uplink_getFloatFromBuffer(1);
        // initiate the move
        motor_defineMoveToPosition(motor_selected, pos, true);  
        // send the acknowledge signal
        Serial.println(uplink_ack);
                
        // start the move
        motor_startMovesToPosition();
        
        uplink_received_count = 0;
        return;
      }
      
            
      
      // Move the Motor to a specific position
      if (uplink_Buffer[0] == CMD_SetMotorCalibration) {  
        
        // receive the float value from the buffer,
        // starting at position 1
        float cal = uplink_getFloatFromBuffer(1);
        // send the acknowledge signal
        Serial.println(uplink_ack);
                
        // set the new calibration value
        motors[motor_selected].setCalibration(cal);
        // set the flag that settings were changed
        sd_setSettingsChangedFlag();
        
        
        uplink_received_count = 0;
        return;
      }
  
          
    } // end: linebreak
        
  } // end: packet size = 7
  
  
  
  
  
  ////////////////////////////////////
  // if we received a packet with 35 bytes
  else if (uplink_received_count == 35) {
    
    if ((uplink_Buffer[33] == 13) &&  // linebreak
        (uplink_Buffer[34] == 10)) {
            
      // add a new motor curve
      if (uplink_Buffer[0] == CMD_AddMotorCurve) {  
                
        // assign a new curve for the motor
        int16_t cIndex = motor_assignNewCurve(motor_selected);
        
        // if another curve could be assigned
        if (cIndex != -1) {
          
          // send the acknowledge signal
          Serial.println(uplink_ack);
          
          // a curve object
          QuadBezierCurve curve;
          
          // set the curves data
          curve.p0 = Point(uplink_getFloatFromBuffer( 1), uplink_getFloatFromBuffer( 5));
          curve.p1 = Point(uplink_getFloatFromBuffer( 9), uplink_getFloatFromBuffer(13));
          curve.p2 = Point(uplink_getFloatFromBuffer(17), uplink_getFloatFromBuffer(21));
          curve.p3 = Point(uplink_getFloatFromBuffer(25), uplink_getFloatFromBuffer(29));
                    
          // and update the curves internal values
          curve.updateDimension();
          
          // now convert the curve to a usable version with linear segments
          //mCurves[cIndex].curve.segmentateCurveOptimized(curve);
          mCurves[cIndex].curve.segmentateCurveOptimized(curve);
          //mCurves[cIndex].curve.segmentateCurve(curve);
          
          // ...and reset all settings of the new curve
          mCurves[cIndex].curve.initMove();
          
        }
        
        uplink_received_count = 0;
        return;
      }
  
          
    } // end: linebreak
        
  } // end: packet size = 7
  
  
}



// ======================================================================================
void uplink_sendInteger(int value) {
  
  byte* bytePtr;
  bytePtr = (byte*) &value;
          
  Serial.write(bytePtr[0]); 
  Serial.write(bytePtr[1]); 
   
}


// ======================================================================================
void uplink_sendLong(long value) {
  
  byte* longPtr;
  longPtr = (byte*) &value;
          
  Serial.write(longPtr[0]); 
  Serial.write(longPtr[1]); 
  Serial.write(longPtr[2]); 
  Serial.write(longPtr[3]); 
   
}



// ======================================================================================
void uplink_sendFloat(float value) {
  
  byte* floatPtr;
  floatPtr = (byte*) &value;
          
  Serial.write(floatPtr[0]); 
  Serial.write(floatPtr[1]); 
  Serial.write(floatPtr[2]); 
  Serial.write(floatPtr[3]); 
  
}


// ======================================================================================
void uplink_sendDouble(double value) {
  
  byte* dblPtr;
  dblPtr = (byte*) &value;
          
  Serial.write(dblPtr[0]); 
  Serial.write(dblPtr[1]); 
  Serial.write(dblPtr[2]); 
  Serial.write(dblPtr[3]); 
  Serial.write(dblPtr[4]); 
  Serial.write(dblPtr[5]); 
  Serial.write(dblPtr[6]); 
  Serial.write(dblPtr[7]); 
  
}



// ============================================================================
float uplink_getFloatFromBuffer(byte offset) {
  	
  float result;
  uint8_t tmp;

  byte* floatPtr;
  floatPtr = (byte*) &result;

  for (byte i = offset; i < (offset + 4);  i++) {
    tmp = (uint8_t) uplink_Buffer[i];
    floatPtr[i - offset] = tmp;
  }  

   return result;
    
}





// ============================================================================
// DEBUGGING FUCNTION
// ============================================================================
void uplink_paintByte(byte data) {
  
  if ((xpos == 0) && (ypos == 0)) {
    tft.clrScr();  
  }
  
  char tmp[5];
  char byt[4];
  
  itoa(data, byt, 10);
  
  if (data < 10) {
    strcpy(tmp, "   ");
  } else if (data < 100) {
    strcpy(tmp, "  ");
  } else {
    strcpy(tmp, " ");
  }
  
  strcat(tmp, byt);
  
  tft.print(tmp, xpos * 8, ypos * 16);
  
  
  xpos+=4;
    
  if ((lastbyte == 13) && (data == 10)) {
    ypos++;
    xpos = 0;
  }
  
  if (xpos > 40) {
    ypos++;
    xpos = 0;
  }  
  
  if (ypos > 15) {
    xpos = 0;
    ypos = 0;
  }
  
  
  lastbyte = data;
  
  
}
