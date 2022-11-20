#include "state.h"
#include "fpga_spi.h"
#include "image.h"
#include <stdbool.h>
#include <string.h>
#include "vgacentrifuge_config.h"

State mixing_states[NUM_MIXING_STATES];
int current_mixing_state;

// The way the state looked last time it was sent to the FPGA
static State last_sent_state;

// Handy macros to send commands for data that has changed
#define SYNC_U8(VARIABLE, COMMAND) \
  if(last_sent_state.VARIABLE != CURR_STATE.VARIABLE) \
    fpga_spi_sendcmd_u8(COMMAND, CURR_STATE.VARIABLE);

#define SYNC_U16(VARIABLE, COMMAND) \
  if(last_sent_state.VARIABLE != CURR_STATE.VARIABLE) \
    fpga_spi_sendcmd_u16(COMMAND, CURR_STATE.VARIABLE);

int state_send_changes() {
  int ret = STATE_SEND_OK;

  // Handle image uploading first, in case it turns out the image is broken
  // In which case we want to reset to a safe state before sending state over SPI
  if (CURR_STATE.image_path_hash >= FG_IS_IMAGE_HASH) {
    // We want to be displaying an image
    if (CURR_STATE.image_path_hash != last_sent_state.image_path_hash) {
      // The image is new! No matter the outcome, we reset the clipping
      CURR_STATE.fg_clipping_left = 0;
      CURR_STATE.fg_clipping_right= 0;
      CURR_STATE.fg_clipping_bottom = 0;
      CURR_STATE.fg_clipping_top = 0;

      // Open the image on the SD card to start the upload.
      ret = image_open_for_upload(CURR_STATE.image_path);
      switch(ret) {
        case IMAGE_UPLOAD_ONGOING:
          ret = STATE_SEND_IMAGE_STARTED;
          break;
        case IMAGE_UPLOAD_ERROR:
          // If the image is broken, return to live video input
          CURR_STATE.image_path_hash = FG_IS_LIVE;
          memset(&CURR_STATE.image_path, 0, sizeof(CURR_STATE.image_path));
          ret = STATE_SEND_IMAGE_FAILED;
          break;
      }
    }
  }

  SYNC_U8(fg_blend_mode, CMD_FG_MODE);
  SYNC_U8(fg_mode_flag, CMD_FG_MODE_FLAGS);
  SYNC_U8(fg_scale, CMD_FG_SCALE);
  SYNC_U16(fg_x_offset, CMD_FG_OFFSET_X);
  SYNC_U16(fg_y_offset, CMD_FG_OFFSET_Y);
  SYNC_U8(fg_transparency, CMD_FG_TRANSPARENCY);

  if (CURR_STATE.image_path_hash != last_sent_state.image_path_hash)
    fpga_spi_sendcmd_u8(CMD_FG_FREEZE, CURR_STATE.image_path_hash != FG_IS_LIVE);

  // If we are showing an image, force the clipping to at least cover outside the image
  if(CURR_STATE.image_path_hash >= FG_IS_IMAGE_HASH) {
    uint16_t width, height;
    image_get_size(&width, &height);
    if (CURR_STATE.fg_clipping_right < HORIZONTAL_ACTIVE - width)
      CURR_STATE.fg_clipping_right = HORIZONTAL_ACTIVE - width;
    if (CURR_STATE.fg_clipping_bottom < VERTICAL_ACTIVE - height)
      CURR_STATE.fg_clipping_bottom = VERTICAL_ACTIVE - height;
  }

  SYNC_U16(fg_clipping_left, CMD_FG_CLIP_LEFT);
  SYNC_U16(fg_clipping_top, CMD_FG_CLIP_TOP);
  SYNC_U16(fg_clipping_right, CMD_FG_CLIP_RIGHT);
  SYNC_U16(fg_clipping_bottom, CMD_FG_CLIP_BOTTOM);

  // Copy all data except the image path, since it is hashed either way
  memcpy(&last_sent_state, &CURR_STATE, sizeof(CURR_STATE)-sizeof(CURR_STATE.image_path));

  return ret;
}

void state_send_reset() {
  memset(&last_sent_state, 0, sizeof(last_sent_state));
  fpga_spi_sendcmd(CMD_RESET);
}
