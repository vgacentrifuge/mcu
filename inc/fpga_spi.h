#pragma once

#include <stdint.h>
#include "state.h"

typedef enum {
  CMD_RESET = 0x0,
  CMD_FG_MODE = 0x1,
  CMD_FG_MODE_FLAGS = 0x2,
  CMD_FG_SCALE = 0x3,
  CMD_FG_OFFSET_X = 0x4,
  CMD_FG_OFFSET_Y = 0x5,
  CMD_FG_TRANSPARENCY = 0x6,
  CMD_FG_CLIP_LEFT = 0x7,
  CMD_FG_CLIP_RIGHT = 0x8,
  CMD_FG_CLIP_TOP = 0x9,
  CMD_FG_CLIP_BOTTOM = 0xA,
  CMD_FG_FREEZE = 0xB,
  CMD_FG_IMG_UPLOAD = 0xC,
} cmd_type_t;

void fpga_spi_init();

void fpga_spi_send_state(State state);

void fpga_spi_reset();

void fpga_spi_fg_mode(uint8_t mode);

void fpga_spi_fg_mode_flags(uint8_t flags);

void fpga_spi_fg_scale(uint8_t scale);

void fpga_spi_fg_offset_x(int16_t offset);

void fpga_spi_fg_offset_y(int16_t offset);

void fpga_spi_fg_transparency(uint8_t transparency);

void fpga_spi_fg_clip_left(int16_t clip);

void fpga_spi_fg_clip_right(int16_t clip);

void fpga_spi_fg_clip_top(int16_t clip);

void fpga_spi_fg_clip_bottom(int16_t clip);

void fpga_spi_fg_freeze(uint8_t freeze);

// TODO: Implement
// void fpga_spi_img_upload();

void fpga_spi_sendcmd(cmd_type_t cmd);

void fpga_spi_sendcmd_u8(cmd_type_t cmd, uint8_t arg);

void fpga_spi_sendcmd_i16(cmd_type_t cmd, int16_t arg);

void fpga_spi_send(uint8_t *data, uint8_t len);

void fpga_spi_receive(uint8_t *buffer, uint8_t len);
