#include "Bayne.h"
#include <Wire.h>


// constructor for class
Bayne::Bayne()
{}

// method to read bytes of data into a matrix for manipulation
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
