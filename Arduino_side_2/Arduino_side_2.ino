#include "Bayne.h"
#include <Wire.h>

void conv2Byte2Signed16(uint8_t LSB,uint8_t MSB, int16_t * dest);
void i2cReadBytes(uint8_t i2c_address, uint8_t reg,uint8_t *data,uint8_t len);

/* Accelerometers to send data to Processing
 *  
 *  I2C address ... ADXL345 = 0x53
 * 
 */
 

const int ADXL345 = 0x53; // The ADXL345 sensor I2C address
bool calibrate = false;

void setup() {
  // two pins for troubleshooting 13 (onboard LED) will go high at end of set up
  // may use blink error codes in future



  
  Serial.begin(115200); // start serial communication

  Wire.begin();       // start I2C communication


  // Send settings to DDXL345 
  Wire.beginTransmission(ADXL345);  // Start communicating with the device 
  Wire.write(0x2D);                 // Access/ talk to POWER_CTL Register - 0x2D
                                    // Enable measurement
  Wire.write(8);                    // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  Wire.endTransmission();
  delay(100);                       // delay to insure registers are set ... 

  for(int i=0;i<10;i++){
      // Set the register to be read in ADXL345 to 0x32
      // delay is to insure ADXL345 register is set before 
      // Initial data point is sent
      Wire.beginTransmission(ADXL345); 
      Wire.write(0x32); // Set the Register to be read 
      Wire.endTransmission(false);
      delay(100);   
    
      // This line is expected by Python code 
      // Signals the beginning of new data
      // Delay is added to be cautious, but likely can be removed
      Serial.println("0.0,0.0,0.0,0.0");
  }
  digitalWrite(13,HIGH);
  

}

void loop() {
  uint8_t raw_data[6];
  Bayne b = Bayne();
  
  float accelx, accely, accelz;
  uint8_t error_code,MSB,LSB;
  
  //b.blink_error(33);
  b.i2cReadBytes(ADXL345, 0x32 , raw_data,6);

  accelx = float(raw_data[0]|raw_data[1] << 8); 
  accely = float(raw_data[2]|raw_data[3] << 8); 
  accelz = float(raw_data[4]|raw_data[5] << 8); 

  // send serial data in form "ax,ay,az,t"
  Serial.print((accelx+9.5)*.935870);          // see calibration.py for formulas and numerical data
  Serial.print(',');
  Serial.print((accely-18)*.930660);         // see above example
  Serial.print(',');
  Serial.print((accelz-36)*1.015939);       // see above example
  unsigned long pTime = millis();
  float t= float(pTime)/1000;
  Serial.print(',');
  Serial.println(t);      // NOTE: println very important for Python program to have serial data with end line
  //delay(10);
  
}

void conv2Byte2Signed16(uint8_t LSB,uint8_t MSB, int16_t * dest)
{
  *dest = 0;
  *dest = (int16_t)LSB;
  *dest |= ((int16_t)MSB << 8);
}







  
