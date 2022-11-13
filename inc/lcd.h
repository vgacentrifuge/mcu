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

void lcd_write(uint8_t);

void lcd_home();

void lcd_set_cursor(uint8_t col, uint8_t line);

void lcd_custom_symbol(uint8_t char_num, uint8_t* rows);
