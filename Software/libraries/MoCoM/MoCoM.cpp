/*

 	Motion-Control-Communication library by Airic Lenz 2013
  
	Licensed under the GNU GPL Version 3.
  
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


#include "Arduino.h"
#include "MoCoM.h"


/*
// --------------------------
// defines the communication behaviour - variable "_status"
//
// B7 = is master
// B6 = is receiving (set if we are currently receiving a message)
// B5 = is in ID-request process
// B4 = is in waiting phase
// B3 = is blocked (when executing com internally)
// B2 =
// B1 =
// B0 =
*/





// ============================================================================

//#ifdef _VARIANT_ARDUINO_DUE_X_
#ifdef __arm__
MoCoM::MoCoM(byte pin_direction, USARTClass& serialPtr) :
#else
MoCoM::MoCoM(byte pin_direction, HardwareSerial& serialPtr) :
#endif

	// initialize the serial to be used
	_serial(serialPtr)

{
	//__setStatusBlocked();
	
	// set the direction pin
	_pinDirection = pin_direction;
	
	// set out client count to zero
	_slaveCount = 0;
	
	// clear the received command buffer
	_receivedCommand = MOCOM_COMMAND_EMPTY;
	
	// set the initial read delay (microSec)
	_readDelay = 200;
	
	//__deleteStatusBlocked();
	
}





// ============================================================================
// P U B L I C
// ============================================================================



// ============================================================================
void MoCoM::init(long baudrate)
{
	//__setStatusBlocked();
	
	// set the dir-pin mode
	pinMode(_pinDirection, OUTPUT);

	// set the com direction to "receive"
  	digitalWrite(_pinDirection, LOW);
	
	// slave device per default
	_status	= B00000000;  
	_ID = MOCOM_NO_ID;
	
	// set the read delay length
	_readDelay = (float) baudrate * 0.01;  // 0.0035

	__clearInput();
	
	_lastSendTime = 0;

	_selectedSlaveIndex = 255;

	// start the serial communication
	_serial.begin(baudrate);
	
	//__deleteStatusBlocked();

}


// ============================================================================
void MoCoM::registerFunction_TimeCritical(MoCoMCallbackFunction newFunction)
{
	_callback_RegularCalled = newFunction;
}

// ============================================================================
void MoCoM::registerFunction_CommunicationReceived(
										MoCoMCallbackFunction newFunction)
{
	_callback_PacketReceived = newFunction;
}


// ============================================================================
boolean MoCoM::isDataAvailable() {
	return _serial.available();
}

// ============================================================================
boolean MoCoM::executeCommunication() {
	
	//if (!_serial.available()) return false;
	
	//if (!__isStatusBlocked()) {
		
		// block this function
		//__setStatusBlocked();
		
		/*
		if (__isStatusWait()) {
			if (_waitEndTime <= millis()) {
		
				_waitEndTime = 0;
				__deleteStatusWait();
			
				if (_sendCharCnt > 0) {
					__send();
				}
			}
		}
		*/
	
		// check if we received anything 
		if (__receive()) {
  		
			// decode the packet
 		   __decode(true);
  		   
		   // delete the block for this function
		   //__deleteStatusBlocked();
		   
		   return true;
  	  	}
		
		// delete the block for this function
		//__deleteStatusBlocked();
		
	//} // end: not blocked 
	
  	return false;

}

// ============================================================================
byte MoCoM::getLastReceivedCommand() {
	
	byte res = _receivedCommand;
	_receivedCommand = MOCOM_COMMAND_EMPTY;
	return res;
}

// ============================================================================
byte MoCoM::getLastSenderID() {
	
	return _senderID;
}


// ============================================================================
boolean MoCoM::isMaster() 
{ 
	return (_status & B10000000);
}


// ============================================================================
boolean MoCoM::setID(byte id) {

	if ((id >= 0) && (id < MOCOM_MAX_SLAVES)) {
		
		// set the ID
		_ID = id;
		
		// are we a msdter now?
		if (_ID == MOCOM_MASTER_ID) {
			
			// set the master flag
			_status = B10000000;
			
			// reset all the slave data
			__clearSlaveData();
		
		} else {
						
			// delete the master flag
			bitClear(_status, 7);
			
		}
		
		return true;
	}
	
	return false;
}

