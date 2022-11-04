#include "fpga_spi.h"
#include "sl_spidrv_instances.h"
#include "debug.h"

#define CMD_PREFIX (0x40)
#define CMD_LEN 2
#define CMD_RESET 0

static uint8_t RECV_BUFFER[2048];

void fpga_spi_init() {
  fgpa_spi_sendcmd(CMD_RESET, 0);
}

void fgpa_spi_sendcmd(uint8_t cmd, uint8_t arg) {
  uint8_t data[CMD_LEN] = { CMD_PREFIX | cmd, arg };
  fpga_spi_send(data, CMD_LEN);
}

void fpga_spi_sendbyte(uint8_t data) {
  fpga_spi_send(&data, 1);
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
