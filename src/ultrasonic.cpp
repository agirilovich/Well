#include "ultrasonic.h"

//define UART3 port
HardwareSerial USSerial(USART3);


void initializeUltrasonic()
{
    USSerial.begin(9600);
}

unsigned int UltrasonicGetDistance()
{
  unsigned int distance;
  uint8_t h_data, l_data, sum = 0;
  uint8_t startByte = 0;
  uint8_t buf[3];

  USSerial.write(0x01);
  delay(50);
  
  if(USSerial.available()){
    startByte = (uint8_t)USSerial.read();
  }

  
  if(startByte == 255){
    USSerial.readBytes(buf, 3);
    h_data = buf[0];
    l_data = buf[1];
    sum = buf[2];
    distance = (h_data<<8) + l_data;
    if((( h_data + l_data)&0xFF) != sum){
      return 0;
    }
    else{
      return distance;
    } 
  } 
  else return 0;
}