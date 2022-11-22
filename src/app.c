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
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  // When is the next frame allowed to start
  static uint64_t next_frame_tick = 0;
  // Wait until next frame is allowed to start
  while(sl_sleeptimer_get_tick_count64() < next_frame_tick) {};
  next_frame_tick = sl_sleeptimer_get_tick_count64() + sl_sleeptimer_ms_to_tick(16);

  board_io_sample();
  keypad_sample();
  ui_update();
  debug_flush();
}