// ============================================================================
boolean MoCoM::isIDFree(byte id) {
	return __ping(id);
} 


// ============================================================================
byte MoCoM::getID() {
	return _ID;
}


// ============================================================================
void MoCoM::registerSlaves() {
  
	// only allow this if we are a master
	if (isMaster()) {
		
		//__setStatusBlocked();
		
		// clear the salves array and all data of them
		__clearSlaveData();	
		
		// when did we start?				
		unsigned long starttime;	
		bool done;
		
		// loop through all possible slave IDs
		for (byte i=1; i<MOCOM_MAX_SLAVES; i++) {
		
			if (__ping(i)) {
							
				_slaves[_slaveCount].id = i;
				_slaveCount++;
				
				// when did we start?				
				starttime = millis();	
				done = false;
				
				// request the motor data from the client
				sendCommand(i, MOCOM_COMMAND_GET_MOTOR_DATA);
				
				while (((starttime + MOCOM_TIMEOUT) > millis()) && 
						done == false) {
														
					if (__receive()) {
						// refresh the time
						starttime = millis();
						
						// decode the package without callback to the parent software
						// this function takes care of registering the motor info packets
						__decode(false);
						
						if ((_receivedCommand == MOCOM_COMMAND_DONE) &&
							(_senderID == i)) {
							
							done = true;
						}
						
					} // we received a packet
					
				} // while receiving motor data
				
			} // ping was successful
						
		} // loop all possible slaves
		
		//__deleteStatusBlocked();
			
	} // we are a master
	
}



// ============================================================================
void MoCoM::sendClearMotorData() {
	sendCommand(MOCOM_MASTER_ID, MOCOM_COMMAND_CLEAR_MOTOR_DATA);
}


// ============================================================================
byte MoCoM::addMotor(byte type) {

	// slave command so only allow if we are a valid slave
	if (!isMaster()) {
		
		//__setStatusBlocked();
		
		// send the command to add a new motor
		sendCommand(MOCOM_MASTER_ID, MOCOM_COMMAND_MOTOR_ADD, type);
		// remember the time
		unsigned long time = millis();

		// wait until we received the motor ID
		while ((time + MOCOM_TIMEOUT) > millis()) {
			
			if (__receive()) {
     			
     			if (_receivedCommand == MOCOM_COMMAND_MOTOR_ID) {
					
					// save the data before unblocking
					byte res = _bufferData[0];
					
					//__deleteStatusBlocked();
					
     				return res;
     			} 
     		}
		}
		
		//__deleteStatusBlocked();
		
	}
	
	return  MOCOM_NO_ID;

};

// ============================================================================
void MoCoM::sendDone(byte id) {
	sendCommand(id, MOCOM_COMMAND_DONE);
}

// ============================================================================
void MoCoM::sendDone() {
	sendCommand(MOCOM_MASTER_ID, MOCOM_COMMAND_DONE);
}


// ============================================================================
void MoCoM::sendCommand(byte receiverID, byte command) {
    
  _bufferOut[_sendCharCnt++] = MOCOM_START_BYTE;
  _bufferOut[_sendCharCnt++] = receiverID;
  _bufferOut[_sendCharCnt++] = _ID;
  _bufferOut[_sendCharCnt++] = command; 
  _bufferOut[_sendCharCnt++] = 0; 			// no data
     
  __send();
}


// ============================================================================
void MoCoM::sendCommand(byte receiverID, byte command, byte data) {
  
  _bufferOut[_sendCharCnt++] = MOCOM_START_BYTE;
  _bufferOut[_sendCharCnt++] = receiverID;
  _bufferOut[_sendCharCnt++] = _ID;
  _bufferOut[_sendCharCnt++] = command; 
  _bufferOut[_sendCharCnt++] = 1; 
  _bufferOut[_sendCharCnt++] = data; 
   
  __send();
}



// ============================================================================
void MoCoM::sendCommand(byte receiverID, byte command, int data) {
  
	  _bufferOut[_sendCharCnt++] = MOCOM_START_BYTE;
	  _bufferOut[_sendCharCnt++] = receiverID;
	  _bufferOut[_sendCharCnt++] = _ID;
	  _bufferOut[_sendCharCnt++] = command; 
	  _bufferOut[_sendCharCnt++] = 2; 
	  _bufferOut[_sendCharCnt++] = data >> 8;
	  _bufferOut[_sendCharCnt++] = data;
     
	  __send();
}

