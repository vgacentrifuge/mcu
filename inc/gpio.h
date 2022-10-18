/*
 * gpio.h
 *
 *  Created on: Oct 9, 2022
 *      Author: marhaug
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include <em_gpio.h>

// PB0
#define BUTTON_PORT     gpioPortB
#define BUTTON_PIN      9

// PB1
#define BUTTON_2_PORT   gpioPortB
#define BUTTON_2_PIN    10

void initGPIO(void);




#endif /* INC_GPIO_H_ */
