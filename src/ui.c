#include "ui.h"
#include "ddc_data.h"
#include "debug.h"
#include "image.h"
#include "keypad.h"
#include "lcd.h"
#include "state.h"
#include "sd_card.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

enum {
  UI_TRANSITION,    // Lets the lcd freeze before switching mode
  UI_MIXING,        // The default mixing view
  UI_OPTIONS,       // The menu
  UI_CLIPPING_MENU, // The clipping menu
  UI_CHROMA_KEY_CFG,  // Chroma key config menu
  UI_SOURCE_SELECT, // Picking source from a list
  UI_IMAGE_UPLOAD,  // Uploading an image
  UI_STATE_SELECT,  // Picking a state from a list
};

static int ui_state;

static void ui_open_transition(int frames, void (*callback)());
static void ui_open_mixing();
static void ui_open_options();
static void ui_open_clipping();
static void ui_open_chroma_key_cfg();
static void source_select_live_input();
static void ui_open_source_select();
static void ui_open_image_upload();
static void ui_open_state_select();

enum {
  PLAY_SYMBOL_INDEX = 0,
  PAUSE_SYMBOL_INDEX = 1,
  IMG_SYMBOL_INDEX = 2,

  TRANS0_SYMBOL_INDEX = 3,
  TRANS25_SYMBOL_INDEX = 4,
  TRANS50_SYMBOL_INDEX = 5,
  TRANS75_SYMBOL_INDEX = 6,
};
static const uint8_t PLAY_SYMBOL[8] = {0b00000, 0b10000, 0b11100, 0b11111,
                                       0b11111, 0b11100, 0b10000, 0b00000};
static const uint8_t PAUSE_SYMBOL[8] = {0b11011, 0b11011, 0b11011, 0b11011,
                                        0b11011, 0b11011, 0b11011, 0b11011};
static const uint8_t IMG_SYMBOL[8] = {0b00000, 0b11111, 0b11001, 0b10001,
                                      0b10111, 0b11111, 0b11111, 0b11111};

static const uint8_t TRANS0_SYMBOL[8] = {0b11111, 0b11111, 0b11111, 0b11111,
                                         0b11111, 0b11111, 0b11111, 0b11111};
static const uint8_t TRANS25_SYMBOL[8] = {0b11111, 0b10101, 0b11111, 0b01010,
                                          0b11111, 0b10101, 0b11111, 0b01010};
static const uint8_t TRANS50_SYMBOL[8] = {0b01010, 0b10101, 0b01010, 0b10101,
                                          0b01010, 0b10101, 0b01010, 0b10101};
static const uint8_t TRANS75_SYMBOL[8] = {0b00000, 0b01010, 0b00000, 0b10101,
                                          0b00000, 0b01010, 0b00000, 0b10101};

// The different clipping menu states
enum {
  CLIP_MENU_LEFT,
  CLIP_MENU_RIGHT,
  CLIP_MENU_TOP,
  CLIP_MENU_BOTTOM,
};

// Chroma key config states
enum {
  CHROMA_KEY_CFG_RED = 0,
  CHROMA_KEY_CFG_GREEN = 1,
  CHROMA_KEY_CFG_BLUE = 2,
};
#define NUM_CHROMA_KEY_CFG_STATES 3

void ui_init(void) {
  lcd_custom_symbol(PLAY_SYMBOL_INDEX, PLAY_SYMBOL);
  lcd_custom_symbol(PAUSE_SYMBOL_INDEX, PAUSE_SYMBOL);
  lcd_custom_symbol(IMG_SYMBOL_INDEX, IMG_SYMBOL);

  lcd_custom_symbol(TRANS0_SYMBOL_INDEX, TRANS0_SYMBOL);
  lcd_custom_symbol(TRANS25_SYMBOL_INDEX, TRANS25_SYMBOL);
  lcd_custom_symbol(TRANS50_SYMBOL_INDEX, TRANS50_SYMBOL);
  lcd_custom_symbol(TRANS75_SYMBOL_INDEX, TRANS75_SYMBOL);

  for (uint8_t i = 0; i < NUM_MIXING_STATES; i++)
    mixing_states[i] = INITIAL_STATE;

  current_mixing_state = 0;

  // We immediately transition into the mixing state
  ui_open_transition(0, &ui_open_mixing);
}

