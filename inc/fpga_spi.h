#pragma once

#include <stdint.h>

void fpga_spi_init();

void fgpa_spi_sendcmd(uint8_t cmd, uint8_t arg);

void fpga_spi_sendbyte(uint8_t data);

void fpga_spi_send(uint8_t *data, uint8_t len);
