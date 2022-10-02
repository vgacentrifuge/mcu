/*
 * lcd.h
 *
 *  Created on: Oct 2, 2022
 *      Author: marhaug
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include <stdbool.h>

extern volatile bool i2c_rxInProgress;
extern volatile bool i2c_startTx;

void initCMU(void);

void initGPIO(void);

void enableI2cSlaveInterrupts(void);

void disableI2cInterrupts(void);

void initI2C(void);

void performI2CTransfer(void);

void receiveI2CData(void);

void I2C0_IRQHandler(void);

void GPIO_EVEN_IRQHandler(void);

void GPIO_ODD_IRQHandler(void);

#endif /* INC_LCD_H_ */
