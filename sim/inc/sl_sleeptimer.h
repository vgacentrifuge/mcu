#pragma once
#include <stdint.h>

// Implemented in mocks.c
void sl_sleeptimer_delay_millisecond(unsigned int);

uint64_t sl_sleeptimer_get_tick_count64(void);

uint32_t sl_sleeptimer_ms_to_tick(uint16_t time_ms);
