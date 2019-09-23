#include <Wire.h>
/* Accelerometers to send data to Processing
 *  
 *  I2C address ... ADXL345 = 0x53
 * 
 */
 

const int ADXL345 = 0x53; // The ADXL345 sensor I2C address


void setup() {
  // two pins for troubleshooting 13 (onboard LED) will go high at end of set up
  // pin 12 is not used at present
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  
  Serial.begin(9600); // start serial communication

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
  
  float accelx, accely, accelz;

  // Set the register to be read in ADXL345 to 0x32
  Wire.beginTransmission(ADXL345); 
  Wire.write(0x32); // Set the Register to be read 
  Wire.endTransmission(false);
 
  // Read 6 registers total, each axis value is stored in 2 registers
  Wire.requestFrom(ADXL345, 6, true); 
  accelx = float(Wire.read()|Wire.read() << 8); 
  accely = float(Wire.read()|Wire.read() << 8); 
  accelz = float(Wire.read()|Wire.read() << 8); 

  // send serial data in form "ax,ay,az,t"
  Serial.print((accelx+10)*.927273);          // calibration scale = 0.97273 and offset +10
  Serial.print(',');
  Serial.print((accely-20)*.9239113);         // see above example
  Serial.print(',');
  Serial.print((accelz-29.5)*1.017964);       // see above example
  unsigned long pTime = millis();
  float t= float(pTime)/1000;
  Serial.print(',');
  Serial.println(t);      // NOTE: println very important for Python program to have serial data with end line
  //delay(10);
}

  
