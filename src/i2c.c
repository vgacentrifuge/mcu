/*
 * i2c.c
 *
 *  Created on: Oct 9, 2022
 *      Author: marhaug
 */

#include "i2c.h"
#include "efm32gg990f1024.h"

#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_i2c.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"

/* Transfer flag */
static volatile bool transferActive = false;

/* I2C error flag */
static volatile bool i2cError = false;

/* Transmit data */
static const uint8_t txData[] = { 0x1, 0x2, 0x3, 0x4, 0x5 };

I2C_TypeDef * i2c= (I2C_TypeDef *)I2C0;

//void initI2C(void) {
//  CMU_ClockEnable(cmuClock_HFPER, true);
//  CMU_ClockEnable(cmuClock_I2C1, true);
//
//  //Set up SDA (PD6 || PC5 )and SCL(PD7 || PC4) pins
//  GPIO_PinModeSet(SDA_port, SDA_pin, gpioModeWiredAnd, SDA_default);
//  GPIO_PinModeSet(SCL_port, SCL_pin, gpioModeWiredAnd, SCL_default);
//
//  //Toggle SCL 9 times for any I2C slave which requires this to be done
//  for (int i=0; i<9; i++) {
//    GPIO_PinOutClear(SCL_port,SCL_pin);
//    GPIO_PinOutSet(SCL_port,SCL_pin);
//  }
//
//  //Route the I2C pins to the I2C peripheral
//  // I2C0->ROUTELOC0 |= I2C_ROUTELOC0_SCLLOC_LOC14 | I2C_ROUTELOC0_SDALOC_LOC16; ??
//  // I2C0->ROUTEPEN |= I2C_ROUTEPEN_SCLPEN | I2C_ROUTEPEN_SDAPENdd; ??
//
//  initI2CStruct();
//}

void initI2C(void) {
  CMU_Clock_TypeDef i2cClock;
  I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;

  CMU_ClockEnable(cmuClock_HFPER, true);

  i2cClock = cmuClock_I2C0;
  CMU_ClockEnable(i2cClock, true);

  /* Output value must be set to 1 to not drive lines low. Set
  SCL first, to ensure it is high before changing SDA. */
  GPIO_PinModeSet(gpioPortD, 7, gpioModeWiredAndPullUp, 1);
  GPIO_PinModeSet(gpioPortD, 6, gpioModeWiredAndPullUp, 1);

  /* In some situations, after a reset during an I2C transfer, the slave
    device may be left in an unknown state. Send 9 clock pulses to
    set slave in a defined state. */
  for (int i = 0; i < 9; i++) {
      GPIO_PinOutSet(gpioPortD, 7);
      GPIO_PinOutClear(gpioPortD, 7);
  }

  /* Enable pins and set location */
  I2C0->ROUTE = I2C_ROUTE_SDAPEN | I2C_ROUTE_SCLPEN | (1 << _I2C_ROUTE_LOCATION_SHIFT);

  I2C_Init(I2C0, &i2cInit);

}

#define _REGISTER_ADDRESS 0x00
#define _REGISTER_VALUE   0x01

void sendSignal(void) {
  // Prepare for a read of 2 bytes
  I2C_TransferSeq_TypeDef i2cTransfert;
  uint8_t values[2] = {0,0};
  values[0] = _REGISTER_ADDRESS;
  values[1] = _REGISTER_VALUE;
  i2cTransfert.addr = I2C_ADDR;
  i2cTransfert.flags = I2C_FLAG_WRITE_WRITE;
  i2cTransfert.buf[0].len = 1;
  i2cTransfert.buf[0].data = &values[0];
  i2cTransfert.buf[1].len = 1;
  i2cTransfert.buf[1].data = &values[1];
  int _status = I2C_TransferInit(I2C0, &i2cTransfert);
  while ( _status == i2cTransferInProgress) {
    _status = I2C_Transfer(I2C0);
  }
}

void initI2CStruct(void) {
  I2C_Init_TypeDef i2c_init = {
      .enable = true,
      .master = true,
      .freq   = 0,
      .clhr   = i2cClockHLRStandard
  };
  I2C_Init(I2C0, &i2c_init);
}