// Call once the state has been changed to handle it.
// Will by default open the mixing mode afterwards.
// Unless an image upload is starting / failed to start,
// or we are in the clipping menu.
void ui_handle_state_changes() {
  int res = state_send_changes();
  switch (res) {
  case STATE_SEND_OK:
    if (ui_state == UI_CLIPPING_MENU) {
      ui_open_clipping();
    } else if (ui_state == UI_CHROMA_KEY_CFG) {
      ui_open_chroma_key_cfg();
    } else {
      ui_open_mixing();
    }
    break;
  case STATE_SEND_IMAGE_STARTED:
    ui_open_image_upload();
    break;
  case STATE_SEND_IMAGE_FAILED:
    lcd_clear();
    lcd_print("> IMAGE ERROR! <");
    ui_open_transition(60, &ui_open_mixing);
    break;
  }
}

static int ui_transition_frames_left;
static void (*ui_transition_callback)();
static void ui_open_transition(int frames, void (*callback)()) {
  ui_state = UI_TRANSITION;
  ui_transition_frames_left = frames;
  ui_transition_callback = callback;
}

static void ui_update_transition() {
  if (ui_transition_frames_left-- <= 0)
    ui_transition_callback();
}

static void ui_open_mixing() {
  ui_state = UI_MIXING;
  char buff[LCD_COLUMNS + 1];

  lcd_clear();
  lcd_set_cursor(0, 0);
  switch (CURR_STATE.fg_blend_mode) {
  case FG_BLEND_NONE:
    lcd_print("NONE  ");
    break;
  case FG_BLEND_OVERLAY:
    lcd_print("OVRLAY");
    break;
  case FG_BLEND_CHROMA:
    lcd_print("CHROMA");
    break;
  }

  lcd_set_cursor(0, 1);
  lcd_write('0' + current_mixing_state);
  switch (CURR_STATE.image_path_hash) {
  case FG_IS_LIVE:
    lcd_write(PLAY_SYMBOL_INDEX);
    break;
  case FG_IS_FROZEN:
    lcd_write(PAUSE_SYMBOL_INDEX);
    break;
  default:
    lcd_write(IMG_SYMBOL_INDEX);
    break;
  }

  const uint8_t TRANS_SYMBOLS[] = {TRANS0_SYMBOL_INDEX, TRANS25_SYMBOL_INDEX,
                                   TRANS50_SYMBOL_INDEX, TRANS75_SYMBOL_INDEX,
                                   ' '};
  lcd_set_cursor(7, 0);
  lcd_write(TRANS_SYMBOLS[CURR_STATE.fg_transparency / 2]);
  lcd_write(TRANS_SYMBOLS[(CURR_STATE.fg_transparency + 1) / 2]);

  lcd_set_cursor(3, 1);
  switch (CURR_STATE.fg_scale) {
  case FG_SCALE_100:
    lcd_print("SC100%");
    break;
  case FG_SCALE_50:
    lcd_print("SC 50%");
    break;
  case FG_SCALE_25:
    lcd_print("SC 25%");
    break;
  }

  lcd_set_cursor(10, 0);
  snprintf(buff, sizeof(buff), "X%+05d", CURR_STATE.fg_x_offset);
  lcd_print(buff);
  lcd_set_cursor(10, 1);
  snprintf(buff, sizeof(buff), "Y%+05d", CURR_STATE.fg_y_offset);
  lcd_print(buff);
}

#define MAX_X_VAL 800
#define MIN_X_VAL -800
#define MAX_Y_VAL 600
#define MIN_Y_VAL -600

#define REPEAT_DELAY 15
#define REPEAT_PERIOD 2
#define KEY_DOWN_OR_REPEAT(KEY)                                                \
  (keypad_keydown(KEY, &was_down_frames) &&                                    \
   ((was_down_frames > REPEAT_DELAY &&                                         \
     (was_down_frames - REPEAT_DELAY) % REPEAT_PERIOD == 0) ||                 \
    was_down_frames == 1))
// How many frames a mode key must be held to enable source select
#define MODE_HOLD_THRESHOLD 30

