#include "ui.h"
#include "lcd.h"
#include "keypad.h"
#include "debug.h"
#include "ddc_data.h"

enum {
  UI_TRANSITION, // Lets the lcd freeze before switching mode
  UI_MIXING, // The default mixing view
  UI_OPTIONS,
};

static int ui_state = UI_TRANSITION;

void open_ui_mixing();
void update_ui_mixing();
void open_ui_options();
void update_ui_options();

// Default is to instantly transition to UI mixing
static int ui_transition_frames_left = 0;
static void (*ui_transition_callback)() = open_ui_mixing;

void open_ui_transition(int frames, void (*callback)()) {
  ui_transition_frames_left = frames;
  ui_transition_callback = callback;
}

void update_ui_transition() {
  if(ui_transition_frames_left-- <= 0)
    ui_transition_callback();
}

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
  if(keypad_keypressed(KEY_INDEX(0,0))) {
    open_ui_mixing();
    flash_ddc_eeprom(DDC_EEPROM1);
  }
  if(keypad_keypressed(KEY_INDEX(1,0))) {
    open_ui_mixing();
    flash_ddc_eeprom(DDC_EEPROM2);
  }
}

void ui_update() {
  switch(ui_state) {
    case UI_TRANSITION:
      update_ui_transition();
      break;
    case UI_MIXING:
      update_ui_mixing();
      break;
    case UI_OPTIONS:
      update_ui_options();
      break;
  }
}
