/**
 * @file
 * @brief Blink examples functions
 * */

#include "sl_simple_led.h"
#include "sl_simple_led_instances.h"
#include "sl_sleeptimer.h"

#ifndef LED_INSTANCE
#define LED_INSTANCE    sl_led_led0
#endif

#ifndef TOOGLE_DELAY_MS
#define TOOGLE_DELAY_MS         50
#endif

sl_sleeptimer_timer_handle_t timer;
bool toggle_timeout = false;

static void on_timeout(sl_sleeptimer_timer_handle_t *handle,
                       void *data);

void blink_init(void)
{
  // Create timer for waking up the system periodically.
  sl_sleeptimer_start_periodic_timer_ms(&timer,
                                        TOOGLE_DELAY_MS,
                                        on_timeout, NULL,
                                        0,
                                        SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG);
}

// Blink ticking function.
void blink_process_action(void)
{
  if (toggle_timeout == true) {
    sl_led_toggle(&LED_INSTANCE);
    toggle_timeout = false;
  }
}

// Sleeptimer timeout callback.
static void on_timeout(sl_sleeptimer_timer_handle_t *handle,
                       void *data)
{
  (void)&handle;
  (void)&data;
  toggle_timeout = true;
}
