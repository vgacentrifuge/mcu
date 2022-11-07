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
#include "state.h"

#define HOME       0
#define SAVE_STATE 1
#define LOAD_STATE 2
#define HOT_KEYS   3
#define USE_IMAGE  4
#define TRANSITION 5
#define SLIDESHOW  6

#define CHOICE_SCREEN_WIDTH (LCD_COLUMNS-2)
#define NUM_CHOICES 7

static choice_t MENUS[NUM_CHOICES] = {
    {"Home", HOME},
    {"Save state", SAVE_STATE},
    {"Load state", LOAD_STATE},
    {"Hotkeys", HOT_KEYS},
    {"Use image", USE_IMAGE},
    {"Transition", TRANSITION},
    {"Slideshow", SLIDESHOW},
};

#define HOTKEY_CHOICES 2

static choice_t HOTKEYS[HOTKEY_CHOICES] = {
    {"HOTKEY 1", 0},
    {"HOTKEY 2", 1},
};

#define NUM_SCROLLS(_num_choices) ((_num_choices) - LCD_LINES + 1)



void func_not_implemented(void);
void return_home(void);
int handle_scroll(int num_choices, choice_t choices[], void(*handle_left)(void));



/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
// global state
struct State state;

void app_init(void)
{
  board_io_init();
  fpga_spi_init();
  sd_card_init();
  keypad_init();
  lcd_init();

  state.width = 800;
  state.height = 600;
  state.current_menu = MENUS[0];



  debug_flush();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  board_io_sample();
  keypad_sample();

  switch (state.current_menu.val)
  {
    case HOME:        home_menu();       break;
    case SAVE_STATE:  save_state();      break;
    case LOAD_STATE:  load_state();      break;
    case HOT_KEYS:    hotkeys_menu();    break;
    case USE_IMAGE:   use_image_menu();  break;
    case TRANSITION:  transition_menu(); break;
    case SLIDESHOW:   slideshow_menu();  break;
  }

  sl_sleeptimer_delay_millisecond(16); // About 60 frames a second
  debug_flush();
}

void home_menu(void)
{
    int choice = handle_scroll(NUM_CHOICES, MENUS, return_home);

    if (choice >= 0) {
        board_io_set_led0(true);
        debug_print("Pressed right on menu choice: ");
        debug_println(MENUS[choice].key);
        state.current_menu = MENUS[choice];
        state.just_switched = true;
    }
}

void save_state(void)
{
    func_not_implemented();
}

void load_state(void)
{
    //int file_val = 8;

    if (sd_card_is_mounted()) {
      static direntry_t entries[32];
      int entries_read;
      if (sd_card_list_files("/", entries, 32, &entries_read) == 0) {
          for (int i = 0; i < entries_read; i++) {
              debug_println(entries[i].fname);


          }
      }
    }
    func_not_implemented();
}



void hotkeys_menu(void)
{

    int choice = handle_scroll(HOTKEY_CHOICES, HOTKEYS, return_home);
    if (choice >= 0) {
        debug_print("Hotkey selected: ");
        debug_println(HOTKEYS[choice].key);
    }
}

void use_image_menu(void)
{
    func_not_implemented();
}

void transition_menu(void)
{
    func_not_implemented();
}

void slideshow_menu(void)
{
    func_not_implemented();
}

void func_not_implemented(void)
{
    debug_println("Function not yet implemented");
    state.current_menu = MENUS[0];
    app_process_action();
}

void return_home(void)
{
    lcd_clear();
    debug_println("Returned home");
    state.current_menu = MENUS[0];
    state.just_switched = true;

    for (int i = 0; i < LCD_LINES; i++) {
        lcd_set_cursor(1, i);
        lcd_print(MENUS[i].key);
    }
    app_process_action();
}

int handle_scroll(int num_choices, choice_t choices[], void (*handle_left)(void))
{
    static int choice;
    static int scroll;
    static bool menu_dirty = true;
    static int marker_blink_timer;

    if (state.just_switched) {
       state.just_switched = false;
       lcd_clear();
       lcd_set_cursor(0,0);
       lcd_print(">");
       choice = 0;
       scroll = 0;
       for (int i = 0; i < LCD_LINES; i++)
       {
           lcd_set_cursor(1, i);
           lcd_print(choices[i].key);
       }
    }

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
    choice = (choice + num_choices) % num_choices;
    scroll = (scroll + NUM_SCROLLS(num_choices)) % NUM_SCROLLS(num_choices);

    if (menu_dirty) {
       // We must redraw the menu on the LCD screen
       menu_dirty = false;
       lcd_clear();

       for (int i = 0; i < LCD_LINES; i++) {
           lcd_set_cursor(1, i);
           lcd_print(choices[scroll+i].key);
           marker_blink_timer = 0;
       }
    }

    if (marker_blink_timer % 20 == 0) {
        marker_blink_timer %= 40;
        bool show_marker = marker_blink_timer == 0;
        lcd_set_cursor(0, choice - scroll);
        lcd_print(show_marker ? ">" : " ");
    }
    marker_blink_timer++;

    if (keypad_keypressed(KEY_LEFT)) {
        handle_left();
    }

    if (keypad_keypressed(KEY_RIGHT)) {
        return choice;
    }
    return -1;
}

