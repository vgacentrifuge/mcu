#include "ui.h"
#include "lcd.h"
#include "keypad.h"
#include "debug.h"
#include "ddc_data.h"
#include "state.h"
#include "fpga_spi.h"
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
    case FG_BLEND_NONE:    lcd_print("NONE  "); break;
    case FG_BLEND_OVERLAY:  lcd_print("OVRLAY"); break;
    case FG_BLEND_CHROMA:  lcd_print("CHROMA"); break;
  }

  char buff[16];

  lcd_set_cursor(0, 1);
  switch (CURR_STATE.fg_image_state) {
    case FG_IS_LIVE:    lcd_write(0); break;
    case FG_IS_FROZEN:  lcd_print("P"); break;
    case FG_IS_IMAGE:   lcd_print("I"); break;
  }

  lcd_set_cursor(6, 0);
  switch (CURR_STATE.fg_transparency) {
    case FG_TRANSPARENCY_0:    lcd_print(";"); break;
    case FG_TRANSPARENCY_25:   lcd_print(":"); break;
    case FG_TRANSPARENCY_50:   lcd_print(","); break;
    case FG_TRANSPARENCY_75:   lcd_print("."); break;
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
    fpga_spi_sendcmd_i16(CMD_FG_OFFSET_Y, CURR_STATE.fg_y_offset);
    ui_open_mixing();
  }
  if (keypad_keypressed(KEY_UP)) {
    if (CURR_STATE.fg_y_offset < MAX_Y_VAL) {
      CURR_STATE.fg_y_offset++;
    }
    fpga_spi_sendcmd_i16(CMD_FG_OFFSET_Y, CURR_STATE.fg_y_offset);
    ui_open_mixing();
  }
  if (keypad_keypressed(KEY_LEFT)) {
    if (CURR_STATE.fg_x_offset > MIN_X_VAL) {
      CURR_STATE.fg_x_offset--;
    }
    fpga_spi_sendcmd_i16(CMD_FG_OFFSET_X, CURR_STATE.fg_x_offset);
    ui_open_mixing();
  }
  if (keypad_keypressed(KEY_RIGHT)) {
    if (CURR_STATE.fg_x_offset < MAX_X_VAL) {
      CURR_STATE.fg_x_offset++;
    }
    fpga_spi_sendcmd_i16(CMD_FG_OFFSET_X, CURR_STATE.fg_x_offset);
    ui_open_mixing();
  }
  if (keypad_keypressed(RESET_OFFSET_KEY)) {
    CURR_STATE.fg_x_offset = 0;
    CURR_STATE.fg_y_offset = 0;
    ui_open_mixing();
    fpga_spi_sendcmd_i16(CMD_FG_OFFSET_X, CURR_STATE.fg_x_offset);
    fpga_spi_sendcmd_i16(CMD_FG_OFFSET_Y, CURR_STATE.fg_y_offset);
  }

  if (keypad_keypressed(MENU_KEY)) {
    ui_open_options();
  }

  if (keypad_keypressed(CHROMA_KEY)) {
    CURR_STATE.fg_blend_mode = FG_BLEND_CHROMA;
    fpga_spi_sendcmd_u8(CMD_FG_MODE, CURR_STATE.fg_blend_mode);
    ui_open_mixing();
  }
  if (keypad_keypressed(OVERLAY_KEY)) {
    CURR_STATE.fg_blend_mode = FG_BLEND_OVERLAY;
    fpga_spi_sendcmd_u8(CMD_FG_MODE, CURR_STATE.fg_blend_mode);
    ui_open_mixing();
  }
  if (keypad_keypressed(NONE_KEY)) {
    CURR_STATE.fg_blend_mode = FG_BLEND_NONE;
    fpga_spi_sendcmd_u8(CMD_FG_MODE, CURR_STATE.fg_blend_mode);
    ui_open_mixing();
  }

  if (keypad_keypressed(RESET_ALL_KEY)) {
    CURR_STATE = INITIAL_STATE;
    // TODO: Might change to send_all_state() to ensure same state on FPGA
    fpga_spi_sendcmd(CMD_RESET);
    ui_open_mixing();
  }

  if (keypad_keypressed(TRANSMINUS_KEY)) {
      if (CURR_STATE.fg_transparency < FG_TRANSPARENCY_MAX) {
        CURR_STATE.fg_transparency++;
        fpga_spi_sendcmd_u8(CMD_FG_TRANSPARENCY, CURR_STATE.fg_transparency);
        ui_open_mixing();
      }
    }
    if (keypad_keypressed(TRANSPLUS_KEY)) {
      if (CURR_STATE.fg_transparency > 0) {
        CURR_STATE.fg_transparency--;
        fpga_spi_sendcmd_u8(CMD_FG_TRANSPARENCY, CURR_STATE.fg_transparency);
        ui_open_mixing();
      }
    }

  if (keypad_keypressed(SCALEDOWN_KEY)) {
    if (CURR_STATE.fg_scale < FG_SCALE_MAX) {
      CURR_STATE.fg_scale++;
      fpga_spi_sendcmd_u8(CMD_FG_SCALE, CURR_STATE.fg_scale);
      ui_open_mixing();
    }
  }
  if (keypad_keypressed(SCALEUP_KEY)) {
    if (CURR_STATE.fg_scale > 0) {
      CURR_STATE.fg_scale--;
      fpga_spi_sendcmd_u8(CMD_FG_SCALE, CURR_STATE.fg_scale);
      ui_open_mixing();
    }
  }

  if (keypad_keypressed(NSTATE_KEY)) {
    current_mixing_state++;
    current_mixing_state %= NUM_MIXING_STATES;
    fpga_spi_send_state(CURR_STATE);
    ui_open_mixing();
  }
  if (keypad_keypressed(PSTATE_KEY)) {
    current_mixing_state += (NUM_MIXING_STATES-1);
    current_mixing_state %= NUM_MIXING_STATES;
    fpga_spi_send_state(CURR_STATE);
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
