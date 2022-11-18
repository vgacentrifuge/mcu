#include "fpga_spi.h"
#include "state.h"
#include "sl_spidrv_instances.h"
#include "debug.h"

void fpga_spi_init() {}

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

void fpga_spi_sendcmd(cmd_type_t cmd) {
  fpga_spi_send(&cmd, 1);
}

void fpga_spi_sendcmd_u8(cmd_type_t cmd, uint8_t arg) {
  uint8_t data[2] = { cmd, arg };
  fpga_spi_send(data, 2);
}

void fpga_spi_sendcmd_u16(cmd_type_t cmd, uint16_t arg) {
  uint8_t arg1 = arg >> 8;
  uint8_t arg2 = (uint8_t) arg;
  uint8_t data[3] = { cmd, arg1, arg2 };
  fpga_spi_send(data, 3);
}
