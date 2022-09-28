/*
 * sd_card.c
 *
 *  Created on: 28. sep. 2022
 *      Author: amatho
 */

#include "sd_card.h"
#include "spidrv.h"
#include "sl_spidrv_instances.h"
#include "sl_sleeptimer.h"
#include "debug.h"

void sd_card_init() {
    sl_sleeptimer_delay_millisecond(1);

    uint8_t txBuffer[8] = { CMD0, 0, 0, 0, 0, 0x1, 0, 0 };
    uint8_t rxBuffer[8];
    SPIDRV_MTransferB(sl_spidrv_sd_card_handle, txBuffer, rxBuffer, 8);

    debug_send_string("contents of response: ");
    for (int i = 0; i < 8; i++) {
        char str[4];
        sprintf(str, "%d", rxBuffer[i]);
        debug_send_string(str);
        debug_send_string(", ");
    }
    debug_send_string("\n");
}

void sd_card_test() {

}