static void ui_update_mixing() {
  bool dirty = false;
  int was_down_frames;
  if (KEY_DOWN_OR_REPEAT(KEY_DOWN)) {
    if (CURR_STATE.fg_y_offset < MAX_Y_VAL) {
      CURR_STATE.fg_y_offset++;
      dirty = true;
    }
  }
  if (KEY_DOWN_OR_REPEAT(KEY_UP)) {
    if (CURR_STATE.fg_y_offset > MIN_Y_VAL) {
      CURR_STATE.fg_y_offset--;
      dirty = true;
    }
  }
  if (KEY_DOWN_OR_REPEAT(KEY_LEFT)) {
    if (CURR_STATE.fg_x_offset > MIN_X_VAL) {
      CURR_STATE.fg_x_offset--;
      dirty = true;
    }
  }
  if (KEY_DOWN_OR_REPEAT(KEY_RIGHT)) {
    if (CURR_STATE.fg_x_offset < MAX_X_VAL) {
      CURR_STATE.fg_x_offset++;
      dirty = true;
    }
  }
  if (keypad_keypressed(KEY_RESET_OFFSET)) {
    CURR_STATE.fg_x_offset = 0;
    CURR_STATE.fg_y_offset = 0;
    dirty = true;
  }

  if (keypad_keypressed(KEY_MENU)) {
    ui_open_options();
  }

  if (keypad_keyreleased(KEY_CHROMA, NULL)) {
    CURR_STATE.fg_blend_mode = FG_BLEND_CHROMA;
    dirty = true;
  }
  if (keypad_keydown(KEY_CHROMA, &was_down_frames) &&
      was_down_frames >= MODE_HOLD_THRESHOLD) {
    CURR_STATE.fg_blend_mode = FG_BLEND_CHROMA;
    ui_open_source_select();
    return;
  }
  if (keypad_keyreleased(KEY_OVERLAY, NULL)) {
    CURR_STATE.fg_blend_mode = FG_BLEND_OVERLAY;
    dirty = true;
  }
  if (keypad_keydown(KEY_OVERLAY, &was_down_frames) &&
      was_down_frames >= MODE_HOLD_THRESHOLD) {
    CURR_STATE.fg_blend_mode = FG_BLEND_OVERLAY;
    ui_open_source_select();
    return;
  }
  if (keypad_keypressed(KEY_NONE)) {
    CURR_STATE.fg_blend_mode = FG_BLEND_NONE;
    dirty = true;
  }

  if (keypad_keypressed(KEY_RESET_ALL)) {
    state_send_reset();
    CURR_STATE = INITIAL_STATE;
    dirty = true;
  }

  if (keypad_keypressed(KEY_TRANSPLUS)) {
    if (CURR_STATE.fg_transparency < FG_TRANSPARANCY_MAX) {
      CURR_STATE.fg_transparency++;
      dirty = true;
    }
  }
  if (keypad_keypressed(KEY_TRANSMINUS)) {
    if (CURR_STATE.fg_transparency > 0) {
      CURR_STATE.fg_transparency--;
      dirty = true;
    }
  }

  if (keypad_keypressed(KEY_SCALEDOWN)) {
    if (CURR_STATE.fg_scale < FG_SCALE_MAX) {
      CURR_STATE.fg_scale++;
      dirty = true;
    }
  }
  if (keypad_keypressed(KEY_SCALEUP)) {
    if (CURR_STATE.fg_scale > 0) {
      CURR_STATE.fg_scale--;
      dirty = true;
    }
  }

  if (keypad_keyreleased(KEY_NSTATE, NULL)) {
    current_mixing_state++;
    current_mixing_state %= NUM_MIXING_STATES;
    dirty = true;
  }
  if (keypad_keyreleased(KEY_PSTATE, NULL)) {
    current_mixing_state += (NUM_MIXING_STATES - 1);
    current_mixing_state %= NUM_MIXING_STATES;
    dirty = true;
  }
  if ((keypad_keydown(KEY_NSTATE, &was_down_frames)
       && was_down_frames >= MODE_HOLD_THRESHOLD)
   || (keypad_keydown(KEY_PSTATE, &was_down_frames)
       && was_down_frames >= MODE_HOLD_THRESHOLD)) {
    ui_open_state_select();
    return;
  }

  if (dirty)
    ui_handle_state_changes();
}

#define MAX_MENU_ENTRIES 100
static char *menu_title;
static char *menu_entries[MAX_MENU_ENTRIES];
static int menu_entry_count;
static int menu_selected_entry;

// Starts a menu titled with title
// The pointer must continue to be valid for the lifetime of the menu
void ui_menuing_begin(char *title) {
  menu_title = title;
  menu_entry_count = 0;
  menu_selected_entry = 0;
  lcd_clear();
  lcd_print(title);
  lcd_set_cursor(0, 1);
  lcd_write('>');
}

