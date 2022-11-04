/***************************************************************************//**
 * @file
 * @brief Top level application functions
 ******************************************************************************/

#include "app.h"

#include <stdbool.h>
#include <stdio.h>
#include "sl_sleeptimer.h"
#include "sl_udelay.h"
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
  board_io_set_led0(0);

  if (sd_card_is_mounted()) {
    static direntry_t entries[32];
    int entries_read;
    if (sd_card_list_files("/", entries, 32, &entries_read) == 0) {
      for (int i = 0; i < entries_read; i++) {
        debug_println(entries[i].fname);
      }
    }
  }

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