// ============================================================================
void MoCoM::sendCommand(byte receiverID, byte command, long data) {
  
	_bufferOut[_sendCharCnt++] = MOCOM_START_BYTE;
	_bufferOut[_sendCharCnt++] = receiverID;
    _bufferOut[_sendCharCnt++] = _ID;
    _bufferOut[_sendCharCnt++] = command; 
    _bufferOut[_sendCharCnt++] = 4; 
    _bufferOut[_sendCharCnt++] = data >> 24;
    _bufferOut[_sendCharCnt++] = data >> 16;
    _bufferOut[_sendCharCnt++] = data >> 8;
    _bufferOut[_sendCharCnt++] = data;
      
    __send();
}


// ============================================================================
void MoCoM::sendCommand(byte receiverID, byte command, float data) {
  
	byte* floatPtr;
	floatPtr = (byte*) &data;
	_bufferOut[_sendCharCnt++] = MOCOM_START_BYTE;
	_bufferOut[_sendCharCnt++] = receiverID;
	_bufferOut[_sendCharCnt++] = _ID;
	_bufferOut[_sendCharCnt++] = command; 
	_bufferOut[_sendCharCnt++] = 4; 
	_bufferOut[_sendCharCnt++] = floatPtr[0];
	_bufferOut[_sendCharCnt++] = floatPtr[1];
	_bufferOut[_sendCharCnt++] = floatPtr[2];
	_bufferOut[_sendCharCnt++] = floatPtr[3];
     
	__send();
}



// ============================================================================
void MoCoM::sendCommand(byte receiverID, byte command, char* data) {
	
	int length = strlen(data);

	_bufferOut[_sendCharCnt++] = MOCOM_START_BYTE;
	_bufferOut[_sendCharCnt++] = receiverID;
	_bufferOut[_sendCharCnt++] = _ID;
	_bufferOut[_sendCharCnt++] = command; 
	_bufferOut[_sendCharCnt++] = length; 
  
	// check the max length of the data packet
	if ((length + 5) > MOCOM_BUFFER_LENGTH) length = MOCOM_BUFFER_LENGTH - 5;
  
	// add data to the buffer
	for (int i = 0; i < length; i++) {
		_bufferOut[_sendCharCnt++] = (byte) data[i];   
	}
   
	__send();
}



// ============================================================================
void MoCoM::sendCommand(byte receiverID, byte command, byte data1, byte data2) {
	
	_bufferOut[_sendCharCnt++] = MOCOM_START_BYTE;
	_bufferOut[_sendCharCnt++] = receiverID;
	_bufferOut[_sendCharCnt++] = _ID;
	_bufferOut[_sendCharCnt++] = command; 
	_bufferOut[_sendCharCnt++] = 2; 
	_bufferOut[_sendCharCnt++] = data1; 
	_bufferOut[_sendCharCnt++] = data2; 
   
	__send();
}


// ============================================================================
void MoCoM::sendCommand(byte receiverID, byte command, byte data1, int data2) {
	
	_bufferOut[_sendCharCnt++] = MOCOM_START_BYTE;
	_bufferOut[_sendCharCnt++] = receiverID;
	_bufferOut[_sendCharCnt++] = _ID;
	_bufferOut[_sendCharCnt++] = command; 
	_bufferOut[_sendCharCnt++] = 3; 
	_bufferOut[_sendCharCnt++] = data1; 
	_bufferOut[_sendCharCnt++] = data2 >> 8;
	_bufferOut[_sendCharCnt++] = data2;
   
	__send();
}


// ============================================================================
void MoCoM::sendCommand(byte receiverID, byte command, byte data1, long data2) {
	
	_bufferOut[_sendCharCnt++] = MOCOM_START_BYTE;
	_bufferOut[_sendCharCnt++] = receiverID;
	_bufferOut[_sendCharCnt++] = _ID;
	_bufferOut[_sendCharCnt++] = command; 
	_bufferOut[_sendCharCnt++] = 5; 
	_bufferOut[_sendCharCnt++] = data1; 
	_bufferOut[_sendCharCnt++] = data2 >> 24;
	_bufferOut[_sendCharCnt++] = data2 >> 16;
	_bufferOut[_sendCharCnt++] = data2 >> 8;
	_bufferOut[_sendCharCnt++] = data2;
   
	__send();
}


