#include "ui.h"
#include "lcd.h"
#include "keypad.h"
#include "debug.h"
#include "ddc_data.h"
#include "state.h"
#include "image.h"
#include <stdio.h>

enum {
  UI_TRANSITION, // Lets the lcd freeze before switching mode
  UI_MIXING, // The default mixing view
  UI_OPTIONS, // The menu
  UI_IMAGE_UPLOAD, // Uploading an image
};

static int ui_state;

static void ui_open_transition(int frames, void (*callback)());
static void ui_open_mixing();
static void ui_open_options();
static void ui_open_image_upload();

enum {
  PLAY_SYMBOL_INDEX = 0,
  PAUSE_SYMBOL_INDEX = 1,
  IMG_SYMBOL_INDEX = 2,

  TRANS0_SYMBOL_INDEX = 3,
  TRANS25_SYMBOL_INDEX = 4,
  TRANS50_SYMBOL_INDEX = 5,
  TRANS75_SYMBOL_INDEX = 6,
};
static const uint8_t PLAY_SYMBOL[8]  = { 0b00000, 0b10000, 0b11100, 0b11111, 0b11111, 0b11100, 0b10000, 0b00000 };
static const uint8_t PAUSE_SYMBOL[8] = { 0b11011, 0b11011, 0b11011, 0b11011, 0b11011, 0b11011, 0b11011, 0b11011 };
static const uint8_t IMG_SYMBOL[8]   = { 0b00000, 0b11111, 0b11001, 0b10001, 0b10111, 0b11111, 0b11111, 0b11111 };
static const uint8_t TRANS0_SYMBOL[8]  = { 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b10000 };
static const uint8_t TRANS25_SYMBOL[8] = { 0b00000, 0b10101, 0b11111, 0b01010, 0b11111, 0b10101, 0b11111, 0b10000 };
static const uint8_t TRANS50_SYMBOL[8] = { 0b00000, 0b10101, 0b01010, 0b10101, 0b01010, 0b10101, 0b01010, 0b10000 };
static const uint8_t TRANS75_SYMBOL[8] = { 0b00000, 0b01010, 0b00000, 0b10101, 0b00000, 0b01010, 0b00000, 0b10000 };

void ui_init(void) {
  lcd_custom_symbol(PLAY_SYMBOL_INDEX, PLAY_SYMBOL);
  lcd_custom_symbol(PAUSE_SYMBOL_INDEX, PAUSE_SYMBOL);
  lcd_custom_symbol(IMG_SYMBOL_INDEX, IMG_SYMBOL);

  lcd_custom_symbol(TRANS0_SYMBOL_INDEX, TRANS0_SYMBOL);
  lcd_custom_symbol(TRANS25_SYMBOL_INDEX, TRANS25_SYMBOL);
  lcd_custom_symbol(TRANS50_SYMBOL_INDEX, TRANS50_SYMBOL);
  lcd_custom_symbol(TRANS75_SYMBOL_INDEX, TRANS75_SYMBOL);

  for (uint8_t i = 0; i < NUM_MIXING_STATES; i++)
    mixing_states[i] = INITIAL_STATE;

  current_mixing_state = 0;

  // We immediately transition into the mixing state
  ui_open_transition(0, &ui_open_mixing);
}

static int ui_transition_frames_left;
static void (*ui_transition_callback)();

static void ui_open_transition(int frames, void (*callback)()) {
  ui_state = UI_TRANSITION;
  ui_transition_frames_left = frames;
  ui_transition_callback = callback;
}

static void ui_update_transition() {
  if (ui_transition_frames_left-- <= 0)
    ui_transition_callback();
}

