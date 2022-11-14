#include "ui.h"
#include "lcd.h"
#include "keypad.h"
#include "debug.h"
#include "ddc_data.h"
#include "state.h"
#include <stdio.h>

enum {
  UI_TRANSITION, // Lets the lcd freeze before switching mode
  UI_MIXING, // The default mixing view
  UI_OPTIONS, // The menu
};

static int ui_state;

static int ui_state;

#define NUM_MIXING_STATES 10
static State mixing_states[NUM_MIXING_STATES];
static int current_mixing_state;
#define CURR_STATE mixing_states[current_mixing_state]

// TODO example - replace with real symbols
unsigned char heart[8]  = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000
};

void ui_init(void)
{
  lcd_custom_symbol(0, heart);

  for (uint8_t i = 0; i < NUM_MIXING_STATES; i++)
    mixing_states[i] = INITIAL_STATE;

  current_mixing_state = 0;

  // We immediately transition into the mixing state
  ui_open_transition(0, &ui_open_mixing);
}

static int ui_transition_frames_left;
static void (*ui_transition_callback)();

void ui_open_transition(int frames, void (*callback)()) {
  ui_state = UI_TRANSITION;
  ui_transition_frames_left = frames;
  ui_transition_callback = callback;
}

void ui_update_transition() {
  if(ui_transition_frames_left-- <= 0)
    ui_transition_callback();
}

void ui_open_mixing() {
  ui_state = UI_MIXING;

  lcd_clear();
  lcd_set_cursor(0, 0);
  switch (CURR_STATE.fg_blend_mode) {
    case FG_BLEND_CHROMA:  lcd_print("CHROMA"); break;
    case FG_BLEND_NORMAL:  lcd_print("OVRLAY"); break;
    case FG_BLEND_NONE:    lcd_print("NONE  "); break;
  }

  char buff[16];

  lcd_set_cursor(0, 1);
  //TODO use custom chat
  snprintf(buff, sizeof(buff), "%d", CURR_STATE.fg_frozen);
  lcd_print(buff);

  lcd_set_cursor(8, 0);
  // TODO use custom char
  snprintf(buff, sizeof(buff), "%d", CURR_STATE.fg_transparancy);
  lcd_print(buff);

  lcd_set_cursor(10, 0);
  snprintf(buff, sizeof(buff), "X%5d", CURR_STATE.fg_x_offset);
  lcd_print(buff);
  lcd_set_cursor(10, 1);
  snprintf(buff, sizeof(buff), "Y%5d", CURR_STATE.fg_y_offset);
  lcd_print(buff);

  lcd_set_cursor(3, 1);
  switch (CURR_STATE.fg_scale) {
    case FG_SCALE_100:  lcd_print("SC100%"); break;
    case FG_SCALE_50:   lcd_print("SC 50%"); break;
    case FG_SCALE_25:   lcd_print("SC 25%"); break;
  }
}

#define MAX_X_VAL  800
#define MIN_X_VAL -800
#define MAX_Y_VAL  600
#define MIN_Y_VAL -600

void ui_update_mixing() {
  if (keypad_keypressed(KEY_DOWN)) {
    if (CURR_STATE.fg_y_offset > MIN_Y_VAL) {
      CURR_STATE.fg_y_offset--;
    }
    ui_open_mixing();
  }
  if (keypad_keypressed(KEY_UP)) {
    if (CURR_STATE.fg_y_offset < MAX_Y_VAL) {
       CURR_STATE.fg_y_offset++;
    }
    ui_open_mixing();
  }
  if (keypad_keypressed(KEY_LEFT)) {
    if (CURR_STATE.fg_y_offset > MIN_X_VAL) {
       CURR_STATE.fg_x_offset--;
    }
    ui_open_mixing();
  }
  if (keypad_keypressed(KEY_RIGHT)) {
    if (CURR_STATE.fg_y_offset < MAX_X_VAL) {
       CURR_STATE.fg_x_offset++;
    }
    ui_open_mixing();
  }

  if (keypad_keypressed(MENU_KEY)) {
     ui_open_options();
  }

  if (keypad_keypressed(CHROMA_KEY)) {
    CURR_STATE.fg_blend_mode = FG_BLEND_CHROMA;
    ui_open_mixing();
  }
  if (keypad_keypressed(OVERLAY_KEY)) {
    CURR_STATE.fg_blend_mode = FG_BLEND_NORMAL;
    ui_open_mixing();
  }
  if (keypad_keypressed(NONE_KEY)) {
    CURR_STATE.fg_blend_mode = FG_BLEND_NONE;
    ui_open_mixing();
  }

  if (keypad_keypressed(RESET_ALL_KEY)) {
    CURR_STATE = INITIAL_STATE;
    ui_open_mixing();
  }

  if (keypad_keypressed(SCALEPLUS_KEY)) {
    if (CURR_STATE.fg_scale+1 < FG_SCALE_MAX) {
      CURR_STATE.fg_scale++;
      ui_open_mixing();
    }
  }

  if (keypad_keypressed(SCALEMINUS_KEY)) {
    if (CURR_STATE.fg_scale > 0) {
      CURR_STATE.fg_scale--;
      ui_open_mixing();
    }
  }

  if (keypad_keypressed(NSTATE_KEY)) {
    current_mixing_state++;
    current_mixing_state %= NUM_MIXING_STATES;
    ui_open_mixing();
  }
  if (keypad_keypressed(PSTATE_KEY)) {
    current_mixing_state += (NUM_MIXING_STATES-1);
    current_mixing_state %= NUM_MIXING_STATES;
    ui_open_mixing();
  }
}

void ui_open_options() {
  ui_state = UI_OPTIONS;
  lcd_clear();
  lcd_print("      MENU      ");
  if (keypad_keypressed(KEY_LEFT)) {
    ui_open_mixing();
  }
}

void ui_update_options() {
  if(keypad_keypressed(KEY_DOWN))
    ui_open_mixing();
  if(keypad_keypressed(KEY_INDEX(0,0))) {
    flash_ddc_eeprom(DDC_EEPROM1);
    lcd_clear();
    lcd_print("Flashed EEPROM1!");
    ui_open_transition(60, &ui_open_mixing);
  }
  if(keypad_keypressed(KEY_INDEX(1,0))) {
    flash_ddc_eeprom(DDC_EEPROM2);
    lcd_clear();
    lcd_print("Flashed EEPROM2!");
    ui_open_transition(60, &ui_open_mixing);
  }
}

void ui_update() {
  switch(ui_state) {
  case UI_TRANSITION:
    ui_update_transition();
    break;
  case UI_MIXING:
    ui_update_mixing();
    break;
  case UI_OPTIONS:
    ui_update_options();
    break;
  }
}
