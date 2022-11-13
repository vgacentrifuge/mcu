/*
 * state.h
 *
 *  Created on: Nov 13, 2022
 *      Author: marhaug
 */

#pragma once

#include <stdint.h>
#include <inttypes.h>

#include "lcd.h"

#define CHOICE_SCREEN_WIDTH (LCD_COLUMNS-2)

// TODO add more state initial values
#define _STATE_INIT(_state) do {\
  _state.fg_scale = 100;        \
  _state.mode = NONE;           \
  _state.fg_x_offset = 100;     \
  _state.fg_y_offset = -100;    \
} while(0)

enum mode {
  CHROMA,
  OVERLAY,
  NONE,
};

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

  uint8_t index;

  uint8_t fg_scale;
  uint8_t fg_blend_mode;
  uint8_t mode_flag;
  uint8_t fg_frozen;
  uint8_t fg_transparancy;

  enum mode mode;
};

// global state
extern struct State state;