static void ui_open_mixing() {
  ui_state = UI_MIXING;

  lcd_clear();
  lcd_set_cursor(0, 0);
  switch (CURR_STATE.fg_blend_mode) {
    case FG_BLEND_NONE: lcd_print("NONE  "); break;
    case FG_BLEND_OVERLAY: lcd_print("OVRLAY"); break;
    case FG_BLEND_CHROMA: lcd_print("CHROMA"); break;
  }

  char buff[16];

  lcd_set_cursor(0, 1);
  switch (CURR_STATE.image_path_hash) {
    case FG_IS_LIVE:   lcd_write(PLAY_SYMBOL_INDEX); break;
    case FG_IS_FROZEN: lcd_write(PAUSE_SYMBOL_INDEX); break;
    default:           lcd_write(IMG_SYMBOL_INDEX); break;
  }

  lcd_set_cursor(6, 0);
  switch (CURR_STATE.fg_transparency) {
    case FG_TRANSPARENCY_0:  lcd_write(TRANS0_SYMBOL_INDEX); break;
    case FG_TRANSPARENCY_25: lcd_write(TRANS25_SYMBOL_INDEX); break;
    case FG_TRANSPARENCY_50: lcd_write(TRANS50_SYMBOL_INDEX); break;
    case FG_TRANSPARENCY_75: lcd_write(TRANS75_SYMBOL_INDEX); break;
  }

  lcd_set_cursor(8, 0);
  snprintf(buff, sizeof(buff), "%d", current_mixing_state);
  lcd_print(buff);

  lcd_set_cursor(10, 0);
  snprintf(buff, sizeof(buff), "X%+05d", CURR_STATE.fg_x_offset);
  lcd_print(buff);
  lcd_set_cursor(10, 1);
  snprintf(buff, sizeof(buff), "Y%+05d", CURR_STATE.fg_y_offset);
  lcd_print(buff);

  lcd_set_cursor(3, 1);
  switch (CURR_STATE.fg_scale) {
    case FG_SCALE_100: lcd_print("SC100%"); break;
    case FG_SCALE_50: lcd_print("SC 50%"); break;
    case FG_SCALE_25: lcd_print("SC 25%"); break;
  }
}

#define MAX_X_VAL  800
#define MIN_X_VAL -800
#define MAX_Y_VAL  600
#define MIN_Y_VAL -600

#define REPEAT_DELAY 15
#define REPEAT_PERIOD 5
#define KEY_DOWN_OR_REPEAT(KEY) \
    (keypad_keydown(KEY, &was_down_frames) \
    && ((was_down_frames > REPEAT_DELAY && (was_down_frames - REPEAT_DELAY) % REPEAT_PERIOD == 0) \
    || was_down_frames == 1))

