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


////////////////////////////////////////////////////////
//                                                    //
//  C O M   V A R I A B L E S                         //
//                                                    //
////////////////////////////////////////////////////////







// ============================================================================
void com_init() {
  
  com.init(57600);  
  com.registerFunction_CommunicationReceived(com_handleEvents);  
  com.setID(11);
  
}


// ============================================================================
void com_process() {
  com.executeCommunication();
}


// ============================================================================
void com_handleEvents() {
  
  byte command = com.getLastReceivedCommand();
  
  if (command == MOCOM_COMMAND_GET_MOTOR_DATA) {
    com_sendMotorInfo();
  }
  
}


// =====================================================================================
void com_sendMotorInfo() {
  
  com.sendClearMotorData();
  
  for (int i=0; i<DEF_MOTOR_COUNT; i++) {
    com.addMotor(motors[i].getType());  
  }

  com.sendDone();
}