// ============================================================================
void MoCoM::sendCommand(byte receiverID, byte command, byte data1, float data2) {
	
	byte* floatPtr;
	floatPtr = (byte*) &data2;
	
	_bufferOut[_sendCharCnt++] = MOCOM_START_BYTE;
	_bufferOut[_sendCharCnt++] = receiverID;
	_bufferOut[_sendCharCnt++] = _ID;
	_bufferOut[_sendCharCnt++] = command; 
	_bufferOut[_sendCharCnt++] = 5; 
	_bufferOut[_sendCharCnt++] = data1; 
	_bufferOut[_sendCharCnt++] = floatPtr[0];
	_bufferOut[_sendCharCnt++] = floatPtr[1];
	_bufferOut[_sendCharCnt++] = floatPtr[2];
	_bufferOut[_sendCharCnt++] = floatPtr[3];
	   
	__send();
	
}




// ============================================================================
boolean MoCoM::isSlaveSelected() {
	return _selectedSlaveIndex != 255;
}

// ============================================================================
byte MoCoM::getSelectedSlaveIndex() {
	return _selectedSlaveIndex;
}


// ============================================================================
byte MoCoM::getSlaveCount() {
	return _slaveCount;
}


// ============================================================================
boolean MoCoM::selectSlave(byte index) {
	
	if ((_slaveCount > 0) &&
		(index >= _slaveCount) &&
		(index < _slaveCount)) {
		
		_selectedSlaveIndex = index;
		return true;	
	}	
	
	_selectedSlaveIndex = 255;
	return false;
}

// ============================================================================
boolean MoCoM::selectNextSlave() {
	
	if (_selectedSlaveIndex == 255) {
	
		if (_slaveCount > 0) {
			_selectedSlaveIndex = 0;
			return true;
		}
	
	} else if (_selectedSlaveIndex < (_slaveCount - 1)) {
		
		_selectedSlaveIndex++;
		return true;
	}
	
	_selectedSlaveIndex = 255;
	return false;
}

// ============================================================================
boolean MoCoM::selectPreviousSlave() {

	if (_selectedSlaveIndex == 255) {
		
		if (_slaveCount > 0) {
			_selectedSlaveIndex = _slaveCount - 1;
			return true;
		}
	} else if (_selectedSlaveIndex > 0) {
		
		_selectedSlaveIndex--;
		return true;
	}

	_selectedSlaveIndex = 255;
	return false;
}

// ============================================================================
void MoCoM::deselectSlave() {
	_selectedSlaveIndex = 255;
}



// ============================================================================
boolean MoCoM::ping(byte count, boolean deleteOnError) {
	
	if (_selectedSlaveIndex != 255) {
	
		for (byte i=0; i<count; i++) {
			
			byte id = _slaves[_selectedSlaveIndex].id;
			
			if (__ping(id)) return true;
		}
	
		if (deleteOnError) __deleteSlave(_selectedSlaveIndex);
	
	}
			
	return false;
}




// ============================================================================
byte MoCoM::getSlaveID() {
	
	if (_selectedSlaveIndex != 255) {
		return _slaves[_selectedSlaveIndex].id;
	} else {
		return MOCOM_NO_ID;
	}
}

// ============================================================================
byte MoCoM::getSlaveMotorCount() {
	
	if (_selectedSlaveIndex != 255) {
		return __getSlaveMotorCount(_selectedSlaveIndex);
	}
	
	return 0;
}

// ============================================================================
byte MoCoM::getSlavesTotalMotorCount() {
	
	byte sum = 0;
	
	for (int i=0; i<_slaveCount; i++) {
		sum += __getSlaveMotorCount(i);
	}
	
	return sum;	
}



// ============================================================================
byte MoCoM::getSlaveMotorType(byte motorID) {
	
	if (_selectedSlaveIndex != 255) {
	
		// which bit do we want to see (from the upper nibble)?
		byte mask = B00010000 << motorID;
	
		return !!(_slaves[_selectedSlaveIndex].motorData & mask);
	}
	
	return 255;
}


