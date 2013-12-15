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


#ifndef MoCoM_h
#define MoCoM_h


#include "Arduino.h"

//#ifdef _VARIANT_ARDUINO_DUE_X_
#ifdef __arm__
#include "USARTClass.h"
#endif
#include "HardwareSerial.h"


#define	DEBUG								false


#define MOCOM_BUFFER_LENGTH					48		// the serial buffer length
#define MOCOM_MAX_SLAVES	 				16  	// the arraylength of the slave-ID-array
#define	MOCOM_MAX_MOTORS					4 		// max allowed motor count per slave
#define MOCOM_MASTER_ID						0
#define MOCOM_NO_ID							255

#define MOCOM_START_BYTE 					0xFF
#define MOCOM_BROADCAST 					0xBC
#define MOCOM_TIMEOUT						300UL 



#define	MOCOM_MOTOR_TYPE_LINEAR				0
#define	MOCOM_MOTOR_TYPE_RADIAL				1



#define MOCOM_COMMAND_EMPTY					255


#define MOCOM_COMMAND_ACK					1
#define MOCOM_COMMAND_PING					2
#define MOCOM_COMMAND_WAIT					3
#define MOCOM_COMMAND_DONE					4
#define MOCOM_COMMAND_CODE					5
#define MOCOM_COMMAND_GET_DATA				6
#define MOCOM_COMMAND_DATA					7


#define MOCOM_COMMAND_PREPARE				10
#define MOCOM_COMMAND_START					11
#define MOCOM_COMMAND_STOP					12


#define MOCOM_COMMAND_CAMERA_SHOOT			20


#define	MOCOM_COMMAND_MOTOR_ADD				30
#define	MOCOM_COMMAND_TOO_MANY_MOTORS		31
#define	MOCOM_COMMAND_MOTOR_ID				32
#define	MOCOM_COMMAND_CLEAR_MOTOR_DATA		33
#define	MOCOM_COMMAND_GET_MOTOR_DATA		34


#define	MOCOM_COMMAND_KEY					40
#define	MOCOM_COMMAND_KEY_RELEASE			41


#define MOCOM_COMMAND_TEXT					127




// callback function
extern "C" {
    typedef void (*MoCoMCallbackFunction)(void);
}

// callback function
extern "C" {
    typedef void (*MoCoMCallbackFunctionInt)(int);
}


// our client structure
struct slave
{
   byte	id;				// just the id of the slave
   byte	motorData;		// data structure in the byte:
    					// B  0  0  0  0    0  0  0  0
    					//   M4 M3 M2 M1 | XX XX XX XX
    					//      Types    | Motor-count
};


// ============================================================================
// ============================================================================
// ============================================================================
class MoCoM
{

	public:	
	
		// ===== FUNCTIONS =====
		
		//#ifdef _VARIANT_ARDUINO_DUE_X_
		#ifdef __arm__
		MoCoM(byte, USARTClass&);
		#else
		MoCoM(byte, HardwareSerial&);
		#endif
		
		void 			init(long);
		
		boolean			setID(byte);
		byte 			getID(void);
		boolean 		isMaster(void);  
		boolean 		isIDFree(byte);  
        void			registerSlaves(void);
		    
		boolean			isDataAvailable(void);
		boolean 		executeCommunication(void);
		byte 			getLastReceivedCommand(void);
		byte 			getLastSenderID(void);
				
		byte 			addMotor(byte);   
		void			sendClearMotorData(void);         
  		void			sendDone();
		void			sendDone(byte);
		
				
				
  		byte 			getSlaveCount(void);
		byte 			getSelectedSlaveIndex(void);
  		boolean			selectSlave(byte);
  		boolean 		selectNextSlave(void);
  		boolean			selectPreviousSlave(void);
  		void			deselectSlave(void);
  		boolean			isSlaveSelected(void);
		