// Adds an option to the generic menu
// The option text pointer must stay valid for the lifetime of the menu
void ui_menuing_add_option(char *option) {
  if (menu_entry_count >= MAX_MENU_ENTRIES) {
    debug_print("Menu overflow: ");
    debug_println(option);
    return;
  }
  menu_entries[menu_entry_count++] = option;
  // Funny hack, when we start updating, we will overflow by 1,
  // which triggers a re-render of option 0
  menu_selected_entry++;
}

// Does game loop updating of a generic menu.
// Returns true once an option is selected.
// Use selected_option to get the index of the selected option.
// It will be -1 if the menu was cancelled.
bool ui_menuing_update(int *selected_option, char **selected_text) {
  if (keypad_keypressed(KEY_MENU) || keypad_keypressed(KEY_LEFT)) {
    *selected_option = -1;
    return true;
  }

  // If the menu has no entries, our only option is cancel
  if (menu_entry_count == 0)
    return false;
  int prev_menu_entry = menu_selected_entry;

  if (keypad_keypressed(KEY_DOWN))
    menu_selected_entry++;
  if (keypad_keypressed(KEY_UP))
    menu_selected_entry--;
  menu_selected_entry =
      (menu_selected_entry + menu_entry_count) % menu_entry_count;

  if (keypad_keypressed(KEY_RIGHT)) {
    *selected_option = menu_selected_entry;
    *selected_text = menu_entries[menu_selected_entry];
    return true;
  }

  // Redraw the selected entry
  if (menu_selected_entry != prev_menu_entry) {
    char buffer[LCD_COLUMNS];
    snprintf(buffer, sizeof(buffer), "%-15.15s",
             menu_entries[menu_selected_entry]);
    lcd_set_cursor(1, 1);
    lcd_print(buffer);
  }

  return false;
}

static void ui_open_options() {
  ui_state = UI_OPTIONS;
  ui_menuing_begin("Menu:");
  ui_menuing_add_option("Clipping");
  ui_menuing_add_option("Chroma Key cfg");
  ui_menuing_add_option("Flash EEPROM1");
  ui_menuing_add_option("Flash EEPROM2");
}

static void ui_update_options() {
  int selected_option;
  char *selected_name;
  if (ui_menuing_update(&selected_option, &selected_name)) {
    switch (selected_option) {
      case 0:
        ui_open_transition(0, &ui_open_clipping);
        break;
      case 1:
        ui_open_transition(0, &ui_open_chroma_key_cfg);
        break;
      case 2:
        flash_ddc_eeprom(DDC_EEPROM1);
        lcd_clear();
        lcd_print("Flashed EEPROM1!");
        ui_open_transition(60, &ui_open_mixing);
        break;
      case 3:
        flash_ddc_eeprom(DDC_EEPROM2);
        lcd_clear();
        lcd_print("Flashed EEPROM2!");
        ui_open_transition(60, &ui_open_mixing);
        break;
      default:
        ui_open_transition(0, &ui_open_mixing);
        break;
    }
  }
}

static int clipping_menu_state = CLIP_MENU_LEFT;
static void ui_open_clipping() {
  ui_state = UI_CLIPPING_MENU;
  lcd_clear();
  lcd_print("Clipping ");

  char *name = NULL;
  uint16_t clip_val = 0;
  switch (clipping_menu_state) {
    case CLIP_MENU_LEFT:
      name = "left";
      clip_val = CURR_STATE.fg_clipping_left;
      break;
    case CLIP_MENU_RIGHT:
      name = "right";
      clip_val = CURR_STATE.fg_clipping_right;
      break;
    case CLIP_MENU_TOP:
      name = "top";
      clip_val = CURR_STATE.fg_clipping_top;
      break;
    case CLIP_MENU_BOTTOM:
      name = "bottom";
      clip_val = CURR_STATE.fg_clipping_bottom;
      break;
  }

  lcd_print(name);
  lcd_print(":");

  lcd_set_cursor(0, 1);
  char buf[LCD_COLUMNS + 1];
  if (clipping_menu_state == CLIP_MENU_LEFT || clipping_menu_state == CLIP_MENU_RIGHT) {
    snprintf(buf, sizeof(buf), "< %+05d > ^T Bv", clip_val);
  } else {
    snprintf(buf, sizeof(buf), "^ %+05d v <L R>", clip_val);
  }
}

