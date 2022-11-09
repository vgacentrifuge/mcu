#include "pin_config.h"
#include "sl_i2cspm.h"
#include "em_cmu.h"
#include "em_i2c.h"

#define I2C_EEPROM1_PERIPHERAL I2C0
#define I2C_EEPROM2_PERIPHERAL I2C0

#define I2C_EEPROM1_ROUTE_LOC 2
#define I2C_EEPROM2_ROUTE_LOC 1

#define I2C_EEPROM1_SCL_PORT ADC1_DDC_SCL_PORT
#define I2C_EEPROM1_SCL_PIN ADC1_DDC_SCL_PIN
#define I2C_EEPROM1_SDA_PORT ADC1_DDC_SDA_PORT
#define I2C_EEPROM1_SDA_PIN ADC1_DDC_SDA_PIN

#define I2C_EEPROM2_SCL_PORT ADC2_DDC_SCL_PORT
#define I2C_EEPROM2_SCL_PIN ADC2_DDC_SCL_PIN
#define I2C_EEPROM2_SDA_PORT ADC2_DDC_SDA_PORT
#define I2C_EEPROM2_SDA_PIN ADC2_DDC_SDA_PIN

// I2CSPM SPEED MODE
// 0 Standard mode (100kbit/s)
// 1 Fast mode (400kbit/s)
// 2 Fast mode plus (1Mbit/s)
// Default: 0
#define I2C_EEPROMS_SPEED_MODE      0

// I2CSPM REFERENCE CLOCK - Reference clock frequency
// Frequency in Hz of the reference clock.
// Select 0 to use the frequency of the currently selected clock.
// Default: 0
#define I2C_EEPROMS_REFERENCE_CLOCK 0

/*
 * The rest of this file is pretty much copied from sl_i2cspm_init.c
 */
#if I2C_EEPROMS_SPEED_MODE == 0
#define I2CSPM_HLR i2cClockHLRStandard
#define I2CSPM_MAX_FREQ I2C_FREQ_STANDARD_MAX
#elif I2C_EEPROMS_SPEED_MODE == 1
#define I2CSPM_HLR i2cClockHLRAsymetric
#define I2CSPM_MAX_FREQ I2C_FREQ_FAST_MAX
#elif I2C_EEPROMS_SPEED_MODE == 2
#define I2CSPM_HLR i2cClockHLRFast
#define I2CSPM_MAX_FREQ I2C_FREQ_FASTPLUS_MAX
#endif

I2CSPM_Init_TypeDef init_eeprom1 = {
  .port = I2C_EEPROM1_PERIPHERAL,
  .sclPort = I2C_EEPROM1_SCL_PORT,
  .sclPin = I2C_EEPROM1_SCL_PIN,
  .sdaPort = I2C_EEPROM1_SDA_PORT,
  .sdaPin = I2C_EEPROM1_SDA_PIN,
  .portLocation = I2C_EEPROM1_ROUTE_LOC,

  .i2cRefFreq = 0,
  .i2cMaxFreq = I2CSPM_MAX_FREQ,
  .i2cClhr = I2CSPM_HLR
};

I2CSPM_Init_TypeDef init_eeprom2 = {
  .port = I2C_EEPROM1_PERIPHERAL,
  .sclPort = I2C_EEPROM1_SCL_PORT,
  .sclPin = I2C_EEPROM1_SCL_PIN,
  .sdaPort = I2C_EEPROM1_SDA_PORT,
  .sdaPin = I2C_EEPROM1_SDA_PIN,
  .portLocation = I2C_EEPROM1_ROUTE_LOC,

  .i2cRefFreq = 0,
  .i2cMaxFreq = I2CSPM_MAX_FREQ,
  .i2cClhr = I2CSPM_HLR
};