static void ui_update_mixing() {
  bool dirty = false;
  int was_down_frames;
  if (KEY_DOWN_OR_REPEAT(KEY_DOWN)) {
    if (CURR_STATE.fg_y_offset > MIN_Y_VAL) {
      CURR_STATE.fg_y_offset--;
      dirty = true;
    }
  }
  if (KEY_DOWN_OR_REPEAT(KEY_UP)) {
    if (CURR_STATE.fg_y_offset < MAX_Y_VAL) {
      CURR_STATE.fg_y_offset++;
      dirty = true;
    }
  }
  if (KEY_DOWN_OR_REPEAT(KEY_LEFT)) {
    if (CURR_STATE.fg_x_offset > MIN_X_VAL) {
      CURR_STATE.fg_x_offset--;
      dirty = true;
    }
  }
  if (KEY_DOWN_OR_REPEAT(KEY_RIGHT)) {
    if (CURR_STATE.fg_x_offset < MAX_X_VAL) {
      CURR_STATE.fg_x_offset++;
      dirty = true;
    }
  }
  if (keypad_keypressed(KEY_RESET_OFFSET)) {
    CURR_STATE.fg_x_offset = 0;
    CURR_STATE.fg_y_offset = 0;
    dirty = true;
  }

  if (keypad_keypressed(KEY_MENU)) {
    ui_open_options();
  }

  if (keypad_keypressed(KEY_CHROMA_KEY)) {
    CURR_STATE.fg_blend_mode = FG_BLEND_CHROMA;
    dirty = true;
  }
  if (keypad_keypressed(KEY_OVERLAY_KEY)) {
    CURR_STATE.fg_blend_mode = FG_BLEND_OVERLAY;
    dirty = true;
  }
  if (keypad_keypressed(KEY_NONE)) {
    CURR_STATE.fg_blend_mode = FG_BLEND_NONE;
    dirty = true;
  }

  if (keypad_keypressed(KEY_RESET_ALL)) {
    state_send_reset();
    CURR_STATE = INITIAL_STATE;
    dirty = true;
  }

  if (keypad_keypressed(KEY_TRANSMINUS)) {
    if (CURR_STATE.fg_transparency < FG_TRANSPARENCY_MAX) {
      CURR_STATE.fg_transparency++;
      dirty = true;
    }
  }
  if (keypad_keypressed(KEY_TRANSPLUS)) {
    if (CURR_STATE.fg_transparency > 0) {
      CURR_STATE.fg_transparency--;
      dirty = true;
    }
  }

  if (keypad_keypressed(KEY_SCALEDOWN)) {
    if (CURR_STATE.fg_scale < FG_SCALE_MAX) {
      CURR_STATE.fg_scale++;
      dirty = true;
    }
  }
  if (keypad_keypressed(KEY_SCALEUP)) {
    if (CURR_STATE.fg_scale > 0) {
      CURR_STATE.fg_scale--;
      dirty = true;
    }
  }

  if (keypad_keypressed(KEY_NSTATE)) {
    current_mixing_state++;
    current_mixing_state %= NUM_MIXING_STATES;
    dirty = true;
  }
  if (keypad_keypressed(KEY_PSTATE)) {
    current_mixing_state += (NUM_MIXING_STATES-1);
    current_mixing_state %= NUM_MIXING_STATES;
    dirty = true;
  }

  if (dirty) {
    int res = state_send_changes();
    switch(res) {
      case STATE_SEND_OK:
        ui_open_mixing();
        break;
      case STATE_SEND_IMAGE_STARTED:
        ui_open_image_upload();
        break;
      case STATE_SEND_IMAGE_FAILED:
        lcd_clear();
        lcd_print("> IMAGE ERROR! <");
        ui_open_transition(60, &ui_open_mixing);
        break;
    }
  }
}

static void ui_open_options() {
  ui_state = UI_OPTIONS;
  lcd_clear();
  lcd_print("      MENU      ");
}

static void ui_update_options() {
  if (keypad_keypressed(KEY_MENU))
    ui_open_mixing();

  if (keypad_keypressed(KEY_INDEX(0, 0))) {
    flash_ddc_eeprom(DDC_EEPROM1);
    lcd_clear();
    lcd_print("Flashed EEPROM1!");
    ui_open_transition(60, &ui_open_mixing);
  }
  if (keypad_keypressed(KEY_INDEX(1, 0))) {
    flash_ddc_eeprom(DDC_EEPROM2);
    lcd_clear();
    lcd_print("Flashed EEPROM2!");
    ui_open_transition(60, &ui_open_mixing);
  }
}

static void ui_open_image_upload() {
  ui_state = UI_IMAGE_UPLOAD;
  lcd_clear();
  lcd_print("Uploading image");
}

static void ui_update_image_upload() {
  uint16_t uploaded, total;
  int res = image_upload_next_lines(6, &uploaded, &total);

  switch(res) {
    case IMAGE_UPLOAD_DONE:
      ui_open_mixing();
      return;
    case IMAGE_UPLOAD_ONGOING: {
      char buf[20];
      snprintf(buf, sizeof(buf), " Lines: %3d/%3d ", uploaded%1000, total%1000);
      lcd_set_cursor(0, 1);
      lcd_print(buf);
      return;
    }
    case IMAGE_UPLOAD_ERROR:
      lcd_clear();
      lcd_print("> IMAGE ERROR! <");
      ui_open_transition(60, &ui_open_mixing);
      return;
  }
}

void ui_update() {
  switch (ui_state) {
  case UI_TRANSITION:
    ui_update_transition();
    break;
  case UI_MIXING:
    ui_update_mixing();
    break;
  case UI_OPTIONS:
    ui_update_options();
    break;
  case UI_IMAGE_UPLOAD:
    ui_update_image_upload();
    break;
  }
}
