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


////////////////////////////////////////////////////////
//                                                    //
//  C O M   V A R I A B L E S                         //
//                                                    //
////////////////////////////////////////////////////////


// the communication-status byte
// B0 = sync signal received      
// B1 = program stop flag         
// B2 = prepare signal
// B3 = acknowledge received               
// B4 =       
// B5 = 
// B6 = 
// B7 = is registered Slave     
byte com_status = 0;



// an array to store some ui values for every slave
// B0 = slave is ready  
// B1 = start point is set        
// B2 = end point is set
// B3 = 
// B4 = 
// B5 = 
// B6 = 
byte com_slaveStati[MOCOM_MAX_SLAVES];



boolean com_isSyncFlag()                       { return isBit(com_status, BIT_0); }
void    com_setSyncFlag()                      { setBit(com_status, BIT_0); }
void    com_deleteSyncFlag()                   { deleteBit(com_status, BIT_0); } 

boolean com_isStopFlag()                       { return isBit(com_status, BIT_1); }
void    com_setStopFlag()                      { setBit(com_status, BIT_1); }
void    com_deleteStopFlag()                   { deleteBit(com_status, BIT_1); } 

boolean com_isPrepareFlag()                    { return isBit(com_status, BIT_2); }
void    com_setPrepareFlag()                   { setBit(com_status, BIT_2); }
void    com_deletePrepareFlag()                { deleteBit(com_status, BIT_2); } 

boolean com_isAcknowledgeFlag()                { return isBit(com_status, BIT_3); }
void    com_setAcknowledgeFlag()               { setBit(com_status, BIT_3); }
void    com_deleteAcknowledgeFlag()            { deleteBit(com_status, BIT_3); } 

boolean com_isRegisteredSlaveFlag()            { return isBit(com_status, BIT_7); }
void    com_setRegisteredSlaveFlag()           { setBit(com_status, BIT_7); }
void    com_deleteRegisteredSlaveFlag()        { deleteBit(com_status, BIT_7); } 



// ============================================================================
void com_init() {
  
  com.init(57600);  
  com.registerFunction_CommunicationReceived(com_handleEvents);
  com.setID(MOCOM_MASTER_ID);
  
  com.clearSlaveData();
  
  // delete the status of all slaves
  com_clearSlaveStatuses();
}


// ============================================================================
void com_process() {
  
  com.executeCommunication();
  
  if (com_isPrepareFlag()) {
    // start the program
    core_startProgram();
  }
  
}


