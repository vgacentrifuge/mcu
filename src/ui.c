#include "ui.h"
#include "lcd.h"
#include "keypad.h"
#include "debug.h"
#include "ddc_data.h"
#include "state.h"
#include <stdio.h>

#define PSTATE_KEY  KEY_INDEX(0,0)
#define NSTATE_KEY  KEY_INDEX(1,0)

#define TRANSPLUS_KEY  KEY_INDEX(0,2)
#define TRANSMINUS_KEY KEY_INDEX(1,2)

#define SCALEPLUS_KEY  KEY_INDEX(3,0)
#define SCALEMINUS_KEY KEY_INDEX(3,1)

#define CHROMA_KEY  KEY_INDEX(0,1)
#define OVERLAY_KEY KEY_INDEX(0,2)
#define NONE_KEY    KEY_INDEX(0,3)

#define RESET_ALL_KEY KEY_INDEX(1,1)
#define MENU_KEY      KEY_INDEX(3,2)

enum {
  UI_TRANSITION, // Lets the lcd freeze before switching mode
  UI_MIXING, // The default mixing view
  UI_OPTIONS,
  UI_INIT,
};

static int ui_state = UI_TRANSITION;
#define NUM_STATES 10

static int ui_state;

struct State states[NUM_STATES];

struct State curr_state;

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
  if(ui_transition_frames_left <= 0)
    ui_transition_callback();
  else
    ui_transition_frames_left--;
}

struct State state_init(uint8_t index);
void reset_state(struct State* state);

void ui_init(void)
{

  ui_state = UI_INIT;

  lcd_custom_symbol(0, heart);
  for (uint8_t i = 0; i < NUM_STATES; i++)
    states[i] = state_init(i);

  curr_state = states[0];
}


struct State state_init(uint8_t index)
{
  // TODO initalize with actual values and fill all values
  struct State state;
  state.index = index;

  _STATE_INIT(state);

  return state;
}


void reset_state(struct State* state) {
  _STATE_INIT((*state));
}

static int ui_mixing_blinker;

void open_ui_mixing() {
  ui_state = UI_MIXING;

  ui_mixing_blinker = 0;

  lcd_clear();
  lcd_set_cursor(0, 0);
  switch (curr_state.mode)
  {
    case CHROMA:  lcd_print("CHROMA"); break;
    case OVERLAY: lcd_print("OVRLAY"); break;
    case NONE:    lcd_print("NONE  "); break;
  }

  lcd_set_cursor(0, 1);

  lcd_write((unsigned char)0); // Does not work?

  lcd_set_cursor(8, 0);
  char buff[16];
  snprintf(buff, sizeof(buff), "%d", curr_state.index);
  lcd_print(buff);

  lcd_set_cursor(10, 0);
  snprintf(buff, sizeof(buff), "X%5d", curr_state.fg_x_offset);
  lcd_print(buff);
  lcd_set_cursor(10, 1);
  snprintf(buff, sizeof(buff), "Y%5d", curr_state.fg_y_offset);
  lcd_print(buff);


  lcd_set_cursor(3, 1);
  snprintf(buff, sizeof(buff), "SC%d%%", curr_state.fg_scale);
  lcd_print(buff);
}

#define MAX_Y_VAL  9999
#define MIN_Y_VAL -9999
#define MAX_X_VAL  9999
#define MIN_X_VAL -9999

void update_ui_mixing() {
  if (keypad_keypressed(KEY_DOWN)) {
     if (curr_state.fg_y_offset > MIN_Y_VAL) {
         curr_state.fg_y_offset--;
     }
     open_ui_mixing();
  }
  if (keypad_keypressed(KEY_UP)) {
     if (curr_state.fg_y_offset < MAX_Y_VAL) {
         curr_state.fg_y_offset++;
     }
     open_ui_mixing();
  }
  if (keypad_keypressed(KEY_LEFT)) {
     if (curr_state.fg_y_offset > MIN_X_VAL) {
         curr_state.fg_x_offset--;
     }
     open_ui_mixing();
  }
  if (keypad_keypressed(KEY_RIGHT)) {
     if (curr_state.fg_y_offset < MAX_X_VAL) {
         curr_state.fg_x_offset++;
     }
     open_ui_mixing();
  }

  if (keypad_keypressed(MENU_KEY)) {
      open_ui_options();
  }

  if (keypad_keypressed(CHROMA_KEY))
  {
      curr_state.mode = CHROMA;
      open_ui_mixing();
  }
  if (keypad_keypressed(OVERLAY_KEY))
  {
      curr_state.mode = OVERLAY;
      open_ui_mixing();
  }
  if (keypad_keypressed(NONE_KEY))
  {
      curr_state.mode = NONE;
      open_ui_mixing();
  }

  if (keypad_keypressed(RESET_ALL_KEY))
  {
      reset_state(&curr_state);
      open_ui_mixing();
  }

  if (keypad_keypressed(SCALEPLUS_KEY)) {
      if (curr_state.fg_scale < 100)
        curr_state.fg_scale += 25;
      open_ui_mixing();
  }

  if (keypad_keypressed(SCALEMINUS_KEY)) {
      if (curr_state.fg_scale > 0)
        curr_state.fg_scale -= 25;
      open_ui_mixing();
  }

  if (keypad_keypressed(NSTATE_KEY)) {
      if (curr_state.index < NUM_STATES-1) {
          // save old state
          states[curr_state.index] = curr_state;
          curr_state = states[curr_state.index + 1];
      }
      open_ui_mixing();
  }
  if (keypad_keypressed(PSTATE_KEY)) {
      if (curr_state.index > 0) {
          // save old state
          states[curr_state.index] = curr_state;
          curr_state = states[curr_state.index - 1];
      }
      open_ui_mixing();
  }
}

void open_ui_options() {
  ui_state = UI_OPTIONS;
  lcd_clear();
  lcd_print("      MENU      ");
  if (keypad_keypressed(KEY_LEFT)) {
      open_ui_mixing();
  }
}

void update_ui_options() {
  if(keypad_keypressed(KEY_DOWN))
    open_ui_mixing();
  if(keypad_keypressed(KEY_INDEX(0,0))) {
    open_ui_mixing();
    // flash_ddc_eeprom(DDC_EEPROM1);
  }
  if(keypad_keypressed(KEY_INDEX(1,0))) {
    open_ui_mixing();
    // flash_ddc_eeprom(DDC_EEPROM2);
  }
}

void ui_update() {
  switch(ui_state) {
    case UI_INIT:       open_ui_mixing();     break;
    case UI_MIXING:     update_ui_mixing();   break;
    case UI_OPTIONS:    open_ui_options();    break;
  }
}
