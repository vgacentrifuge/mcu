#include "ui.h"
#include "lcd.h"
#include "keypad.h"
#include "debug.h"

// Before anything has been shown. A state that immediatly jumps to MENU_MIXING
#define UI_INIT -1
// The default screen, letting the user modify the current state using buttons
#define UI_MIXING 0
// A list of options, for doing more advanced things
#define UI_OPTIONS 1

static int ui_state = UI_INIT;

void open_ui_mixing();
void update_ui_mixing();
void open_ui_options();
void update_ui_options();

static int ui_mixing_blinker;
void open_ui_mixing() {
  ui_state = UI_MIXING;

  ui_mixing_blinker = 0;

  lcd_clear();
  lcd_print("Curr state: TODO");
}

void update_ui_mixing() {
  if(++ui_mixing_blinker % 15 == 0) {
    lcd_set_cursor(LCD_COLUMNS-1, 0);
    lcd_print(ui_mixing_blinker%2 ? "X":" ");
    ui_mixing_blinker %= 30;
  }

  if(keypad_keypressed(KEY_UP))
    open_ui_options();
}

void open_ui_options() {
  ui_state = UI_OPTIONS;
  lcd_clear();
  lcd_print("      MENU      ");
}

void update_ui_options() {
  if(keypad_keypressed(KEY_DOWN))
    open_ui_mixing();
}

void ui_update() {
  switch(ui_state) {
    case UI_INIT:
      open_ui_mixing();
      break;
    case UI_MIXING:
      update_ui_mixing();
      break;
    case UI_OPTIONS:
      update_ui_options();
      break;
  }
}
