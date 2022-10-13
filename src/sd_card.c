/*
 * sd_card.c
 *
 *  Created on: 28. sep. 2022
 *      Author: amatho
 */

#include "sd_card.h"
#include "spidrv.h"
#include "sl_spidrv_instances.h"
#include "sl_spidrv_sd_card_config.h"
#include "sl_sleeptimer.h"
#include "debug.h"
#include "em_gpio.h"
#include "efm32gg990f1024.h"

#define PORT_D 3
#define CMD0 0x40 // Reset
#define CMD1 0x41 // Init
#define CMD0_CRC 0x95
#define spi_handle sl_spidrv_sd_card_handle
#define CS_LOW() GPIO_PinOutSet(SL_SPIDRV_SD_CARD_CS_PORT, SL_SPIDRV_SD_CARD_CS_PIN)
#define CS_HIGH() GPIO_PinOutClear(SL_SPIDRV_SD_CARD_CS_PORT, SL_SPIDRV_SD_CARD_CS_PIN)

const uint8_t dummy = 0xFF;

int spi_xchg(uint8_t tx, uint8_t *rx) {
    if (SPIDRV_MTransferB(spi_handle, &tx, rx, 1) == ECODE_EMDRV_SPIDRV_OK) {
        return 0;
    }
    *rx = 0;
    return -1;
}

void wait_ready() {
    uint8_t data;
    do {
        spi_xchg(0xFF, &data);
    } while (data != 0xFF);
}

void deselect() {
    uint8_t data;
    CS_HIGH();
    spi_xchg(0xFF, &data);
}

void select() {
    uint8_t data;
    CS_LOW();
    spi_xchg(0xFF, &data);
    wait_ready();
}

void sd_card_init() {
    sl_sleeptimer_delay_millisecond(1);
    SPIDRV_SetBitrate(spi_handle, 200000);
    GPIO_PinOutSet(PORT_D, 0);
    sl_sleeptimer_delay_millisecond(1);

    uint8_t txBuffer[16] = { CMD0, 0, 0, 0, 0, CMD0_CRC, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    uint8_t rxBuffer[16] = { 0 };
    if (SPIDRV_MTransferB(handle, txBuffer, rxBuffer, 16) == ECODE_EMDRV_SPIDRV_OK) {
        debug_send_string("send OK\n");
    } else {
        debug_send_string("send not ok\n");
    }

    debug_send_string("contents of response: ");
    for (int i = 0; i < 16; i++) {
        char str[6];
        sprintf(str, "0x%x ", rxBuffer[i]);
        debug_send_string(str);
    }
    debug_send_string("\n");
}

void sd_card_test() {

}
