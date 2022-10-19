#include "fpga_spi.h"

#include "sl_spidrv_instances.h"
#include "debug.h"

void fpga_spi_init() {

}

void fpga_spi_send(uint8_t* data, uint8_t len) {
  Ecode_t ret = SPIDRV_MTransmitB(sl_spidrv_fpga_handle, data, len);
  if (ret != ECODE_OK) {
    debug_print("fpga_spi_send() failed: ");
    debug_printintln(ret);
  }
}
