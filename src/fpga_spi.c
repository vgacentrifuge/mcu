#include "fpga_spi.h"

#include "sl_spidrv_instances.h"
#include "debug.h"

void fpga_spi_init() {

}

void fpga_spi_send(uint8_t* data, uint8_t len) {
  Ecode_t ret = SPIDRV_MTransmitB(sl_spidrv_usart_fpga_handle, data, len);
  if (ret != ECODE_OK) {
    char out[30];
    snprintf(out, sizeof(out), "fpga_spi_send() failed: %d\n", ret);
    debug_print(out);
  }
}
