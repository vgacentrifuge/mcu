/*!
 * @file DFRobot_LCD.h
 * @brief DFRobot's LCD
 * @n High Accuracy Ambient Light Sensor
 *
 * @copyright [DFRobot](http://www.dfrobot.com), 2016
 * @copyright GNU Lesser General Public License
 *
 * @author [yangyang](971326313@qq.com)
 * @version  V1.0
 * @date  2017-2-10
 */

#pragma once

#include <inttypes.h>
#include "sl_i2cspm.h"

/*!
 *  @brief Device I2C Address
 */
#define LCD_ADDRESS     0x7c
#define RGB_ADDRESS     0xc0

/*!
 *  @brief color define
 */
#define WHITE           0
#define RED             1
#define GREEN           2
#define BLUE            3
#define ONLY            3

#define REG_RED         0x04        // pwm2
#define REG_GREEN       0x03        // pwm1
#define REG_BLUE        0x02        // pwm0
#define REG_ONLY        0x02

#define REG_MODE1       0x00
#define REG_MODE2       0x01
#define REG_OUTPUT      0x08

/*!
 *  @brief commands
 */
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

/*!
 *  @brief flags for display entry mode
 */
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

/*!
 *  @brief flags for display on/off control
 */
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

/*!
 *  @brief flags for display/cursor shift
 */
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

/*!
 *  @brief flags for function set
 */
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

/*!
 *  @brief initialize
 */
void DFR_init(uint8_t lcd_cols, uint8_t lcd_rows, sl_i2cspm_t *i2cspm, uint8_t lcd_Addr, uint8_t RGB_Addr);

void DFR_clear();
void DFR_home();

/*!
 *  @brief Turn the display on/off (quickly)
 */
void DFR_noDisplay();
void DFR_display();

/*!
 *  @brief Turn on and off the blinking showCursor
 */
void DFR_stopBlink();
void DFR_blink();

/*!
 *  @brief Turns the underline showCursor on/off
 */
void DFR_noCursor();
void DFR_cursor();

/*!
 *  @brief These commands scroll the display without changing the RAM
 */
void DFR_scrollDisplayLeft();
void DFR_scrollDisplayRight();

/*!
 *  @brief This is for text that flows Left to Right
 */
void DFR_leftToRight();

/*!
 *  @brief This is for text that flows Right to Left
 */
void DFR_rightToLeft();

/*!
 *  @brief This will 'left justify' text from the showCursor
 */
void DFR_noAutoscroll();

/*!
 *  @brief This will 'right justify' text from the showCursor
 */
void DFR_autoscroll();

/*!
 *  @brief Allows us to fill the first 8 CGRAM locations
 *     with custom characters
 */
void DFR_customSymbol(uint8_t, uint8_t[]);
void DFR_setCursor(uint8_t, uint8_t);

/*!
 *  @brief color control
 */
void DFR_setRGB(uint8_t r, uint8_t g, uint8_t b); // set rgb
void DFR_setPWM(uint8_t color, uint8_t pwm);      // set pwm
void DFR_setColor(uint8_t color);
void DFR_setColorAll();
void DFR_setColorWhite();

/*!
 *  @brief blink the LED backlight
 */
void DFR_blinkLED(void);
void DFR_noBlinkLED(void);

/*!
 *  @brief send data
 */
void DFR_write(uint8_t);
void DFR_print(char *str);

/*!
 *  @brief send command
 */
void DFR_command(uint8_t);

/*!
 *  @brief compatibility API function aliases
 */
void DFR_blink_on();            // alias for blink()
void DFR_blink_off();                 // alias for noBlink()
void DFR_cursor_on();                 // alias for cursor()
void DFR_cursor_off();                // alias for noCursor()
void DFR_setBacklight(uint8_t new_val);       // alias for backlight() and nobacklight()
void DFR_load_custom_character(uint8_t char_num, uint8_t *rows);  // alias for createChar()
