#include "lcd.h"
#include <stdio.h>

#include "sim_config.h"

#define LINES 2
#define COLS 16
static char data[LINES][COLS];
static uint8_t line, col;

void lcd_init() {}

void lcd_write(uint8_t char_num) {
    if (char_num < 8)
        data[line][col++] = '0'+char_num;
    else
        data[line][col++] = char_num;
}

void lcd_print(char* str) {
    for(;*str != '\0'; str++) {
        if(line >= 0 && line < LINES && col >= 0 && col < COLS)
            lcd_write(*str);
        else
            fprintf(stderr, "LCD overflow: %c\n", *str);
    }
}

void lcd_clear() {
    for(int l = 0; l < LINES; l++) {
        for(int c = 0; c < COLS; c++) {
            data[l][c] = ' ';
        }
    }
    lcd_home();
}

void lcd_home() {
    line = 0;
    col = 0;
}

void lcd_set_cursor(uint8_t new_col, uint8_t new_line) {
  col = new_col;
  line = new_line;
}

#include "raylib.h"

#define LCD_CHAR_WIDTH (LCD_WIDTH/COLS)
#define LCD_CHAR_HEIGHT (LCD_HEIGHT/LINES)

void lcd_draw() {
    BeginMode2D((Camera2D){{LCD_X, LCD_Y}, .zoom=1.0f});
    for(int l = 0; l < LINES; l++) {
        for(int c = 0; c < COLS; c++) {
            char text[2] = {data[l][c], 0};
            DrawRectangle(c*LCD_CHAR_WIDTH, l*LCD_CHAR_HEIGHT, LCD_CHAR_WIDTH-1, LCD_CHAR_HEIGHT-1, LIGHTGRAY);
            DrawText(text, c*LCD_CHAR_WIDTH+2, l*LCD_CHAR_HEIGHT, LCD_CHAR_HEIGHT, BLACK);
        }
    }
    EndMode2D();
}

void lcd_custom_symbol(uint8_t char_num, uint8_t const* rows) {
    (void) char_num;
    (void) rows;
}
