#include "raylib.h"
#include <stdio.h>
#include <stdint.h>

void sl_sleeptimer_delay_millisecond(unsigned int ms) {
    WaitTime(ms/1000.0);
}

void sl_debug_swo_write_u8(uint32_t channel, uint8_t byte) {
    (void) channel;
    putchar(byte);
}

void sl_udelay_wait(unsigned int us) {
    (void) us; // no-op
}
