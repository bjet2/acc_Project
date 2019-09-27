#include "Bayne.h"
#include <Wire.h>


// constructor for class
Bayne::Bayne()
{}


void Bayne::blink_error(int error_num=10){
  
  // Error code for builtin LED on pin 13. Can be used to give information about 
  // program errors without plugging in to computer. Useful for troubleshooting 
  // robot during competition.
  // Default error 10 = catch all error 
  // numbers up to 10 can be defined by programmer as needed. Anything above 10
  // or below 0 will be changed to default error of 10
  // Will display number on serial port above 10 (and below 1) for greater detail
  
  _error_num = error_num;
  if(_error_num < 1 | _error_num > 10) _error_num = 10;
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  while(1==1){
    for(int i=0;i<_error_num;i++){
      digitalWrite(13,HIGH);
      delay(200);
      digitalWrite(13,LOW);
      delay(200);
    }
    delay(2000);
    if (Serial){
       // if serial port is connected send error number. This number can be 
       Serial.print("Error = ");Serial.println(error_num);

    }
    
  }
}

void Bayne::i2cReadBytes(uint8_t i2c_address, uint8_t reg,uint8_t  *data,uint8_t len)
{
  Wire.beginTransmission(i2c_address);
  Wire.write(reg);
  uint8_t error = Wire.endTransmission(i2c_address);
  if(error != 0){
    Serial.print("Wire error = ");Serial.println(error);
  }
  Wire.requestFrom(i2c_address,len,true);
  for ( int i =0; i<len; i++){
    data[i] = Wire.read();
  }
}
