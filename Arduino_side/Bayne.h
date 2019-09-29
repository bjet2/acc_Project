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
  public:
    Bayne();
    void i2cReadBytes(uint8_t i2c_address, uint8_t reg,uint8_t  *data,uint8_t len);
    void i2cReadOneByte(uint8_t i2c_address, uint8_t reg,uint8_t *data );
    void blink_error(int error_num);
    void startADXL345();
    void startMPU3050();
};
#endif
