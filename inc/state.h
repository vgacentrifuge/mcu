/*
 * state.h
 *
 *  Created on: Nov 13, 2022
 *      Author: marhaug
 */

#pragma once

#include <stdint.h>
#include <inttypes.h>

typedef struct {
  int16_t
    fg_x_offset,
    fg_y_offset;

  uint8_t fg_scale;
  uint8_t fg_blend_mode;
  uint8_t mode_flag;
  uint8_t fg_frozen;
  uint8_t fg_transparancy;
} State;

enum fg_scale {
  FG_SCALE_100 = 0,
  FG_SCALE_50 = 1,
  FG_SCALE_25 = 2,
  FG_SCALE_MAX = 2
};

enum fg_blend_mode {
  FG_BLEND_NONE = 0,
  FG_BLEND_NORMAL = 1,
  FG_BLEND_CHROMA = 2,
};

enum fg_frozen {
  FG_NOT_FROZEN = 0,
  FG_FROZEN = 1,
};

enum fg_transparancy {
  FG_TRANSPARANCY_0 = 0,
  FG_TRANSPARANCY_25 = 1,
  FG_TRANSPARANCY_50 = 2,
  FG_TRANSPARANCY_MAX = 2
};

#define INITIAL_STATE (State){\
  .fg_x_offset = 0,           \
  .fg_y_offset = 0,           \
  .fg_scale = FG_SCALE_100,   \
  .fg_blend_mode = FG_BLEND_NORMAL, \
  .mode_flag = 0,             \
  .fg_frozen = FG_NOT_FROZEN, \
  .fg_transparancy = FG_TRANSPARANCY_0, \
}
