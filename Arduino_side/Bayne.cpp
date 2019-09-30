#include "Bayne.h"
#include <Wire.h>

/*************************************************************************************************************/
// constructor for class
Bayne::Bayne(){
  _oldTime = 0; 
  _change = 0; 
  for(int i=0;i<3;i++){
    _gyroSums[i] = 0.0;
  }
  
}
/*************************************************************************************************************/
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
/*************************************************************************************************************/
void Bayne::blink_error(int error_num=10){
  
  // Error code for builtin LED on pin 13. Can be used to give information about 
  // program errors without plugging in to computer. Useful for troubleshooting 
  // robot during competition.
  // Default error 10 = catch all error 
  // numbers up to 10 can be defined by programmer as needed. Anything above 10
  // or below 0 will be changed to default error of 10
  // Will display number on serial port above 10 (and below 1) for greater detail
  
  if (error_num ==0){
    digitalWrite(13,LOW);
  }else{
    if(error_num < 0 | error_num > 10) error_num = 10;
    pinMode(13,OUTPUT);
    digitalWrite(13,LOW);
    while(1==1){
      for(int i=0;i<error_num;i++){
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
}
/*************************************************************************************************************/
void Bayne::startADXL345(){
  uint8_t ADXL345 = 0x53;
  uint8_t error;
  Wire.beginTransmission(ADXL345);  // Start communicating with the device 
  Wire.write(0x2D);                 // Access/ talk to POWER_CTL Register - 0x2D
                                  // Enable measurement
  Wire.write(8);                    // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  Wire.endTransmission();
  if(error !=0){
    Serial.print("1 Error starting ADXL345 = "); Serial.println(error);
  }
  Wire.beginTransmission(ADXL345);  // Start communicating with the device 
  Wire.write(0x32);                 // register to read
  Wire.endTransmission();
  if(error !=0){
    Serial.print("2 Error starting ADXL345 = "); Serial.println(error);
  }
}
/*************************************************************************************************************/

// method to one byte of data
void Bayne::i2cReadOneByte(uint8_t i2c_address, uint8_t reg, uint8_t *data ){
  
  Serial.print("Data in = "); Serial.println(*data);
  Wire.beginTransmission(i2c_address);
  Wire.write(reg);
  uint8_t error = Wire.endTransmission(i2c_address);
  if(error != 0){
    Serial.print("Wire error = ");Serial.println(error);
  }
  Wire.requestFrom(i2c_address,1,true);
  *data = Wire.read(); 
}
/*************************************************************************************************************/
void Bayne::startMPU3050(){
  uint8_t i2cAddress = MPU3050_ADDRESS;
  uint8_t DLPFRegAddress = 0x16;
  uint8_t MPU3050_lowpass_188HZ= 1, MPU3050_range_PM500 = 1;
  uint8_t data=0x00;
  uint8_t config_DLPF_byte,check_DLPF_byte ;
  
  Serial.print("In Gyrostartup");
  i2cReadOneByte(i2cAddress, MPU3050_REG_WHOAMI, &data);
  Serial.print("Id number = "); Serial.println(data);
  if((data&0x7E) == MPU3050_ADDRESS){                       // mask to find ID from MPU3050_REG_WHOAMI
    Serial.println("Correct Gyrostartup IDed");
    config_DLPF_byte = (MPU3050_lowpass_188HZ | (MPU3050_range_PM500<<3)) & 0x1F;
      Wire.beginTransmission(i2cAddress);
      Wire.write(DLPFRegAddress);     // First byte is to set the register pointer
      Wire.write(config_DLPF_byte);   // Write the data byte
    int error_code= Wire.endTransmission();
    i2cReadOneByte(i2cAddress, DLPFRegAddress, &check_DLPF_byte );
    while(error_code !=0){
      Serial.print("Setupt Error from gyro = "); Serial.println(error_code);
    }
  }
}
/*************************************************************************************************************/
unsigned long Bayne::changeInTime(unsigned long newTime){
  unsigned long change;
  _change = newTime-_oldTime;
  _oldTime = newTime;
  return _change;
}
/*************************************************************************************************************/
void Bayne::sumGyro(float *gyro){
  for(int i=0;i<3;i++){
    _gyroSums[i]=_gyroSums[i]+gyro[i]*float(_change)/1000;
    Serial.print(_gyroSums[i]);Serial.print(",");
  }
  Serial.println();
}

/*************************************************************************************************************/
