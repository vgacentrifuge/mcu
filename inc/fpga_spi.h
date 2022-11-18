#pragma once

#include <stdint.h>
#include "state.h"

enum {
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
};
typedef uint8_t cmd_type_t;

void fpga_spi_init();

void fpga_spi_sendcmd(cmd_type_t cmd);

void fpga_spi_sendcmd_u8(cmd_type_t cmd, uint8_t arg);

void fpga_spi_sendcmd_u16(cmd_type_t cmd, int16_t arg);

void fpga_spi_receive(uint8_t *buffer, uint8_t len);
