

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
//       This repeated line of is "0.0,0.0,0.0,0.0,0.0,0.0,0.0" is expected by Python code exactly in this form...
//       Signals the beginning of new data
      Serial.println("0.0,0.0,0.0,0.0,0.0,0.0,0.0");
  }
}


/*************************************************************************************************************/
// Main loop
/*************************************************************************************************************/
void loop() {
  uint8_t ADXL345 = 0x53, ADXL345_data_register = 0x32;
  uint8_t MPU3050 = 0x68, MPU3050_data_register = 0x1D;
  uint8_t raw_data[6];
  float accel[3];

  float scale[3] = {0.935870 , 0.930660, 1.015939};
  float offset[3] = {9.5,-18,-36};
  float gyro[3];
  float gyro_offset[3]={450,119.5,70.5};// working well with numbers around {449.5,119.5,70.5}
  unsigned long deltaTime;
  


  //*******************************************************************************
  // Coding for Accelerometer    accel
  //*******************************************************************************
  if(b.collectData()){
    b.i2cReadBytes(ADXL345, ADXL345_data_register , raw_data,6);
    for(int i=0;i<3;i++){
      int j,k;
      j = i*2;
      k=j+1;
      accel[i] = float(raw_data[j]|raw_data[k]<<8);
      accel[i] = (accel[i] + offset[i])*scale[i];
      accel[i] = accel[i]*9.8/255;
      //Serial.print(accel[i]); Serial.print(",");
    }
  
    //*******************************************************************************
    // Coding for Gyro: gyro  and loading serial data
    //*******************************************************************************
    // Get gyro rawdata and offset with rough calibration numbers
    
    b.i2cReadBytes(MPU3050, MPU3050_data_register , raw_data,6);
    for(int i=0;i<3;i++){
      int j,k;
      j = i*2;
      k=j+1;
      gyro[i] = float(raw_data[k]|raw_data[j]<<8);
      gyro[i] = (gyro[i] + gyro_offset[i])/131/4;   // see notes as to 131 scaling ... linked to full scale setting
      //Serial.print(gyro[i]); Serial.print(",");
    }
    // find the time and calculat the change in time in milliseconds
    unsigned long pTime =  millis();
    deltaTime =b.changeInTime(pTime);
    // if delta time is not zero ... meaning no new data... print new data stream
    if(deltaTime != 0){
      // print the running average for accleration accel_smoothed[3]
      b.accelAvg(accel);
      b.avgAccelPrint();
      // calculate the sum of the gyro movement anf print this
      b.sumGyro(gyro); 
      b.sumGyroPrint();
      Serial.print(float(pTime)/1000,3); 
      Serial.println();
    }
    
    //*******************************************************************************
    // TODO: self calibration when a = 9.8 for gyros
    //*******************************************************************************
  }else{
    Serial.println("Waiting");
  }
  if(digitalRead(12)==HIGH){
    b.runStop();
  }
}

/*************************************************************************************************************/












  
