#pragma once

#include <stdint.h>
#include <inttypes.h>

struct State {
  uint16_t width;
  uint16_t height;

  int16_t
      fg_x_offset,
      fg_y_offset;

  char current_menu[20];

  uint8_t fg_scale;
  uint8_t fg_blend_mode;
  uint8_t mode_flag;
  uint8_t fg_frozen;
  uint8_t fg_transparancy;
};

// global state
extern struct State state;