  		boolean 		ping(byte, boolean);
  		byte			getSlaveID(void);
  		byte 			getSlaveMotorCount(void); 
  		byte			getSlaveMotorType(byte);
		byte 			getSlavesTotalMotorCount(void);
  		
		unsigned long	getSlaveDataULong(byte);
		
		  		
  		void 			sendCommand(byte, byte);
  		
		void 			sendCommand(byte, byte, byte);
  		void 			sendCommand(byte, byte, int);
		void 			sendCommand(byte, byte, long);
		void 			sendCommand(byte, byte, float);
		void 			sendCommand(byte, byte, char*);
  		
		void			sendCommand(byte, byte, byte, byte);
		void			sendCommand(byte, byte, byte, int);
		void			sendCommand(byte, byte, byte, long);
		void			sendCommand(byte, byte, byte, float);
		
		byte			getDataByte(byte);
		int				getDataInt(byte);
		long			getDataLong(byte);
		float			getDataFloat(byte);
		
		byte			requestDataByte(byte);
		int				requestDataInt(byte);
		long			requestDataLong(byte);
		float			requestDataFloat(byte);
		
		
		
  		void 			registerFunction_TimeCritical(MoCoMCallbackFunction newFunction);
  		void 			registerFunction_CommunicationReceived(MoCoMCallbackFunction newFunction);

	private:
		
		// ===== VARIABLES =====
	
		//#ifdef _VARIANT_ARDUINO_DUE_X_
		#ifdef __arm__
		USARTClass& _serial;
		#else		
		HardwareSerial&	_serial;
		#endif
		
		byte 			_pinDirection;
		byte 			_status;
		byte 			_ID;
		byte 			_receivedCharCnt;
		byte			_sendCharCnt;
		byte 			_dataLength;
		byte 			_receivedCommand;
		byte			_receiverID;
		byte 			_senderID;
		
				
		byte 			_bufferData[MOCOM_BUFFER_LENGTH >> 1];  // buffer for the data (1/2 of the regular buffer)
		byte 			_bufferIn[MOCOM_BUFFER_LENGTH];   		// incomming buffer  
		byte 			_bufferOut[MOCOM_BUFFER_LENGTH];    	// buffer for sending  

		
		byte 			_slaveCount;							// the number of connected slaves
		slave			_slaves[MOCOM_MAX_SLAVES];
		byte 			_selectedSlaveIndex;					// teh salve which is selected right now
		
		unsigned long 	_lastReceiveTime;		
		unsigned long 	_lastSendTime;		
		unsigned long	_receiveStartTime;
		//unsigned long	_waitEndTime;
		
		MoCoMCallbackFunction 		_callback_RegularCalled;
		MoCoMCallbackFunction 		_callback_PacketReceived;

		int				_readDelay;
		
		// ===== FUNCTIONS =====

		
		boolean 		__receive(void);
		void			__deletePacketFromBuffer(byte);
  		void 			__decode(boolean);
				
		boolean 		__isStatusReceiving(void);      
		void  			__setStatusReceiving(void); 
		void  			__deleteStatusReceiving(void);      
		
		
		/*
		boolean 		__isStatusBlocked(void);      
		void  			__setStatusBlocked(void); 
		void  			__deleteStatusBlocked(void);       
		*/
		
		/*
		boolean 		__isStatusWait(void);        
		void 			__setStatusWait(void); 
		void 			__deleteStatusWait(void); 
		*/
		
		void 			__send(void);
		boolean			__ping(byte);
		
		void			__clearSlaveData(void);
		void			__deleteSlave(byte);
		byte			__getSlaveArrayPos(byte);
		byte 			__getSlaveMotorCount(byte);
		
		boolean			__requestData(byte); 		
		long		 	__getNumberFromBuffer(byte);
		float 			__getFloatFromBuffer(byte);
		
		
		uint8_t 		__CRC(byte[], byte);
		void	 		__comDelay(unsigned int);
		void			__clearInput(void);
		
};

#endif