static void ui_update_clipping() {
  bool dirty = false;
  int was_down_frames;

  if (keypad_keypressed(KEY_MENU)) {
    ui_open_transition(0, &ui_open_options);
    return;
  }
  if (keypad_keypressed(KEY_RESET_ALL)) {
    STATE_RESET_CLIPPING(CURR_STATE);
    dirty = true;
  }
  if (keypad_keypressed(KEY_RESET_OFFSET)) {
    switch (clipping_menu_state) {
      case CLIP_MENU_LEFT:
        CURR_STATE.fg_clipping_left = 0;
        break;
      case CLIP_MENU_RIGHT:
        CURR_STATE.fg_clipping_right = 0;
        break;
      case CLIP_MENU_TOP:
        CURR_STATE.fg_clipping_top = 0;
        break;
      case CLIP_MENU_BOTTOM:
        CURR_STATE.fg_clipping_bottom = 0;
        break;
    }
    dirty = true;
  }
  if (KEY_DOWN_OR_REPEAT(KEY_LEFT)) {
    if (clipping_menu_state == CLIP_MENU_LEFT && CURR_STATE.fg_clipping_left > 0) {
      CURR_STATE.fg_clipping_left--;
    } else if (clipping_menu_state == CLIP_MENU_RIGHT && CURR_STATE.fg_clipping_right < MAX_X_VAL) {
      CURR_STATE.fg_clipping_right++;
    } else {
      clipping_menu_state = CLIP_MENU_LEFT;
    }
    dirty = true;
  }
  if (KEY_DOWN_OR_REPEAT(KEY_RIGHT)) {
    if (clipping_menu_state == CLIP_MENU_RIGHT && CURR_STATE.fg_clipping_right > 0) {
      CURR_STATE.fg_clipping_right--;
    } else if (clipping_menu_state == CLIP_MENU_LEFT && CURR_STATE.fg_clipping_left < MAX_X_VAL) {
      CURR_STATE.fg_clipping_left++;
    } else {
      clipping_menu_state = CLIP_MENU_RIGHT;
    }
    dirty = true;
  }
  if (KEY_DOWN_OR_REPEAT(KEY_UP)) {
    if (clipping_menu_state == CLIP_MENU_TOP && CURR_STATE.fg_clipping_top > 0) {
      CURR_STATE.fg_clipping_top--;
    } else if (clipping_menu_state == CLIP_MENU_BOTTOM && CURR_STATE.fg_clipping_bottom < MAX_Y_VAL) {
      CURR_STATE.fg_clipping_bottom++;
    } else {
      clipping_menu_state = CLIP_MENU_TOP;
    }
    dirty = true;
  }
  if (KEY_DOWN_OR_REPEAT(KEY_DOWN)) {
    if (clipping_menu_state == CLIP_MENU_BOTTOM && CURR_STATE.fg_clipping_bottom > 0) {
      CURR_STATE.fg_clipping_bottom--;
    } else if (clipping_menu_state == CLIP_MENU_TOP && CURR_STATE.fg_clipping_top < MAX_Y_VAL) {
      CURR_STATE.fg_clipping_top++;
    } else {
      clipping_menu_state = CLIP_MENU_BOTTOM;
    }
    dirty = true;
  }

  if (dirty) {
    ui_handle_state_changes();
  }
}

#define CHROMA_KEY_RED_MASK 0xF800
#define CHROMA_KEY_GREEN_MASK 0x7E0
#define CHROMA_KEY_BLUE_MASK 0x1F
static int chroma_key_cfg_color = CHROMA_KEY_CFG_RED;
static void ui_open_chroma_key_cfg() {
  ui_state = UI_CHROMA_KEY_CFG;
  lcd_clear();
  lcd_print("Chroma Key cfg:");

  char buf[LCD_COLUMNS + 1];
  snprintf(buf, sizeof(buf), " R%02d  G%02d  B%02d",
      (CURR_STATE.chroma_key_cfg & CHROMA_KEY_RED_MASK) >> 11,
      (CURR_STATE.chroma_key_cfg & CHROMA_KEY_GREEN_MASK) >> 5,
      CURR_STATE.chroma_key_cfg & CHROMA_KEY_BLUE_MASK);
  lcd_set_cursor(0, 1);
  lcd_print(buf);

  switch (chroma_key_cfg_color) {
    case CHROMA_KEY_CFG_RED:
      lcd_set_cursor(0, 1);
      lcd_write('>');
      break;
    case CHROMA_KEY_CFG_GREEN:
      lcd_set_cursor(5, 1);
      lcd_write('>');
      break;
    case CHROMA_KEY_CFG_BLUE:
      lcd_set_cursor(10, 1);
      lcd_write('>');
      break;
  }
}

