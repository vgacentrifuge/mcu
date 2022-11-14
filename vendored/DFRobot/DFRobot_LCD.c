/*!
 * @file DFRobot_LCD.c
 * @brief DFRobot's LCD, modified from Arduino cpp to Gecko c
 * @n High Accuracy Ambient Light Sensor
 *
 * @copyright [DFRobot](http://www.dfrobot.com), 2016
 * @copyright GNU Lesser General Public License
 *
 * @author [yangyang](971326313@qq.com)
 * @version  V1.0
 * @date  2017-2-10
 */

#include "DFRobot_LCD.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "sl_sleeptimer.h"

#define DEFAULT_CHARSIZE LCD_5x8DOTS
void DFR_begin(uint8_t cols, uint8_t rows, uint8_t charsize);
void DFR_send(uint8_t *data, uint8_t len);
void DFR_setReg(uint8_t addr, uint8_t data);
uint8_t _showfunction;
uint8_t _showcontrol;
uint8_t _showmode;
uint8_t _initialized;
uint8_t _numlines,_currline;
uint8_t _lcdAddr;
uint8_t _RGBAddr;
uint8_t _cols;
uint8_t _rows;
uint8_t _backlightval;

#include "sl_i2cspm.h"
sl_i2cspm_t *_i2cspm;

const uint8_t color_define[4][3] =
{
    {255, 255, 255},            // white
    {255, 0, 0},                // red
    {0, 255, 0},                // green
    {0, 0, 255},                // blue
};

/*******************************public*******************************/
void DFR_init(uint8_t lcd_cols, uint8_t lcd_rows, sl_i2cspm_t *i2cspm, uint8_t lcd_Addr, uint8_t RGB_Addr)
{
    _i2cspm = i2cspm;
    _lcdAddr = lcd_Addr;
    _RGBAddr = RGB_Addr;
    _cols = lcd_cols;
    _rows = lcd_rows;
    _showfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    DFR_begin(_cols, _rows, DEFAULT_CHARSIZE);
}

void DFR_clear()
{
    DFR_command(LCD_CLEARDISPLAY);        // clear display, set cursor position to zero
    sl_sleeptimer_delay_millisecond(2);          // this command takes a long time!
}

void DFR_home()
{
    DFR_command(LCD_RETURNHOME);        // set cursor position to zero
    sl_sleeptimer_delay_millisecond(2);        // this command takes a long time!
}

void DFR_noDisplay()
{
    _showcontrol &= ~LCD_DISPLAYON;
    DFR_command(LCD_DISPLAYCONTROL | _showcontrol);
}

void DFR_display() {
    _showcontrol |= LCD_DISPLAYON;
    DFR_command(LCD_DISPLAYCONTROL | _showcontrol);
}

void DFR_stopBlink()
{
    _showcontrol &= ~LCD_BLINKON;
    DFR_command(LCD_DISPLAYCONTROL | _showcontrol);
}
void DFR_blink()
{
    _showcontrol |= LCD_BLINKON;
    DFR_command(LCD_DISPLAYCONTROL | _showcontrol);
}

void DFR_noCursor()
{
    _showcontrol &= ~LCD_CURSORON;
    DFR_command(LCD_DISPLAYCONTROL | _showcontrol);
}

void DFR_cursor() {
    _showcontrol |= LCD_CURSORON;
    DFR_command(LCD_DISPLAYCONTROL | _showcontrol);
}