// ============================================================================
byte MoCoM::getDataByte(byte index) {
	
	if (_dataLength > index) {
		return _bufferData[index];
	}
	
	return 0;
}


// ============================================================================
int MoCoM::getDataInt(byte index) {
	
	if (_dataLength - 1 > index) {
		return __getNumberFromBuffer(index);
	}
	
	return 0;
	
}

// ============================================================================
long MoCoM::getDataLong(byte index) {
	
	if (_dataLength - 1 > index) {
		return __getNumberFromBuffer(index);
	}
	
	return 0;
}


// ============================================================================
float MoCoM::getDataFloat(byte index) {
	
	if (_dataLength - 1 > index) {
		return __getFloatFromBuffer(index);
	}
	
	return 0;
}



// ============================================================================
byte MoCoM::requestDataByte(byte code) {
	
	//__setStatusBlocked();

	if (__requestData(code)) {
				
		// store the data value before unblocking
		byte res = __getNumberFromBuffer(1);
		
		//__deleteStatusBlocked();
		
		return res;
	}
	
	//__deleteStatusBlocked();
	
	return 0;
		
}



// ============================================================================
int MoCoM::requestDataInt(byte code) {
	
	//__setStatusBlocked();

	if (__requestData(code)) {
				
		// store the data value before unblocking
		int res = __getNumberFromBuffer(1);
		
		//__deleteStatusBlocked();
		
		return res;
	}

	//__deleteStatusBlocked();
	
	return 0;
	
		
}

// ============================================================================
long MoCoM::requestDataLong(byte code) {
	
	//__setStatusBlocked();
	
	if (__requestData(code)) {
		
		// store the data value before unblocking
		long res = __getNumberFromBuffer(1);
		
		//__deleteStatusBlocked();
		
		return res;
	}
	
	//__deleteStatusBlocked();
	
	return 0;
		
}


// ============================================================================
float MoCoM::requestDataFloat(byte code) {
	
	//__setStatusBlocked();
	
	if (__requestData(code)) {
		
		// store the data value before unblocking
		float res = (float) __getFloatFromBuffer(1);
		
		//__deleteStatusBlocked();
		
		return res;
	}
	
	//__deleteStatusBlocked();
	
	return 0;
		
}







// ============================================================================
// P R I V A T E   F U N C T I O N S
// ============================================================================
 


// ============================================================================
void MoCoM::__send() {
 		
 	//if (!__isStatusWait() &&
	if ((_ID >= 0) &&
	 	(_ID <= MOCOM_MAX_SLAVES)) {
 		
		/*		
 		// wait a little after the last package was sent or received
	 	while (	((_lastSendTime + 1UL) > millis()) ||
				((_lastReceiveTime + 1UL) > millis()) ) {
					
	 		// call the function that needs to be called regularily
	  		if (_callback_RegularCalled != NULL) (*_callback_RegularCalled)();
	 	}
		*/
 	 	
	  	// transmit mode
 	 	digitalWrite(_pinDirection, HIGH);
   	  
 	 	// call the function that needs to be called regularily
 	 	if (_callback_RegularCalled != NULL) (*_callback_RegularCalled)();
  
 	 	// add the ckecksum
 	 	_bufferOut[_sendCharCnt] = __CRC(_bufferOut, _sendCharCnt);
 	 	_sendCharCnt++;
  	
		__comDelay(1); 
  	  
 	 	// send the packet
		for (int i = 0; i < _sendCharCnt; i++) {
 	 		_serial.write(_bufferOut[i]);
 	 	}
  
		// wait until all data is sent
  	  	_serial.flush();
 	 	__comDelay(3); 
		  	
 	 	// receive mode again
 	 	digitalWrite(_pinDirection, LOW);
	
		// remember the last time we sent a package
		_lastSendTime = millis();
		
		_sendCharCnt = 0;
	}

}


