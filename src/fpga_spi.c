#include "fpga_spi.h"
#include "state.h"
#include "sl_spidrv_instances.h"
#include "debug.h"

void fpga_spi_init() {
  fpga_spi_sendcmd(CMD_RESET);
}

void fpga_spi_send_state(State state) {
  fpga_spi_sendcmd_u8(CMD_FG_MODE, state.fg_blend_mode);
  // TODO: Implement on FPGA?
  // fpga_spi_sendcmd_u8(CMD_FG_MODE_FLAGS, 0);
  fpga_spi_sendcmd_u8(CMD_FG_SCALE, state.fg_scale);
  fpga_spi_sendcmd_i16(CMD_FG_OFFSET_X, state.fg_x_offset);
  fpga_spi_sendcmd_i16(CMD_FG_OFFSET_Y, state.fg_y_offset);
  fpga_spi_sendcmd_u8(CMD_FG_TRANSPARENCY, state.fg_transparency);
  fpga_spi_sendcmd_i16(CMD_FG_CLIP_LEFT, state.fg_clipping_left);
  fpga_spi_sendcmd_i16(CMD_FG_CLIP_RIGHT, state.fg_clipping_right);
  fpga_spi_sendcmd_i16(CMD_FG_CLIP_TOP, state.fg_clipping_top);
  fpga_spi_sendcmd_i16(CMD_FG_CLIP_BOTTOM, state.fg_clipping_bottom);
  // TODO: Implement in menu
  // fpga_spi_sendcmd_u8(CMD_FG_FREEZE, 0);
}

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
