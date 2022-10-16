#pragma once

#include <stdint.h>

void fpga_spi_init();

void fpga_spi_send(uint8_t* data, uint8_t len);