// ===================================================================
boolean MoCoM::__receive() {
		
	// ---------------------------- 
	// read the data
  	while ((_serial.available()) && 
         (_receivedCharCnt < MOCOM_BUFFER_LENGTH)) {
   
    	// read the data from the com buffer and save it in our array
    	_bufferIn[_receivedCharCnt] = (byte) _serial.read(); 
    	_receivedCharCnt++;
    
    	// delay a little while to receive the rest of the packet
    	delayMicroseconds(_readDelay); 
	} 
	
	// ---------------------------- 
	// timeout check
  	if ((millis() > (_receiveStartTime + MOCOM_TIMEOUT)) &&
    	(__isStatusReceiving())) {
    	
		__deleteStatusReceiving();
    	_receivedCharCnt = 0;
		    	
  	}
	
	// ---------------------------- 
	// check if we are starting with a new packet
  	if ((_receivedCharCnt > 0) &&
      	(!__isStatusReceiving()) && 
      	(_bufferIn[0] == (byte) MOCOM_START_BYTE)) {
      	
      	// start a new data packet
    	_receiveStartTime = millis();
    	__setStatusReceiving();

	}
	
	// ---------------------------- 
	// delete everything from the buffer-begin
	// until we have a valid start-byte there
	while (((_bufferIn[0] != (byte) MOCOM_START_BYTE) &&
		    (_receivedCharCnt > 0)) || 
		    ((_bufferIn[1] == (byte) MOCOM_START_BYTE) ||		// if we have 255 on pos 2 & 3
			 (_bufferIn[2] == (byte) MOCOM_START_BYTE))	) {		// in the packet, it is invalid
	
		_receivedCharCnt--;
		
		// move everything to 1 position up in the array
		for (int i=0; i<(_receivedCharCnt); i++) {
			_bufferIn[i] = _bufferIn[i+1];	
		}
	}
	
	
	// ---------------------------- 
	// check the packet structure
	if (_receivedCharCnt > 5) {
		
		boolean checkCRC = false;
		byte crcPos = _bufferIn[4] + 5;

		if ((_bufferIn[0] == (byte) MOCOM_START_BYTE) &&
			(_receivedCharCnt >= crcPos)) {
      		
      		checkCRC = true;
      	}
      	      	
      	// if the CRC value is valid
      	if (checkCRC) {
      		
      		if (_bufferIn[crcPos] == __CRC(_bufferIn, crcPos)) {
      		
				// is the packet for us?
        	  	if ((_bufferIn[1] == _ID) ||
    				(_bufferIn[1] == MOCOM_BROADCAST)) {
          			
					// remember the time when the packet was received
					_lastReceiveTime = millis();
				
	          		// store the last command & the sender's ID
					_receiverID = _bufferIn[1];
    	      		_senderID = _bufferIn[2];
        	  		_receivedCommand = _bufferIn[3];
          			_dataLength = _bufferIn[4];
          			
         			// copy the data
	          		for (int i=0; i<_dataLength; i++) {
    	      			_bufferData[i] = _bufferIn[i+5];
        	  		}
          			
          			__deletePacketFromBuffer(crcPos);
          			
          			return true;
          		}
          	
			}    	
      		
      		__deletePacketFromBuffer(crcPos);
		} 
	}
	
	return false;

}


// ===================================================================
void MoCoM::__deletePacketFromBuffer(byte crcPos) {
	
	// delete the package from the buffer if there is more in
	// the buffer than the last packet (everything in front
	// of and including crcPos) 
	if (_receivedCharCnt > (crcPos + 1)) {
          		
        // move everything to the front of the buffer	
    	for (int i=0; i<(_receivedCharCnt - crcPos - 1); i++) {
   			_bufferIn[i] = _bufferIn[crcPos + i + 1];
   		}
          			          
       	_receivedCharCnt -= (crcPos + 1);          
        		
   	} else {
   		
   		_receivedCharCnt = 0;
   	}
 	    	
  	__deleteStatusReceiving();
	
}




