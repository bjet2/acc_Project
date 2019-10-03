#ifndef Bayne_h
#define Bayne_h


#if(ARDUINO >=100) 
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#define MPU3050_ADDRESS 0x68
#define MPU3050_REG_WHOAMI 0

class Bayne
{
  private:
    unsigned long _oldTime, _change;
    float _accel_old[15][3];
    float _accel_smoothed[3];
    float _gyroSums[3];
    float _sumThreshold;
    bool _collectData;
  public:
    Bayne();
    void i2cReadBytes(uint8_t i2c_address, uint8_t reg,uint8_t  *data,uint8_t len);
    void i2cReadOneByte(uint8_t i2c_address, uint8_t reg,uint8_t *data );
    void blink_error(int error_num);
    void startADXL345();
    void startMPU3050();
    unsigned long changeInTime(unsigned long newTime);
    void sumGyro(float *gyro);
    void accelAvg(float *accel);
    void sumGyroPrint();
    void avgAccelPrint();
    bool collectData();
    void runStop();
};
#endif