void DFR_scrollDisplayLeft(void)
{
    DFR_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void DFR_scrollDisplayRight(void)
{
    DFR_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void DFR_leftToRight(void)
{
    _showmode |= LCD_ENTRYLEFT;
    DFR_command(LCD_ENTRYMODESET | _showmode);
}

void DFR_rightToLeft(void)
{
    _showmode &= ~LCD_ENTRYLEFT;
    DFR_command(LCD_ENTRYMODESET | _showmode);
}

void DFR_noAutoscroll(void)
{
    _showmode &= ~LCD_ENTRYSHIFTINCREMENT;
    DFR_command(LCD_ENTRYMODESET | _showmode);
}

void DFR_autoscroll(void)
{
    _showmode |= LCD_ENTRYSHIFTINCREMENT;
    DFR_command(LCD_ENTRYMODESET | _showmode);
}

void DFR_customSymbol(uint8_t location, uint8_t charmap[])
{
    location &= 0x7; // we only have 8 locations 0-7
    DFR_command(LCD_SETCGRAMADDR | (location << 3));


    uint8_t data[9];
    data[0] = 0x40;
    for(int i=0; i<8; i++)
    {
        data[i+1] = charmap[i];
    }
    DFR_send(data, 9);
}

void DFR_setCursor(uint8_t col, uint8_t row)
{

    col = (row == 0 ? col|0x80 : col|0xc0);
    uint8_t data[3] = {0x80, col};

    DFR_send(data, 2);

}

void DFR_setRGB(uint8_t r, uint8_t g, uint8_t b)
{
    DFR_setReg(REG_RED, r);
    DFR_setReg(REG_GREEN, g);
    DFR_setReg(REG_BLUE, b);
}

void DFR_setPWM(uint8_t color, uint8_t pwm)
{
    DFR_setReg(color, pwm);
}

void DFR_setColor(uint8_t color)
{
    if(color > 3)return;
    DFR_setRGB(color_define[color][0], color_define[color][1], color_define[color][2]);
}

void DFR_setColorAll()
{
    DFR_setRGB(0, 0, 0);
}

void DFR_setColorWhite()
{
    DFR_setRGB(255, 255, 255);
}

void DFR_blinkLED(void)
{
    ///< blink period in seconds = (<reg 7> + 1) / 24
    ///< on/off ratio = <reg 6> / 256
    DFR_setReg(0x07, 0x17);  // blink every second
    DFR_setReg(0x06, 0x7f);  // half on, half off
}

void DFR_noBlinkLED(void)
{
    DFR_setReg(0x07, 0x00);
    DFR_setReg(0x06, 0xff);
}

void DFR_write(uint8_t value)
{
    uint8_t data[3] = {0x40, value};
    DFR_send(data, 2);
}

void DFR_print(char *str)
{
    for(; *str != '\0'; str++)
      DFR_write(*str);
}

inline void DFR_command(uint8_t value)
{
    uint8_t data[3] = {0x80, value};
    DFR_send(data, 2);
}

/*******************************aliases*******************************/
void DFR_blink_on(){
    DFR_blink();
}

void DFR_blink_off(){
    DFR_stopBlink();
}

void DFR_cursor_on(){
    DFR_cursor();
}

void DFR_cursor_off(){
    DFR_noCursor();
}

void DFR_setBacklight(uint8_t new_val){
  if(new_val){
      DFR_blinkLED();   // turn backlight on
  }else{
      DFR_noBlinkLED();   // turn backlight off
  }
}

void DFR_load_custom_character(uint8_t char_num, uint8_t *rows){
    DFR_customSymbol(char_num, rows);
}

/*******************************private*******************************/
void DFR_begin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
    (void)cols;

    if (lines > 1) {
        _showfunction |= LCD_2LINE;
    }
    _numlines = lines;
    _currline = 0;

    ///< for some 1 line displays you can select a 10 pixel high font
    if ((dotsize != 0) && (lines == 1)) {
        _showfunction |= LCD_5x10DOTS;
    }

    ///< SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    ///< according to datasheet, we need at least 40ms after power rises above 2.7V
    ///< before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    sl_sleeptimer_delay_millisecond(50);


    ///< this is according to the hitachi HD44780 datasheet
    ///< page 45 figure 23

    ///< Send function set command sequence
    DFR_command(LCD_FUNCTIONSET | _showfunction);
    sl_sleeptimer_delay_millisecond(5);  // wait more than 4.1ms

  ///< second try
    DFR_command(LCD_FUNCTIONSET | _showfunction);
    sl_sleeptimer_delay_millisecond(5);

    ///< third go
    DFR_command(LCD_FUNCTIONSET | _showfunction);




    ///< turn the display on with no cursor or blinking default
    _showcontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    DFR_display();

    ///< clear it off
    DFR_clear();

    ///< Initialize to default text direction (for romance languages)
    _showmode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    ///< set the entry mode
    DFR_command(LCD_ENTRYMODESET | _showmode);


    ///< backlight init
    DFR_setReg(REG_MODE1, 0);
    ///< set LEDs controllable by both PWM and GRPPWM registers
    DFR_setReg(REG_OUTPUT, 0xFF);
    ///< set MODE2 values
    ///< 0010 0000 -> 0x20  (DMBLNK to 1, ie blinky mode)
    DFR_setReg(REG_MODE2, 0x20);

    DFR_setColorWhite();

}

#include "debug.h"
static inline void checked_transfer(I2C_TransferSeq_TypeDef *seq) {
    int ret = I2CSPM_Transfer(_i2cspm, seq);
    if(ret != 0) {
        char out[20];
        snprintf(out, sizeof(out), "LCD fail: %d", ret);
        debug_println(out);
    }
}

void DFR_send(uint8_t *data, uint8_t len)
{
    I2C_TransferSeq_TypeDef transfer = {
        .addr = _lcdAddr + 0, //LSB 0 is write
        .flags = I2C_FLAG_WRITE,
        .buf[0]={data, len},
        };
    checked_transfer(&transfer);
}

void DFR_setReg(uint8_t addr, uint8_t data)
{
    uint8_t buffer[2] = {addr, data};

    I2C_TransferSeq_TypeDef transfer = {
          .addr = _RGBAddr + 0, //LSB 0 is write
          .flags = I2C_FLAG_WRITE,
          .buf[0] = {buffer, 2}
          };
    checked_transfer(&transfer);
}

