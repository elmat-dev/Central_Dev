/*
  Copyright 2014 Marcus Sorensen <marcus@electron14.com>

This program is licensed under the GNU GPL v2
*/
#ifndef SI7021_h
#define SI7021_h

#include <stdbool.h>
#include "tm_stm32f4_i2c.h"
#include "tm_stm32f4_delay.h"

#define SI7021_I2C_PORT             I2C3
#define SI7021_I2C_PINS             TM_I2C_PinsPack_1
#define SI7021_I2C_SPEED            200000
#define SI7021_ADDRESS              (0x40 << 1)


typedef struct si7021_env {
    int celsiusHundredths;
    float fahrenheitHundredths;
    unsigned int humidityBasisPoints;
    int connected;
} si7021_env;


    
    bool SI7021_begin();
    bool SI7021_sensorExists();
    float SI7021_getFahrenheitHundredths();
    int SI7021_getCelsiusHundredths();
    unsigned int SI7021_getHumidityPercent();
    unsigned int SI7021_getHumidityBasisPoints();
    int SI7021_getCelsiusPostHumidity();
    int SI7021_getHumidityAndTemperature(si7021_env*);
    int SI7021_getSerialBytes(uint8_t * buf);
    int SI7021_getDeviceId();
	
    void SI7021_command(uint8_t * cmd, uint8_t * buf );
    void SI7021_writeReg(uint8_t * reg, int reglen);
    int SI7021_readReg(uint8_t * reg, int reglen);



#endif

