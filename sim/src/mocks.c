#include "raylib.h"
#include <stdio.h>
#include <stdint.h>

void sl_sleeptimer_delay_millisecond(unsigned int ms) {
    WaitTime(ms/1000.0);
}

uint64_t sl_sleeptimer_get_tick_count64(void) {
    return (uint64_t)(GetTime()*1000);
}

uint32_t sl_sleeptimer_ms_to_tick(uint16_t time_ms) {
    return time_ms;
}

void sl_debug_swo_write_u8(uint32_t channel, uint8_t byte) {
    (void) channel;
    putchar(byte);
}

void sl_udelay_wait(unsigned int us) {
    (void) us; // no-op
}
