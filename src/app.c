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
  board_io_sample();
  keypad_sample();

  ui_update();

  sl_sleeptimer_delay_millisecond(16); // About 60 frames a second
  debug_flush();
}
