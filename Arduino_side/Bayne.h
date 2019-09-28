#ifndef Bayne_h
#define Bayne_h


#if(ARDUINO >=100) 
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif


class Bayne
{
  private:
  public:
    Bayne();
    void i2cReadBytes(uint8_t i2c_address, uint8_t reg,uint8_t  *data,uint8_t len);
};
#endif
