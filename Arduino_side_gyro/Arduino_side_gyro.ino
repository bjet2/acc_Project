#include <Wire.h>
/* Accelerometers to send data to Processing
 *  
 *  I2C address ... ADXL345 = 0x53
 * 
 */
 

const int ADXL345 = 0x53;         // The ADXL345 sensor I2C address
const int MPU_addr = 0x68;       // address for gyro
bool calibrate = false;
int error_code;

void setup() {
  // two pins for troubleshooting 13 (onboard LED) will go high at end of set up
  // pin 12 is not used at present

  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  
  Serial.begin(115200); // start serial communication

  Wire.begin();       // start I2C communication


  // Send settings to DDXL345 
  Wire.beginTransmission(ADXL345);  // Start communicating with the device 
  Wire.write(0x2D);                 // Access/ talk to POWER_CTL Register - 0x2D
                                    // Enable measurement
  Wire.write(8);                    // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  Wire.endTransmission();
  delay(100);                       // delay to insure registers are set ... 
  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  error_code= Wire.endTransmission();
  while(error_code !=0){
    Serial.print("Setupt Error from gyro = "); Serial.println(error_code);
  }
  
  for(int i=0;i<10;i++){
        // Set the register to be read in ADXL345 to 0x32
        // delay is to insure ADXL345 register is set before 
        // Initial data point is sent
        Wire.beginTransmission(ADXL345); 
        Wire.write(0x32); // Set the Register to be read 
        Wire.endTransmission();
        delay(100);   
        Serial.println("0.0,0.0,0.0,0.0");
        // This line is expected by Python code 
        // Signals the beginning of new data
        // Delay is added to be cautious, but likely can be removed
   }


    
  digitalWrite(13,HIGH);
  

}

void loop() {
  
  float accelx, accely, accelz;
  int16_t gx,gy,gz,Tmp;

  // Set the register to be read in ADXL345 to 0x32
  Wire.beginTransmission(ADXL345); 
  Wire.write(0x32); // Set the Register to be read 
  Wire.endTransmission();
 
  // Read 6 registers total, each axis value is stored in 2 registers
  Wire.requestFrom(ADXL345, 6, true); 
  accelx = float(Wire.read()|Wire.read() << 8); 
  accely = float(Wire.read()|Wire.read() << 8); 
  accelz = float(Wire.read()|Wire.read() << 8); 

  if(calibrate){
      // send serial data in form "ax,ay,az"
    Serial.print("CALIBRATION DATA:");   
    Serial.print((accelx));          
    Serial.print(',');
    Serial.print((accely));         
    Serial.print(',');
    Serial.println((accelz));       
  }else{
    // send serial data in form "ax,ay,az,t"
    Serial.print((accelx+9.5)*.935870);          // see calibration.py for formulas and numerical data
    Serial.print(',');
    Serial.print((accely-18)*.930660);         // see above example
    Serial.print(',');
    Serial.print((accelz-36)*1.015939);       // see above example
    unsigned long pTime = millis();
    float t= float(pTime)/1000;
    Serial.print(',');
    Serial.print(t);      // NOTE: println very important for Python program to have serial data with end line

    Wire.beginTransmission(MPU_addr);
    Wire.write(0x1D);  // starting with register 0x3B (ACCEL_XOUT_H)
    delay(100);
    error_code = Wire.endTransmission();
  
    while(error_code !=0){
      Serial.print("loop Error from gyro = "); Serial.println(error_code);
    }
    
    Wire.requestFrom(MPU_addr,6,true);  // request a total of 6 registers
    gx=Wire.read()<<8|Wire.read();  // 0x1D (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    gy=Wire.read()<<8|Wire.read();  // 
    gz=Wire.read()<<8|Wire.read();  // 
    Serial.print("     gyro raw data:");   
    Serial.print(gx);          
    Serial.print(',');
    Serial.print(gy);         
    Serial.print(',');
    Serial.println(gz);       
  }
}

  
