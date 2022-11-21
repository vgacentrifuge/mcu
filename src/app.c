/***************************************************************************//**
 * @file
 * @brief Top level application functions
 ******************************************************************************/

#include "app.h"

#include <stdbool.h>
#include <stdio.h>
#include "sl_sleeptimer.h"
#include "debug.h"
#include "board_io.h"
#include "lcd.h"
#include "fpga_spi.h"
#include "sd_card.h"
#include "keypad.h"
#include "adc_config.h"
#include "ui.h"

/***************************************************************************//**
 * The volatile boolean wait is used to make sure frames have a minimum delay
 ******************************************************************************/
static sl_sleeptimer_timer_handle_t delay_timer;
static volatile bool wait = false;
static void next_frame_callback(sl_sleeptimer_timer_handle_t *handle, void* data) {
  (void) handle;
  (void) data;
  wait = false;
}

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  board_io_init();
  board_io_set_led0(1);
  fpga_spi_init();
  sd_card_init();
  keypad_init();
  adcs_init();
  lcd_init();
  ui_init();
  board_io_set_led0(0);
  debug_flush();

  // Use a periodic timer to set wait=false, allowing next frame to run
  sl_sleeptimer_start_periodic_timer(&delay_timer,
                                     sl_sleeptimer_ms_to_tick(16),
                                     &next_frame_callback,
                                     NULL, 0, 0);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  // Wait until next frame is allowed to start
  while(wait) {};
  wait = true;

  board_io_sample();
  keypad_sample();
  ui_update();
  debug_flush();
}
