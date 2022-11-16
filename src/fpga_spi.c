#include "fpga_spi.h"
#include "state.h"
#include "sl_spidrv_instances.h"
#include "debug.h"

void fpga_spi_init() {
  fpga_spi_sendcmd(CMD_RESET);
}

void fpga_spi_send_state(State state) {
  fpga_spi_fg_mode(state.fg_blend_mode);
  fpga_spi_fg_mode_flags(state.mode_flag);
  fpga_spi_fg_scale(state.fg_scale);
  fpga_spi_fg_offset_x(state.fg_x_offset);
  fpga_spi_fg_offset_y(state.fg_y_offset);
  fpga_spi_fg_transparency(state.fg_transparency);
  fpga_spi_fg_clip_left(state.fg_clipping_left);
  fpga_spi_fg_clip_right(state.fg_clipping_right);
  fpga_spi_fg_clip_top(state.fg_clipping_top);
  fpga_spi_fg_clip_bottom(state.fg_clipping_bottom);
  fpga_spi_fg_freeze(state.fg_image_state);
}

void fpga_spi_reset() {
  fpga_spi_sendcmd(CMD_RESET);
}

void fpga_spi_fg_mode(uint8_t mode) {
  fpga_spi_sendcmd_u8(CMD_FG_MODE, mode);
}

void fpga_spi_fg_mode_flags(uint8_t flags) {
  fpga_spi_sendcmd_u8(CMD_FG_MODE_FLAGS, flags);
}

void fpga_spi_fg_scale(uint8_t scale) {
  fpga_spi_sendcmd_u8(CMD_FG_SCALE, scale);
}

void fpga_spi_fg_offset_x(int16_t offset) {
  fpga_spi_sendcmd_i16(CMD_FG_OFFSET_X, offset);
}

void fpga_spi_fg_offset_y(int16_t offset) {
  fpga_spi_sendcmd_i16(CMD_FG_OFFSET_Y, offset);
}

void fpga_spi_fg_transparency(uint8_t transparency) {
  fpga_spi_sendcmd_u8(CMD_FG_TRANSPARENCY, transparency);
}

void fpga_spi_fg_clip_left(int16_t clip) {
  fpga_spi_sendcmd_i16(CMD_FG_CLIP_LEFT, clip);
}

void fpga_spi_fg_clip_right(int16_t clip) {
  fpga_spi_sendcmd_i16(CMD_FG_CLIP_RIGHT, clip);
}

void fpga_spi_fg_clip_top(int16_t clip) {
  fpga_spi_sendcmd_i16(CMD_FG_CLIP_TOP, clip);
}

void fpga_spi_fg_clip_bottom(int16_t clip) {
  fpga_spi_sendcmd_i16(CMD_FG_CLIP_BOTTOM, clip);
}

void fpga_spi_fg_freeze(uint8_t freeze) {
  fpga_spi_sendcmd_u8(CMD_FG_FREEZE, freeze);
}

// TODO: Implement
// void fpga_spi_img_upload();

void fpga_spi_sendcmd(cmd_type_t cmd) {
  fpga_spi_send(&cmd, 1);
}

void fpga_spi_sendcmd_u8(cmd_type_t cmd, uint8_t arg) {
  uint8_t data[2] = { cmd, arg };
  fpga_spi_send(data, 2);
}

void fpga_spi_sendcmd_i16(cmd_type_t cmd, int16_t arg) {
  uint8_t arg1 = ((uint16_t) arg) >> 8;
  uint8_t arg2 = (uint8_t) arg;
  uint8_t data[3] = { cmd, arg1, arg2 };
  fpga_spi_send(data, 3);
}

void fpga_spi_send(uint8_t *data, uint8_t len) {
  Ecode_t ret = SPIDRV_MTransmitB(sl_spidrv_fpga_handle, data, len);
  if (ret != ECODE_OK) {
    debug_print("fpga_spi_send() failed: ");
    debug_printintln(ret);
  }
}

void fpga_spi_receive(uint8_t *buffer, uint8_t len) {
  Ecode_t ret = SPIDRV_MReceiveB(sl_spidrv_fpga_handle, buffer, len);
  if (ret != ECODE_OK) {
    debug_print("fpga_spi_receive() failed: ");
    debug_printintln(ret);
  }
}
