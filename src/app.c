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

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  board_io_init();
  lcd_init();
  fpga_spi_init();
  sd_card_init();

  if (sd_card_is_mounted()) {
    static direntry_t entries[32];
    int entries_read;
    if(sd_card_list_files("/", entries, 32, &entries_read) == 0) {
      for(int i = 0; i < entries_read; i++) {
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
  static bool lastPressed = false;
  static uint8_t number = 0;

  bool pressed = false;
  if (board_io_get_button0()) {
      pressed = true;
      if(!lastPressed)
        number++;
  }
  if (board_io_get_button1()) {
        pressed = true;
        if(!lastPressed)
          number--;
    }

  if (pressed && !lastPressed) {
      board_io_set_led0(true);
      debug_println("Trykk!");
      lcd_clear();
      lcd_home();
      char out[20];
      sprintf(out, "Tall: %d", number);
      lcd_print(out);
      fpga_spi_send(&number, 1);

      sl_sleeptimer_delay_millisecond(100);
      board_io_set_led0(false);
  }
  lastPressed = pressed;

  debug_flush();
}