static void ui_update_chroma_key_cfg() {
  bool dirty = false;
  int was_down_frames;

  if (keypad_keypressed(KEY_MENU)) {
    ui_open_transition(0, &ui_open_options);
    return;
  }
  if (keypad_keypressed(KEY_RESET_ALL)) {
    CURR_STATE.chroma_key_cfg = CHROMA_KEY_CFG_DEFAULT;
    dirty = true;
  }
  if (keypad_keypressed(KEY_LEFT)) {
    chroma_key_cfg_color--;
    if (chroma_key_cfg_color < 0) {
      chroma_key_cfg_color = NUM_CHROMA_KEY_CFG_STATES - 1;
    }
    dirty = true;
  }
  if (keypad_keypressed(KEY_RIGHT)) {
    chroma_key_cfg_color++;
    chroma_key_cfg_color %= NUM_CHROMA_KEY_CFG_STATES;
    dirty = true;
  }
  if (KEY_DOWN_OR_REPEAT(KEY_UP)) {
    switch (chroma_key_cfg_color) {
      case CHROMA_KEY_CFG_RED:
        if (((CURR_STATE.chroma_key_cfg & CHROMA_KEY_RED_MASK) >> 11) < 31) {
          CURR_STATE.chroma_key_cfg += (1 << 11);
        }
        break;
      case CHROMA_KEY_CFG_GREEN:
        if (((CURR_STATE.chroma_key_cfg & CHROMA_KEY_GREEN_MASK) >> 5) < 63) {
          CURR_STATE.chroma_key_cfg += (1 << 5);
        }
        break;
      case CHROMA_KEY_CFG_BLUE:
        if ((CURR_STATE.chroma_key_cfg & CHROMA_KEY_BLUE_MASK) < 31) {
          CURR_STATE.chroma_key_cfg += 1;
        }
        break;
    }
    dirty = true;
  }
  if (KEY_DOWN_OR_REPEAT(KEY_DOWN)) {
    switch (chroma_key_cfg_color) {
      case CHROMA_KEY_CFG_RED:
        if (((CURR_STATE.chroma_key_cfg & CHROMA_KEY_RED_MASK) >> 11) > 0) {
          CURR_STATE.chroma_key_cfg -= (1 << 11);
        }
        break;
      case CHROMA_KEY_CFG_GREEN:
        if (((CURR_STATE.chroma_key_cfg & CHROMA_KEY_GREEN_MASK) >> 5) > 0) {
          CURR_STATE.chroma_key_cfg -= (1 << 5);
        }
        break;
      case CHROMA_KEY_CFG_BLUE:
        if ((CURR_STATE.chroma_key_cfg & CHROMA_KEY_BLUE_MASK) > 0) {
          CURR_STATE.chroma_key_cfg -= 1;
        }
        break;
    }
    dirty = true;
  }

  if (dirty) {
    ui_handle_state_changes();
  }
}

// Changes the state to use the live image as input
// If it used to be an image, clipping is reset
// Does not send the changes to the FPGA
static void source_select_live_input() {
  if (CURR_STATE.image_path_hash >= FG_IS_IMAGE_HASH)
    STATE_RESET_CLIPPING(CURR_STATE);
  CURR_STATE.image_path_hash = FG_IS_LIVE;
}

// Changes the state to freeze the current input
// Does not send the changes to the FPGA
static void source_select_frozen_input() {
  CURR_STATE.image_path_hash = FG_IS_FROZEN;
}

// Changes the current state to use the given image path
// Does not send the changes to the FPGA
static void source_select_image(char *path_name) {
  uint16_t hash = 2;
  int i = 0;
  for (; i < STATE_IMAGE_PATH_LEN && path_name[i] != '\0'; i++) {
    CURR_STATE.image_path[i] = path_name[i];
    hash = hash * 97 + path_name[i];
  }
  CURR_STATE.image_path[i] = '\0';
  CURR_STATE.image_path_hash = hash;
}

