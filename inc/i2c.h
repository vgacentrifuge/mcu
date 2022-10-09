/*
 * i2c.h
 *
 *  Created on: Oct 9, 2022
 *      Author: marhaug
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "em_i2c.h"

#define SDA_port  2 // ?
#define SDA_pin   11 // 7
#define SDA_default true

#define SCL_port  2
#define SCL_pin   10 // 9
#define SCL_default true

void initI2C(void);
void initI2CStruct(void);


#endif /* INC_I2C_H_ */
