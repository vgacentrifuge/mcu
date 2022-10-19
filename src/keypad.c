/**
 *
 * Keypad layout:
 *
 *         col0 col1 col2 col3
 * Row 0: [ 0    1    2    3  ]
 * Row 1: [ 4    5    6    7  ]
 * Row 2: [ 8    9    10   11 ]
 * Row 3: [ 12   13   14   15 ]
 *
 * The rows have resistors, so can safely be driven both high and low
 *
 * Wires are positioned in a grid, and each button will close the node.
 *
 * example: Pressing button 12 will close col0 and row3.
 *          Giving a signal in row3 will then result in the same signal being read from col0.
 *
 */

#include "keypad.h"

#include "sl_sleeptimer.h"
#include "sl_udelay.h"
#include "em_gpio.h"
#include "pin_config.h"

const GPIO_Port_TypeDef row_ports[] = {
    KEYPAD_ROW0_PORT,
    KEYPAD_ROW1_PORT,
    KEYPAD_ROW2_PORT,
    KEYPAD_ROW3_PORT,
};

const GPIO_Port_TypeDef col_ports[] = {
    KEYPAD_COL0_PORT,
    KEYPAD_COL1_PORT,
    KEYPAD_COL2_PORT,
    KEYPAD_COL3_PORT,
};

const int row_pins[] = {
    KEYPAD_ROW0_PIN,
    KEYPAD_ROW1_PIN,
    KEYPAD_ROW2_PIN,
    KEYPAD_ROW3_PIN
};

const int col_pins[] = {
    KEYPAD_COL0_PIN,
    KEYPAD_COL1_PIN,
    KEYPAD_COL2_PIN,
    KEYPAD_COL3_PIN,
};

// When a button is first regarded as down, key_state becomes DEBOUNCE_CYCLES+2
#define DEBOUNCE_CYCLES 2
// When positive, and still down, decreases by one each sample, until it becomes 1
// Being 2 or below means the key is officially down.
// When the value is exactly 2, the key is officially pressed that frame.
// When the button is released, and was held for long enough to be "pressed",
// it will for one frame be considered officially released (value -1)
static int key_state[KEYPAD_ROWS*KEYPAD_COLS];

void keypad_init() {
  for (int i = 0; i < KEYPAD_ROWS; i++)
    GPIO_PinModeSet(row_ports[i], row_pins[i], gpioModePushPull, 0);
  for (int i = 0; i < KEYPAD_COLS; i++)
      GPIO_PinModeSet(col_ports[i], col_pins[i], gpioModeInput, 0);
}

void keypad_sample() {
  for(int row = 0; row < KEYPAD_ROWS; row++) {
      GPIO_PinOutSet(row_ports[row], row_pins[row]);
      sl_udelay_wait(10); // wait for some microseconds for RC to happen
      for (int col = 0; col < KEYPAD_COLS; col++) {
          int index = KEY_INDEX(col, row);
          if (GPIO_PinInGet(col_ports[col], col_pins[col])) {
              //The button is down
              if(key_state[index] == 0) // Pressed between last sample and now
                key_state[index] = DEBOUNCE_CYCLES+2;
              else if(key_state[index] > 1) // Held since last sample
                key_state[index]--;
          } else {
              // Key is not pressed
              if (keypad_keydown(index)) // Was officially "down" last sample
                key_state[index] = -1; // Released this frame
              else
                key_state[index] = 0;
          }
      }
      GPIO_PinOutClear(row_ports[row], row_pins[row]);
  }
}

bool keypad_keydown(int key) {
  return key_state[key] == 1 || key_state[key] == 1;
}

bool keypad_keypressed(int key) {
  return key_state[key] == 2;
}

bool keypad_keyreleased(int key) {
  return key_state[key] == -1;
}