#define MAX_SD_CARD_ENTRIES 64
static void ui_open_source_select() {
  ui_state = UI_SOURCE_SELECT;
  ui_menuing_begin("Source select:");
  ui_menuing_add_option("Live from VGA1");
  ui_menuing_add_option("Frozen");
  if (sd_card_is_mounted()) {
    static direntry_t entries[MAX_SD_CARD_ENTRIES];
    int entries_read;
    if (sd_card_list_files("/", entries, MAX_SD_CARD_ENTRIES, &entries_read) == 0) {
      for (int i = 0; i < entries_read; i++) {
        ui_menuing_add_option(entries[i].fname);
      }
    }
  }
}

static void ui_update_source_select() {
  int selected_option;
  char *selected_name;
  if (ui_menuing_update(&selected_option, &selected_name)) {
    if (selected_option == -1) {
    }                              // Menu button pressed, cancels
    else if (selected_option == 0) // Live input
      source_select_live_input();
    else if (selected_option == 1) // Frozen input
      source_select_frozen_input();
    else
      source_select_image(selected_name);
    ui_handle_state_changes();
  }
}

static void ui_open_image_upload() {
  ui_state = UI_IMAGE_UPLOAD;
  lcd_clear();
  lcd_print("Uploading image");
}

static void ui_update_image_upload() {
  uint16_t uploaded, total;
  int res = image_upload_next_lines(6, &uploaded, &total);

  switch (res) {
  case IMAGE_UPLOAD_DONE:
    ui_open_mixing();
    return;
  case IMAGE_UPLOAD_ONGOING: {
    char buf[20];
    snprintf(buf, sizeof(buf), " Lines: %3d/%3d ", uploaded % 1000,
             total % 1000);
    lcd_set_cursor(0, 1);
    lcd_print(buf);
    return;
  }
  case IMAGE_UPLOAD_ERROR:
    lcd_clear();
    lcd_print("> IMAGE ERROR! <");
    ui_open_transition(60, &ui_open_mixing);
    return;
  }
}

static int state_select_selection;
static void ui_open_state_select() {
  ui_state = UI_STATE_SELECT;
  lcd_clear();
  lcd_print("Select state");
  lcd_set_cursor(0, 1);
  lcd_print("<P            N>");
  // We overflow on purpose to trigger a re-draw first update
  state_select_selection = current_mixing_state + NUM_MIXING_STATES;
}

static void ui_update_state_select() {
  int last_state_select_selection = state_select_selection;

  if (keypad_keypressed(KEY_PSTATE))
    state_select_selection--;
  if (keypad_keypressed(KEY_NSTATE))
    state_select_selection++;
  state_select_selection = (state_select_selection + NUM_MIXING_STATES) % NUM_MIXING_STATES;

  // Hold reset to trigger copy the existsing state to the new one
  int was_down_time;
  if (keypad_keydown(KEY_RESET_ALL, &was_down_time) && was_down_time >= MODE_HOLD_THRESHOLD) {
    // Copy CURR_STATE into the new slot
    if (current_mixing_state != state_select_selection)
      memcpy(&mixing_states[state_select_selection], &CURR_STATE, sizeof(CURR_STATE));
    current_mixing_state = state_select_selection;
    ui_handle_state_changes();
    return;
  }

  if (keypad_keyreleased(KEY_RESET_ALL, NULL)) {
    current_mixing_state = state_select_selection;
    ui_handle_state_changes();
    return;
  }

  if (keypad_keypressed(KEY_MENU)) {
    // abort state change
    ui_handle_state_changes();
    return;
  }

  if (state_select_selection != last_state_select_selection) {
    lcd_set_cursor(LCD_COLUMNS/2, 1);
    lcd_write('0' + state_select_selection);
  }
}

void ui_update() {
  switch (ui_state) {
  case UI_TRANSITION:
    ui_update_transition();
    break;
  case UI_MIXING:
    ui_update_mixing();
    break;
  case UI_OPTIONS:
    ui_update_options();
    break;
  case UI_CLIPPING_MENU:
    ui_update_clipping();
    break;
  case UI_CHROMA_KEY_CFG:
    ui_update_chroma_key_cfg();
    break;
  case UI_SOURCE_SELECT:
    ui_update_source_select();
    break;
  case UI_IMAGE_UPLOAD:
    ui_update_image_upload();
    break;
  case UI_STATE_SELECT:
    ui_update_state_select();
    break;
  }
}
