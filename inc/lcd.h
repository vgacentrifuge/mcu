/***************************************************************************//**
 * @file
 * @brief Functions for writing to the I2C LCD
 ******************************************************************************/

#pragma once

#include <stdint.h>

#define LCD_LINES 2
#define LCD_COLUMNS 16

void lcd_init();

void lcd_print(char* str);

void lcd_clear();

void lcd_home();

void lcd_set_cursor(uint8_t col, uint8_t line);

void lcd_set_color(uint8_t r, uint8_t g, uint8_t b);
