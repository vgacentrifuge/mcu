/*
 * lcd.c
 *
 *  Created on: Oct 9, 2022
 *      Author: marhaug
 */

#include "sl_i2cspm_instances.h"
#include "DFRobot/DFRobot_LCD.h"

void lcd_init() {
  DFR_init(16, 2, sl_i2cspm_i2c1, LCD_ADDRESS, RGB_ADDRESS);
}

void lcd_print(char* str) {
  DFR_print(str);
}

void lcd_clear() {
  DFR_clear();
}

/**
 * Moves the cursor to the top right corner, aka (0,0)
 */
void lcd_home() {
  DFR_home();
}

/**
 * Moves the cursor to the given column, line-pair.
 * Both are 0-indexed, so (0,0) is home
 */
void lcd_set_cursor(uint8_t col, uint8_t line) {
  DFR_setCursor(col, line);
}