// ===================================================================
void MoCoM::__decode(boolean callBack) {
  	
  	unsigned long now = millis();
    	
   	// ---------------------------- 
   	// we received a PING
   	if (_receivedCommand == MOCOM_COMMAND_PING) {
   		
		/*
		// if global ping
		if (_receiverID == MOCOM_BROADCAST) {
			// respond with a delay based on our ID (ID number * 50ms)
			// this way all slaves answer after each other
			_waitEndTime = millis() + (_ID * 50);
			__setStatusWait(); 			
		}
		*/
		
		sendCommand(_senderID, MOCOM_COMMAND_ACK);
		
   	}
   	
	
   	// ---------------------------- 
   	// we received a WAIT request
   	if (_receivedCommand == MOCOM_COMMAND_WAIT) {
   		
   		// ignore this WAIT if our ID is in the data area of the package
   		if (_bufferData[0] != _ID) {
   			//_waitEndTime = millis() + 500UL + random(500);
   			//__setStatusWait();
   		}
   	}
   	   	
    	
   	// ---------------------------- 
   	if (_receivedCommand == MOCOM_COMMAND_CLEAR_MOTOR_DATA) {
   		// get the slave index in our array
   		byte slaveIndex = __getSlaveArrayPos(_senderID);
   		
   		// delete the motor info byte
   		_slaves[slaveIndex].motorData = 0;
   	}
   	
    	
    	
   	// ----------------------------    
   	if (_receivedCommand == MOCOM_COMMAND_MOTOR_ADD) {
   		
   		// get the slave index in our array
   		byte index = __getSlaveArrayPos(_senderID);
   		
   		if (index != 255) {    		
   		    		    		    		
   			// get the current count of the motors from the data-byte
   			byte motCnt = __getSlaveMotorCount(index);
   		    		    		
	  		// only assign a new motor if the max count was not reached yet
 	  		if (motCnt < MOCOM_MAX_MOTORS) {
	    			    		
 	 			// set the motor type bit:
   				byte mask = B00010000 << motCnt;
   				byte motType = _bufferData[0] << (4 + motCnt);
   				_slaves[index].motorData = (_slaves[index].motorData & ~mask) | (motType & mask);
    							
   				// send the motor ID
   				sendCommand(_senderID, MOCOM_COMMAND_MOTOR_ID, motCnt); 
    			
   				// increment the motor count
   				motCnt++;
    			
   				// set the new motor count
   				mask = B00001111;
   				_slaves[index].motorData = (_slaves[index].motorData & ~mask) | (motCnt & mask);
    			    			
   			} else {
   				sendCommand(_senderID, MOCOM_COMMAND_TOO_MANY_MOTORS);
   			}
   		}
   	}
    	 
    	 
   	// call the external function that handles further actions?    	
   	if (callBack && (_callback_PacketReceived != NULL)) {
		// call the external function to handle the data   
		(*_callback_PacketReceived)();
   	}
 
    
}


// ============================================================================
boolean MoCoM::__ping(byte id) {
	
	// remember the old block status
	//boolean oldBlock = __isStatusBlocked();

	//__setStatusBlocked();

	sendCommand(id, MOCOM_COMMAND_PING);

	unsigned long starttime = millis();	

	while ((starttime + MOCOM_TIMEOUT) > millis()) {

		if (__receive()) {

			if ((_receivedCommand == MOCOM_COMMAND_ACK) &&
				(_senderID == id)) {
				
				//if (!oldBlock) __deleteStatusBlocked();
													
				return true;	
			}
		}
		
		//if (!oldBlock) __deleteStatusBlocked();
				
	} 
	
	return false;
}



// ============================================================================
byte MoCoM::__getSlaveArrayPos(byte slaveID) {
	
	for (byte i=0; i<_slaveCount; i++) {
		
		if (_slaves[i].id == slaveID) {
			return i;
		}
	}
	
	return 255;
}

// ============================================================================
void MoCoM::__deleteSlave(byte slaveIndex) {
	
	if ((slaveIndex != _slaveCount-1) &&
	 	(_slaveCount > 1)) {		
	
		for (int i=slaveIndex; i<(_slaveCount-1); i++) {
			_slaves[i] = _slaves[i+1];
			
		}
		
	}
	
	_selectedSlaveIndex = 255;
	_slaveCount--;
}


// ============================================================================
byte MoCoM::__getSlaveMotorCount(byte index) {
	
	// returns the number stored in the right 4 bits of the motor-data-byte
	return _slaves[index].motorData & ~(0xFFFFFFFF << 4);
}




/*
// ============================================================================
boolean MoCoM::__isStatusWait()         
{ 
	return _status & B00010000; 
}

// ============================================================================
void MoCoM::__setStatusWait() 
{ 
	_status |= B00010000; 
}

// ============================================================================
void MoCoM::__deleteStatusWait() 
{ 
	bitClear(_status, 4); // B11101111
}
*/


