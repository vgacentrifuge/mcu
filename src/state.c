#include "state.h"
#include "fpga_spi.h"
#include <string.h>

State mixing_states[NUM_MIXING_STATES];
int current_mixing_state;

State last_sent_state;

#define SYNC_U8(VARIABLE, COMMAND) \
  if(last_sent_state.VARIABLE != CURR_STATE.VARIABLE) \
    fpga_spi_sendcmd_u8(COMMAND, CURR_STATE.VARIABLE);

#define SYNC_U16(VARIABLE, COMMAND) \
  if(last_sent_state.VARIABLE != CURR_STATE.VARIABLE) \
    fpga_spi_sendcmd_u16(COMMAND, CURR_STATE.VARIABLE);

void state_send_changes() {
  SYNC_U8(fg_blend_mode, CMD_FG_MODE);
  SYNC_U8(fg_mode_flag, CMD_FG_MODE_FLAGS);
  SYNC_U8(fg_scale, CMD_FG_SCALE);
  SYNC_U16(fg_x_offset, CMD_FG_OFFSET_X);
  SYNC_U16(fg_y_offset, CMD_FG_OFFSET_Y);
  SYNC_U8(fg_transparency, CMD_FG_TRANSPARENCY);
  SYNC_U16(fg_clipping_left, CMD_FG_CLIP_LEFT);
  SYNC_U16(fg_clipping_right, CMD_FG_CLIP_RIGHT);
  SYNC_U16(fg_clipping_top, CMD_FG_CLIP_TOP);
  SYNC_U16(fg_clipping_bottom, CMD_FG_CLIP_BOTTOM);
  SYNC_U8(fg_image_state, CMD_FG_FREEZE);
  memcpy(&last_sent_state, &CURR_STATE, sizeof(CURR_STATE));
}

void state_send_reset() {
  memset(&last_sent_state, 0, sizeof(last_sent_state));
  fpga_spi_sendcmd(CMD_RESET);
}
