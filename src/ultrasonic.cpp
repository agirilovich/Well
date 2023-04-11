#include "ultrasonic.h"

//define UART3 port
HardwareSerial Serial3(PB11, PB10);


void initializeUltrasonic()
{
    Serial3.begin(9600);
}

unsigned int UltrasonicGetDistance()
{
  unsigned int distance;
  uint8_t startByte, h_data, l_data, sum = 0;
  uint8_t buf[3];
  
  startByte = (uint8_t)Serial3.read();
  if(startByte == 255){
    Serial3.readBytes(buf, 3);
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