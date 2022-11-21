#include "fpga_spi.h"
#include "state.h"
#include "sl_spidrv_fpga_config.h"
#include "sl_spidrv_instances.h"
#include "debug.h"
#include "em_gpio.h"

// Remember that SPI CS is active low
#define CS_HIGH() GPIO_PinOutSet(SL_SPIDRV_FPGA_CS_PORT, SL_SPIDRV_FPGA_CS_PIN)
#define CS_LOW() GPIO_PinOutClear(SL_SPIDRV_FPGA_CS_PORT, SL_SPIDRV_FPGA_CS_PIN)

void fpga_spi_init() {
  GPIO_PinModeSet(SL_SPIDRV_FPGA_CS_PORT, SL_SPIDRV_FPGA_CS_PIN, gpioModePushPull, 1);
}

void fpga_spi_send(uint8_t *data, int len) {
  CS_LOW();
  // In case the message is longer than the DMA allows
  for(int i = 0; i < len; i+= DMADRV_MAX_XFER_COUNT) {
    int send_len = len-i;
    if (send_len > DMADRV_MAX_XFER_COUNT)
      send_len = DMADRV_MAX_XFER_COUNT;
    Ecode_t ret = SPIDRV_MTransmitB(sl_spidrv_fpga_handle, data+i, send_len);
    if (ret != ECODE_OK) {
      debug_print("fpga_spi_send() failed: ");
      debug_printintln(ret);
    }
  }
  CS_HIGH();
}

void fpga_spi_receive(uint8_t *buffer, int len) {
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
  uint8_t arg1 = (arg >> 8) & 0xFF;
  uint8_t arg2 = arg & 0xFF;
  uint8_t data[3] = { cmd, arg1, arg2 };
  fpga_spi_send(data, 3);
}

void fpga_spi_sendimageline(uint8_t *data, uint16_t width) {
  data[0] = CMD_FG_IMG_UPLOAD;
  fpga_spi_send(data, 3+width*2);
}