// ============================================================================
boolean MoCoM::__isStatusReceiving() 
{ 
	return _status & B01000000; 
}  

// ============================================================================
void MoCoM::__setStatusReceiving() 
{ 
	_status |= B01000000; 
}

// ============================================================================
void MoCoM::__deleteStatusReceiving() 
{ 
	bitClear(_status, 6); // B10111111
}



/*

// ============================================================================
boolean MoCoM::__isStatusBlocked() 
{ 
	return _status & B00001000; 
}  

// ============================================================================
void MoCoM::__setStatusBlocked() 
{ 
	_status |= B00001000; 
}

// ============================================================================
void MoCoM::__deleteStatusBlocked() 
{ 
	bitClear(_status, 3); // B11110111
}

*/




// ============================================================================
boolean MoCoM::__requestData(byte code) {
		
	if (_selectedSlaveIndex != 255) {
		
		// request the motor data from the client
		sendCommand(_slaves[_selectedSlaveIndex].id, MOCOM_COMMAND_GET_DATA, code);
		
		unsigned long starttime = millis();
		
		//boolean printIn = true;
		
		while ((starttime + MOCOM_TIMEOUT) > millis()) {
														
			if (__receive()) {
									
				// decode the package without callback to the parent software
				// this function takes care of registering the motor info packets
				__decode(false);
			
				if ((_receivedCommand == MOCOM_COMMAND_DATA) &&
					(_senderID == _slaves[_selectedSlaveIndex].id) &&
					(code = _bufferData[0])) {

						return true; 
				} 
			
			} // we received a packet
		
		} // while receiving motor data

	} // a valid slave is selected
	
	return false;
	
}




// ============================================================================
long MoCoM::__getNumberFromBuffer(byte offset) {
  
  	uint32_t tmp = 0;
  	uint32_t res = 0;
  	
  	for (byte i=offset; i < _dataLength;  i++) {
  	  	tmp = (uint8_t) _bufferData[i];
    	tmp = tmp << ((_dataLength - 1 - i) * 8);
    	res += tmp;
  	}

  	return (long) res;
    
}


// ============================================================================
float MoCoM::__getFloatFromBuffer(byte offset) {
  	
	float result;
  	uint8_t tmp;
	
	byte* floatPtr;
	floatPtr = (byte*) &result;
  
  	for (byte i=offset; i < _dataLength;  i++) {
		
		tmp = (uint8_t) _bufferData[i];
		floatPtr[i - offset] = tmp;
  	}  

  return result;
    
}



// ============================================================================
void MoCoM::__clearSlaveData() {
	
	// reset all the slave data
	_slaveCount = 0;
	
	for (int i=0; i<MOCOM_MAX_SLAVES; i++) {
		_slaves[i].id = MOCOM_NO_ID;
		_slaves[i].motorData = 0;
	}	
	
}


// ============================================================================
uint8_t MoCoM::__CRC(byte buffer[], byte len)
{
  	uint8_t crc=0;
  
  	// call the function that needs to be called regularily
  	if (_callback_RegularCalled != NULL) (*_callback_RegularCalled)();
  
  	for (uint8_t i=0; i<len;i++) {
    
    	// call the function that needs to be called regularily
  		if (_callback_RegularCalled != NULL) (*_callback_RegularCalled)();
  	
    	uint8_t inbyte = buffer[i];
    
    	for (uint8_t j=0;j<8;j++) {
	
      		// call the function that needs to be called regularily
  			if (_callback_RegularCalled != NULL) (*_callback_RegularCalled)();
      		
      		uint8_t mix = (crc ^ inbyte) & 0x01;
      		crc >>= 1;
	
      		if (mix) crc ^= 0x8C;

      		inbyte >>= 1;
    	}
  	}
  
  	return crc;
} 


// ===================================================================================
void MoCoM::__comDelay(unsigned int milliseconds) {
  
	unsigned long startTime = millis();

  	while ((startTime + milliseconds) > millis()) {
    
    	// call the function that needs to be called regularily
		if (_callback_RegularCalled != NULL) (*_callback_RegularCalled)();
   
  	} 
    
}

// ===================================================================================
void MoCoM::__clearInput() {
		
	// clear the in-buffer to delete anything that might be waiting there
	_receivedCharCnt = 0;
	__deleteStatusReceiving();
}