// ============================================================================
void com_handleEvents() {
  
  byte command = com.getLastReceivedCommand();
  byte senderId = com.getLastSenderID();
  
  
  // ---------------------
  // we received a COMMAND
  // ---------------------
  
  
  
  // ---------------------
  // actually start the engine
  if ((command == MOCOM_COMMAND_SYNC) &&
      (com.isMaster() == false)) {
    
    // set the sync received flag
    com_setSyncFlag();
    
    return;
  }
  
  
  // ---------------------
  // initialize the start of the engine
  if ((command == MOCOM_COMMAND_PREPARE) &&
      (com.isMaster() == false)) {
        
    if (core_isProgramRunningFlag()) core_stopProgram(false); 
    com_setPrepareFlag();
    return;
  }
  
  
  // ---------------------
  // initialize the PREVIEW
  if ((command == MOCOM_COMMAND_PREVIEW) &&
      (com.isMaster() == false)) {
        
    
    com_setPrepareFlag();
    return;
  }
  
  
  // ---------------------
  // camera shoot
  if ((command == MOCOM_COMMAND_CAMERA_SHOOT) &&
      (com.isMaster() == false)) {
    
    // set the sync received flag
    com_setSyncFlag();
    return;
  }
  
  
  // ---------------------
  // we received an Acknowledge from the master
  if ((command == MOCOM_COMMAND_ACK) &&
      (com.isMaster() == false)) {
        
    com_setAcknowledgeFlag();
    return;
  }
  
  
  // ---------------------
  // stop the engine
  if ((command == MOCOM_COMMAND_STOP) &&
      (com.isMaster() == false)) {
    
    com_setStopFlag();
    core_stopProgram(false);
    return;
  }
  
  
  // ---------------------
  // motor data requested
  if (command == MOCOM_COMMAND_GET_MOTOR_DATA) {
    
    com.sendClearMotorData();
  
    for (int i=0; i<DEF_MOTOR_COUNT; i++) {
      com.addMotor(motors[i].getType());  
    }

    // send the signal that we have no more motors
    com.sendDone();
    // remember that we are now a registered slave device
    com_setRegisteredSlaveFlag();
    // do a full repaint to update possible menu changes
    uicore_repaint(true);
    // paint the message that shows that the chain is updated...
    uicore_showMessage(233, 236, 236, 2000);
    return;
  }
  
  
  // ---------------------
  // Done Command received
  if ((command == MOCOM_COMMAND_DONE) && 
       com.isMaster()) {
    
    // send the acknowledge to the slave
    com.sendCommand(senderId, MOCOM_COMMAND_ACK);  
        
    // deselect the prossibly selected slave
    com.deselectSlave();
            
    // loop all slaves we have registered
    while (com.selectNextSlave()) {
      
      // is the ID of the slave the ID we are looking for?
      if (senderId == com.getSlaveID()) {
        
        setBit(com_slaveStati[com.getSelectedSlaveIndex()], BIT_0);
        return;
      }
      
    }
        
  }
  
  
  
  
  
  
  // ---------------------
  // we received data
  // ---------------------
  if (command == MOCOM_COMMAND_DATA) {
    
    byte code = com.getDataByte(0);   
        
    switch (code) {
      
      // system mode
      case MOCOM_DATA_SYSTEMMODE :    {
        
                  // set the new move style
                  core_mode = com.getDataByte(1); 
                  // check everything for validity
                  core_checkModes();
                  // do a full repaint
                  uicore_repaint(true);
                  // set the flag that settings were changed
                  sd_setSettingsChangedFlag(); 
                  
                  break;
      }
      
      // system mode
      case MOCOM_DATA_SETUPSTYLE :    {
        
                  // set the new move style
                  core_setup_style = com.getDataByte(1); 
                  // check everything for validity
                  core_checkModes();
                  // do a full repaint
                  uicore_repaint(true);
                  // set the flag that settings were changed
                  sd_setSettingsChangedFlag(); 
                  
                  break;
      }
      
      // move style
      case MOCOM_DATA_MOVESTYLE :    {
        
                  // set the new move style
                  core_move_style = com.getDataByte(1); 
                  // check everything for validity
                  core_checkModes();
                  // do a full repaint
                  uicore_repaint(true);
                  // set the flag that settings were changed
                  sd_setSettingsChangedFlag(); 
                  break;
      }
      
      // record time
      case MOCOM_DATA_RECORDTIME :    {
        
                  // set the new move style
                  setup_record_time = (uint32_t) com.getDataLong(1); 
                  // check everything for validity
                  core_checkValues();
                  // set the flag that settings were changed
                  sd_setSettingsChangedFlag(); 
                  
                  break;
      }
      
      // playback time
      case MOCOM_DATA_PLAYTIME :    {
        
                  // set the new move style
                  setup_play_time = (uint32_t) com.getDataLong(1); 
                  // check everything for validity
                  core_checkValues();
                  // set the flag that settings were changed
                  sd_setSettingsChangedFlag(); 
                  
                  break;
      }
      
      // camera FPS
      case MOCOM_DATA_PLAYFPS :    {
        
                  // set the new move style
                  cam_fps = (uint8_t) com.getDataByte(1); 
                  // check everything for validity
                  core_checkValues();
                  // set the flag that settings were changed
                  sd_setSettingsChangedFlag(); 
                  
                  break;
      }
      
      
      // Loop
      case MOCOM_DATA_LOOP :    {
        
                  // set the new move style
                  boolean value = (boolean) com.getDataByte(1); 
                  // set the actual variable
                  if (value) core_setLoopFlag();
                  else core_deleteLoopFlag();
                  // set the flag that settings were changed
                  sd_setSettingsChangedFlag(); 
                  
                  break;
      }


      // Endless Shoot
      case MOCOM_DATA_ENDLESSSHOOT :    {
        
                  // set the new move style
                  boolean value = (boolean) com.getDataByte(1); 
                  // set the actual variable
                  if (value) core_setEndlessShootFlag();
                  else core_deleteEndlessShootFlag();
                  // set the flag that settings were changed
                  sd_setSettingsChangedFlag(); 
                  
                  break;
      }
      
      

      
      
    } // end: switch code
  
  } // end: command == MOCOM_COMMAND_DATA 
  
  
}




