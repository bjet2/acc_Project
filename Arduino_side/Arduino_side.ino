#include <Wire.h>
/* Accelerometers to send data to Processing
 *  
 *  I2C address ... ADXL345 = 0x53
 * 
 */
 

const int ADXL345 = 0x53; // The ADXL345 sensor I2C address


void setup() {
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
//  digitalWrite(12,HIGH);
//  delay(5000);
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  
  Serial.begin(9600); // start serial communication
  Serial.flush();
  Wire.begin();       // start I2C communication
  Wire.flush();

  // Send settings to accel_0_port
  Wire.beginTransmission(ADXL345); // Start communicating with the device 
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
                    // Enable measurement
  Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  Wire.endTransmission();
  delay(100);
    // Set the register to be read in ADXL345
  Wire.beginTransmission(ADXL345); 
  Wire.write(0x32); // Set the Register to be read 
  Wire.endTransmission(false);
  delay(100); 
  digitalWrite(12,HIGH);
  

}

void loop() {
  
  float accelx, accely, accelz;

  Wire.beginTransmission(ADXL345); 
  Wire.write(0x32); // Set the Register to be read 
  Wire.endTransmission(false);
  digitalWrite(12,HIGH); 
 
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  accelx = float(Wire.read()|Wire.read() << 8); 
  accely = float(Wire.read()|Wire.read() << 8); 
  accelz = float(Wire.read()|Wire.read() << 8); 
  //Serial.print("d,"); 
  Serial.print(accelx);
  Serial.print(',');
  Serial.print(accely);
  Serial.print(',');
  Serial.print(accelz);
  unsigned long pTime = millis();
  float t= float(pTime)/1000;
  Serial.print(',');
  Serial.println(t);      // NOTE: ln very important for Processing
  delay(100);
}

  
