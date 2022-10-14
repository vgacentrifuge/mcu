/***************************************************************************//**
 * @file
 * @brief Top level application functions
 ******************************************************************************/

#include "app.h"

#include <stdbool.h>
#include "sl_sleeptimer.h"
#include "debug.h"
#include "board_io.h"
#include "lcd.h"
#include "em_gpio.h"

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  board_io_init();
  lcd_init();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  static bool lastPressed = false;
  bool pressed = board_io_get_button0();
  if (pressed && !lastPressed) {
      board_io_set_led0(true);
      debug_println("Hei!");
      lcd_print("Sykt!");
      sl_sleeptimer_delay_millisecond(1000);
      board_io_set_led0(false);
      lcd_clear();
  }
  lastPressed = pressed;
}
