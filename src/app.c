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

#define CHOICE_SCREEN_WIDTH (LCD_COLUMNS-2)
static char CHOICES[20][CHOICE_SCREEN_WIDTH+1] = {
  "Save state",
  "Load state",
  "Hotkeys",
  "Use image",
  "Transition",
  "Slideshow"
};
static uint32_t NUM_CHOICES = 6;
#define NUM_SCROLLS (NUM_CHOICES - LCD_LINES + 1)

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  board_io_init();
  fpga_spi_init();
  sd_card_init();
  keypad_init();
  lcd_init();

  if (sd_card_is_mounted()) {
    static direntry_t entries[32];
    int entries_read;
    if (sd_card_list_files("/", entries, 32, &entries_read) == 0) {
      for (int i = 0; i < entries_read; i++) {
          debug_println(entries[i].fname);

          // Add file names to the menu
          if (NUM_CHOICES >= sizeof(CHOICES)/sizeof(*CHOICES))
            continue; // Avoid overflowing the buffer
          for (int j = 0; j < CHOICE_SCREEN_WIDTH; j++)
            CHOICES[NUM_CHOICES][j] = entries[i].fname[j];
          NUM_CHOICES++;
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

  static int choice;
  static int scroll;
  static bool menu_dirty = true;
  static int marker_blink_timer;

  if (keypad_keypressed(KEY_DOWN)) {
    choice++;
    if (choice >= scroll + LCD_LINES)
      scroll++;
    menu_dirty = true;
  }
  if (keypad_keypressed(KEY_UP)) {
    choice--;
    if (choice < scroll)
      scroll--;
    menu_dirty = true;
  }
  choice = (choice + NUM_CHOICES) % NUM_CHOICES;
  scroll = (scroll + NUM_SCROLLS) % NUM_SCROLLS;

  if (menu_dirty) {
    // We must redraw the menu on the LCD screen
    menu_dirty = false;
    lcd_clear();
    for (int i = 0; i < LCD_LINES; i++) {
      lcd_set_cursor(1, i);
      lcd_print(CHOICES[scroll+i]);
      marker_blink_timer = 0;
    }
  }

  if (marker_blink_timer % 20 == 0) {
    marker_blink_timer %= 40;
    bool show_marker = marker_blink_timer == 0;
    lcd_set_cursor(0, choice-scroll);
    lcd_print(show_marker ? ">" : " ");
  }
  marker_blink_timer++;

  if (keypad_keypressed(KEY_LEFT))
    board_io_set_led0(false);
  if (keypad_keypressed(KEY_RIGHT)) {
    board_io_set_led0(true);
    debug_print("Pressed right on menu choice: ");
    debug_println(CHOICES[choice]);
  }

  sl_sleeptimer_delay_millisecond(16); // About 60 frames a second
  debug_flush();
}
