#pragma once

#include <stdint.h>
#include <inttypes.h>

#define STATE_IMAGE_PATH_LEN 128

typedef struct {
  uint8_t fg_blend_mode;
  uint8_t fg_mode_flag;
  uint8_t fg_scale;
  int16_t fg_x_offset;
  int16_t fg_y_offset;
  uint8_t fg_transparency;
  uint16_t fg_clipping_left;
  uint16_t fg_clipping_right;
  uint16_t fg_clipping_top;
  uint16_t fg_clipping_bottom;
  uint16_t chroma_key_cfg;

  // FG_LIVE means playing live, FG_FROZEN means frozen
  // Any other value is a 16 bit hash of the image_path
  // We guarantee that no file path has 0 or 1 as hash
  uint16_t image_path_hash;

  // The name of the image shown instead of the foreground
  // Last in the struct, to allow skipping it during memcpy
  char image_path[STATE_IMAGE_PATH_LEN+1];
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

// We have 3 bits, with 7 meaning the foreground is 12.5% visible
#define FG_TRANSPARANCY_MAX 7

// If no image is currently being displayed, image_path_hash instead is one of these
enum fg_image_state {
  FG_IS_LIVE = 0,
  FG_IS_FROZEN = 1,
  FG_IS_IMAGE_HASH = 2, //And every value above it
};

#define CHROMA_KEY_CFG_DEFAULT 0b0010010110001100

#define INITIAL_STATE (State){        \
    .fg_blend_mode = FG_BLEND_NONE,   \
    .fg_mode_flag = 0,                \
    .fg_scale = FG_SCALE_100,         \
    .fg_x_offset = 0,                 \
    .fg_y_offset = 0,                 \
    .fg_transparency = 0,             \
    .fg_clipping_left = 0,            \
    .fg_clipping_right = 0,           \
    .fg_clipping_top= 0,              \
    .fg_clipping_bottom = 0,          \
    .chroma_key_cfg = CHROMA_KEY_CFG_DEFAULT, \
    .image_path_hash = FG_IS_LIVE,    \
    .image_path[0 ... STATE_IMAGE_PATH_LEN] = 0 \
}
#define STATE_RESET_CLIPPING(State) do{    \
    (State).fg_clipping_left = 0;     \
    (State).fg_clipping_right = 0;    \
    (State).fg_clipping_top = 0;      \
    (State).fg_clipping_bottom = 0;   \
    } while(false)


#define NUM_MIXING_STATES 10
extern State mixing_states[NUM_MIXING_STATES];
extern int current_mixing_state;
#define CURR_STATE mixing_states[current_mixing_state]

enum {
  STATE_SEND_OK = 0,
  STATE_SEND_IMAGE_STARTED = 1,
  STATE_SEND_IMAGE_FAILED = 2
};

// Sends any changes made to the CURR_STATE struct since last call.
// Will return STATE_SEND_IMAGE_STARTED if a new image is supposed to be uploaded
// using image_upload_next_line() until finished.
// If opening the image failed, it will send STATE_SEND_IMAGE_FAILED,
// and also set CURR_STATE to not show images
int state_send_changes();
void state_send_reset();
