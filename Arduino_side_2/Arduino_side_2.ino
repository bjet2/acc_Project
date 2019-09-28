

/* Accelerometers to send data to Processing
 *  
 *  I2C address ... ADXL345 = 0x53
 * 
 */
 

#include "Bayne.h"
#include <Wire.h>

void blink_error(int error_num=10);
void startADXL345();
uint8_t ADXL345 = 0x53; // The ADXL345 sensor I2C address
/*************************************************************************************************************/
void setup() {
  uint8_t error = 0;
  blink_error(error);
  Serial.begin(115200); // start serial communication
  Wire.begin();       // start I2C communication
  startADXL345();
  for(int i=0;i<10;i++){
      // This repeated line of is "0.0,0.0,0.0,0.0" is expected by Python code 
      // Signals the beginning of new data
      Serial.println("0.0,0.0,0.0,0.0");
  }
}
/*************************************************************************************************************/
void loop() {
  uint8_t ADXL345 = 0x53, ADXL345_data_register = 0x32;
  Bayne b = Bayne();
  uint8_t raw_data[6];
  float accel[3];
  float scale[3] = {0.935870 , 0.930660, 1.015939};
  float offset[3] = {9.5,-18,-36};
  b.i2cReadBytes(ADXL345, ADXL345_data_register , raw_data,6);
  for(int i=0;i<3;i++){
    int j,k;
    j = i*2;
    k=j+1;
    accel[i] = float(raw_data[j]|raw_data[k]<<8);
    accel[i] = (accel[i] + offset[i])*scale[i];
    accel[i] = accel[i]*9.8/255;
    Serial.print(accel[i]); Serial.print(',');
  }
  unsigned long pTime = millis();
  float t= float(pTime)/1000;
  Serial.println(t);      // NOTE: println very important for Python program to have serial data with end line
  delay(100);
}

/*************************************************************************************************************/
void startADXL345(){
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
void blink_error(int error_num=10){
  
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










  
