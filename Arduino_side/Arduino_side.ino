

/* Accelerometers to send data to Processing
 *  
 *  I2C address ... ADXL345 = 0x53
 * 
 */
 

#include "Bayne.h"
#include <Wire.h>

Bayne b = Bayne();



uint8_t ADXL345 = 0x53; // The ADXL345 sensor I2C address
/*************************************************************************************************************/
void setup() {
  
  uint8_t error = 0;
  b.blink_error(error);
  Serial.begin(115200); // start serial communication
  Wire.begin();       // start I2C communication
  b.startADXL345();
  b.startMPU3050();
  
  for(int i=0;i<10;i++){
      // This repeated line of is "0.0,0.0,0.0,0.0" is expected by Python code 
      // Signals the beginning of new data
      //Serial.println("0.0,0.0,0.0,0.0");
  }
}
/*************************************************************************************************************/
void loop() {
  uint8_t ADXL345 = 0x53, ADXL345_data_register = 0x32;
  uint8_t MPU3050 = 0x68, MPU3050_data_register = 0x1D;
  Bayne b = Bayne();
  uint8_t raw_data[6];
  float accel[3];
  float scale[3] = {0.935870 , 0.930660, 1.015939};
  float offset[3] = {9.5,-18,-36};
  float gyro[3];
  float gyro_offset[3]={17641.1,-17465.6,-18680.1};
  b.i2cReadBytes(ADXL345, ADXL345_data_register , raw_data,6);
  for(int i=0;i<3;i++){
    int j,k;
    j = i*2;
    k=j+1;
    accel[i] = float(raw_data[k]|raw_data[j]<<8);
    accel[i] = (accel[i] + offset[i])*scale[i];
    accel[i] = accel[i]*9.8/255;
    //Serial.print(accel[i]); Serial.print(',');
  }
  unsigned long pTime = millis();
  float t= float(pTime)/1000;
  //Serial.println(t);      // NOTE: println very important for Python program to have serial data with end line

//*******************************************************************************
// TODO: Adding code here for Gyro
//*******************************************************************************

  b.i2cReadBytes(MPU3050, MPU3050_data_register , raw_data,6);
  //Serial.print("RAW DATA MPU3050>>>>  ");

  
  for(int i=0;i<3;i++){
    int j,k;
    j = i*2;
    k=j+1;
    gyro[i] = float(raw_data[k]|raw_data[j]<<8);
    gyro[i] = (gyro[i] - gyro_offset[i])/131;
    //Serial.print(gyro[i]); Serial.print(',');
  }
  //Serial.println("  <<<<RAW DATA MPU3050");
  Serial.print(gyro[0]);
  Serial.print(",");
  Serial.print(gyro[1]);
  Serial.print(",");
  Serial.println(gyro[2]);

  delay(10);

  
//*******************************************************************************
// end Gyro coding construction 
//*******************************************************************************
}

/*************************************************************************************************************/












  
