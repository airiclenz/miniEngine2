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


#define PREC_TOUCH_CONST   10

#define PixSizeX	   13.78
#define PixOffsX	   411

#define PixSizeY	   11.01
#define PixOffsY	   378



int TP_X, TP_Y;


// ============================================================================
void touch_init() {
  
  
  pinMode(PIN_TOUCH_CLK,  OUTPUT);
  pinMode(PIN_TOUCH_CS,   OUTPUT);
  pinMode(PIN_TOUCH_DIN,  OUTPUT);
  pinMode(PIN_TOUCH_DOUT, INPUT);
  pinMode(PIN_TOUCH_IRQ,  INPUT);

  digitalWrite(PIN_TOUCH_CS,  HIGH);
  digitalWrite(PIN_TOUCH_CLK, HIGH);
  digitalWrite(PIN_TOUCH_DIN, HIGH);
  digitalWrite(PIN_TOUCH_CLK, HIGH);

}



// ============================================================================
void touch_process() {
  
  unsigned int  i,j;
  while(touch_dataAvailable() == 1)
  {
    touch_read();
    i = touch_getX();
    j = touch_getY();
    
  } 
  
}



// ============================================================================
void touch_writeData(unsigned char data)
{
  unsigned char temp;
  unsigned char nop;
  unsigned char count;

  temp=data;
  digitalWrite(PIN_TOUCH_CLK,LOW);

  for(count=0; count<8; count++) {

    if (temp & 0x80) {
      digitalWrite(PIN_TOUCH_DIN, HIGH);
    } else {
      digitalWrite(PIN_TOUCH_DIN, LOW);
    }
    
    temp = temp << 1; 
    digitalWrite(PIN_TOUCH_CLK, LOW);                
    nop++;
    digitalWrite(PIN_TOUCH_CLK, HIGH);
    nop++;
  }
}


// ============================================================================
unsigned int touch_readData()
{
  unsigned char nop;
  unsigned int data = 0;
  unsigned char count;
  for(count=0; count<12; count++) {
    
    data <<= 1;
    
    digitalWrite(PIN_TOUCH_CLK, HIGH);               
    nop++;
    digitalWrite(PIN_TOUCH_CLK, LOW);
    nop++;
    
    if (digitalRead(PIN_TOUCH_DOUT)) data++;
  }
  
  return(data);
}

// ============================================================================
void touch_read()
{
  unsigned long tx=0;
  unsigned long ty=0;

  digitalWrite(PIN_TOUCH_CS,LOW);                    

  for (int i=0; i<PREC_TOUCH_CONST; i++) {
    
    touch_writeData(0x90);        
    digitalWrite(PIN_TOUCH_CLK,HIGH);
    digitalWrite(PIN_TOUCH_CLK,LOW); 
    ty += touch_readData();

    touch_writeData(0xD0);      
    digitalWrite(PIN_TOUCH_CLK,HIGH);
    digitalWrite(PIN_TOUCH_CLK,LOW);
    tx += touch_readData();

  }

  digitalWrite(PIN_TOUCH_CS,HIGH);

  TP_X = tx / PREC_TOUCH_CONST;
  TP_Y = ty / PREC_TOUCH_CONST;

}


// ============================================================================
char touch_dataAvailable()
{
  char avail;
  avail = !digitalRead(PIN_TOUCH_IRQ);
  return avail;
}


// ============================================================================
int touch_getX()
{
  int value;
  
  value = ((TP_X-PixOffsX)/PixSizeX);
  if (value < 0) value = 0;
  return value;
}

// ============================================================================
int touch_getY()
{
  int value;
  
  value = ((TP_Y-PixOffsY)/PixSizeY);
  if (value < 0) value = 0;
  return value;
}
