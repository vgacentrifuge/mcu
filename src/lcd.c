#include "lcd.h"
#include "sl_i2cspm_instances.h"
#include "DFRobot/DFRobot_LCD.h"

void lcd_init() {
  DFR_init(16, 2, sl_i2cspm_i2c1, LCD_ADDRESS, RGB_ADDRESS);
}

void lcd_print(char* str) {
  DFR_print(str);
}

/**
 * Clears the display, also sets the cursor to home
 */
void lcd_clear() {
  DFR_clear();
}

/**
 * Writes to lcd, also allows writing the 0 byte
 */
void lcd_write(uint8_t char_num) {
  DFR_write(char_num);
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

/**
 * Creates custom symbol, the ASCII values 0-7 can be customized
 */
void lcd_custom_symbol(uint8_t char_num, uint8_t const* rows) {
  DFR_customSymbol(char_num, rows);
}