// ===================================================================================
// sends the current system setup to one or all slaves
// ===================================================================================
void com_sendSystemMode(boolean all) {
    
  if (com.isMaster() &&
      (com.getSlaveCount() > 0)
     ){
  
    // send the code for "mode" to all clients
    com_sendCommandData(MOCOM_DATA_SYSTEMMODE, core_mode, all);
    // send the code for "setup style" to all clients
    com_sendCommandData(MOCOM_DATA_SETUPSTYLE, core_setup_style, all);
    // set the "move style" to the same as we have now
    com_sendCommandData(MOCOM_DATA_MOVESTYLE, core_move_style, all);  
  }
  
}


// ===================================================================================
// sends the main setp-data if in an SMS mode
// ===================================================================================
void com_sendRunData() {
  
  if (com.isMaster() &&
      (com.getSlaveCount() > 0)
     ){
  
    if (core_mode == MODE_TIMELAPSE) 
    {
      com.sendCommand(MOCOM_BROADCAST, MOCOM_COMMAND_DATA, MOCOM_DATA_RECORDTIME, (long) setup_record_time);
      com.sendCommand(MOCOM_BROADCAST, MOCOM_COMMAND_DATA, MOCOM_DATA_PLAYTIME,   (long) setup_play_time);
      com.sendCommand(MOCOM_BROADCAST, MOCOM_COMMAND_DATA, MOCOM_DATA_PLAYFPS,    cam_fps);    
    } 
    
    else if (core_mode == MODE_VIDEO) 
    {    
      com.sendCommand(MOCOM_BROADCAST, MOCOM_COMMAND_DATA, MOCOM_DATA_RECORDTIME, (long) setup_record_time);    
    }
  }

}


// ===================================================================================
void com_sendLoopData() {
  
  if (com.isMaster() &&
      (com.getSlaveCount() > 0)
     ){
       
    com_sendCommandData(MOCOM_DATA_LOOP, (byte) core_isLoopFlag(), true);
  }
  
}


// ===================================================================================
void com_sendEndlessShootData()
{
  
  if (com.isMaster() &&
      (com.getSlaveCount() > 0)
     ){
       
    com_sendCommandData(MOCOM_DATA_ENDLESSSHOOT, (byte) core_isEndlessShootFlag(), true);
  }
  

  
}

// ===================================================================================
// this function sends a code plus a data-byte to the curretly selected,
// or to all slaves
// ===================================================================================
void com_sendCommandData(byte code, byte data, boolean all) {
  
  if (all) {
    
    com.sendCommand(MOCOM_BROADCAST, MOCOM_COMMAND_DATA, (byte) code, (byte) data);
    
  } else {

    com.sendCommand(com.getSlaveID(), MOCOM_COMMAND_DATA, (byte) code, (byte) data);
  }
 
}


// ===================================================================================
// sends a done to the master until an acknowledge command is received
// ===================================================================================
void com_sendDoneWithAck() {
    
  // create a random delay for avoiding sending at the same interval
  int randDelay = 0;
  
  // the time variable to remember when we sent the "done" command
  uint32_t timeSent = 0;
  
  // loop until we jump out of this function
  while (1) {
    
    // do we need to send a new mail?
    if ((timeSent + randDelay) < millis()) {
      
      // send a new done command
      com.sendDone();
      
      // remember the tine
      timeSent = millis();
      // generate a new random delay       
      randDelay = random(500);  
    }
    
    // process the communication
    // --> gets processes in the com_handle_events()
    com.executeCommunication();    
    
    // did we receive the acknowledge from the master?
    if (com_isAcknowledgeFlag()) {
      
      com_deleteAcknowledgeFlag();
      return;   
    } 
    
    /*
    // leave on key press
    if (keys_get_key_pressed() == KEY_SELECT) {
      return;  
    }
    */
    
  } // end: eternal loop
   
}



// ============================================================================
void com_clearSlaveStatuses() 
{
  for (int i=0; i<MOCOM_MAX_SLAVES; i++) {
    com_slaveStati[i] = 0;
  }
}


// ============================================================================
void com_clearSlavesReady() 
{
  for (int i=0; i<MOCOM_MAX_SLAVES; i++) {
    deleteBit(com_slaveStati[i], BIT_0);
  }
}



