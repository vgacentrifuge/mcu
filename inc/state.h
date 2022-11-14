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
  uint8_t fg_blend_mode;
  uint8_t mode_flag;
  uint8_t fg_scale;
  int16_t fg_x_offset;
  int16_t fg_y_offset;
  uint8_t fg_transparency;
  uint16_t fg_clipping_left;
  uint16_t fg_clipping_right;
  uint16_t fg_clipping_top;
  uint16_t fg_clipping_bottom;
  uint8_t fg_image_state;
} State;

enum fg_blend_mode {
  FG_BLEND_NONE = 0,
  FG_BLEND_OVERLAY = 1,
  FG_BLEND_CHROMA = 2,
};

enum fg_scale {
  FG_SCALE_100 = 0,
  FG_SCALE_50 = 1,
  FG_SCALE_25 = 2,
  FG_SCALE_MAX = 2
};

// Transparency 0 means fully opaque
enum fg_transparency {
  FG_TRANSPARENCY_0 = 0,
  FG_TRANSPARENCY_25 = 1,
  FG_TRANSPARENCY_50 = 2,
  FG_TRANSPARENCY_75 = 3,
  FG_TRANSPARENCY_MAX = 3
};

enum fg_frozen {
  FG_NOT_FROZEN = 0,
  FG_FROZEN = 1
};

enum fg_image_mode {
  FG_IS_LIVE=0,
  FG_IS_FROZEN=1,
  FG_IS_IMAGE=2
};

#define INITIAL_STATE (State){        \
    .fg_blend_mode = FG_BLEND_NONE,   \
    .mode_flag = 0,                   \
    .fg_scale = FG_SCALE_100,         \
    .fg_x_offset = 0,                 \
    .fg_y_offset = 0,                 \
    .fg_transparency = FG_TRANSPARENCY_0, \
    .fg_clipping_left = 0,            \
    .fg_clipping_right = 0,           \
    .fg_clipping_top= 0,              \
    .fg_clipping_bottom = 0,          \
    .fg_image_state = FG_IS_LIVE \
}
