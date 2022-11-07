#pragma once

#include <stdint.h>
#include <inttypes.h>

#include "lcd.h"

#define CHOICE_SCREEN_WIDTH (LCD_COLUMNS-2)

typedef struct {
  char key[CHOICE_SCREEN_WIDTH];
  int val;
} choice_t;

struct State {
  uint16_t width;
  uint16_t height;

  int16_t
      fg_x_offset,
      fg_y_offset;

  choice_t current_menu;
  bool just_switched;

  uint8_t fg_scale;
  uint8_t fg_blend_mode;
  uint8_t mode_flag;
  uint8_t fg_frozen;
  uint8_t fg_transparancy;
};

// global state
extern struct State state;
