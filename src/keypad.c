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

#define MIN_HOLD_TIME 3
// The number of samples a button has been sampled HIGH.
// The button counts as officially "pressed" when key_down_time == MIN_HOLD_TIME
// The button is "down" if key_down_time >= MIN_HOLD_TIME
// When the key is released, key_down_time gets its sign flipped, for one frame.
// This lets keypad_keyreleased() return for how long the button was regarded as down.
// A key is only regarded as released, if it was sampled HIGH long enough to be "pressed".
static int key_down_time[KEYPAD_ROWS*KEYPAD_COLS];

void keypad_init() {
  // Rows are outputs, they have protective resistors to avoid fighting
  for (int i = 0; i < KEYPAD_ROWS; i++)
    GPIO_PinModeSet(row_ports[i], row_pins[i], gpioModePushPull, 0);

  // Columns are inputs, they get pulled low when no button is connected
  for (int i = 0; i < KEYPAD_COLS; i++)
      GPIO_PinModeSet(col_ports[i], col_pins[i], gpioModeInputPull, 0);
}

void keypad_sample() {
  for(int row = 0; row < KEYPAD_ROWS; row++) {
    GPIO_PinOutSet(row_ports[row], row_pins[row]);
    sl_udelay_wait(10); // wait for some microseconds for RC to happen
    for (int col = 0; col < KEYPAD_COLS; col++) {
      int index = KEY_INDEX(col, row);

      // If the key was released last frame, reset it
      if(key_down_time[index] < 0)
        key_down_time[index] = 0;

      if (GPIO_PinInGet(col_ports[col], col_pins[col])) {
        //The button is down
        key_down_time[index]++;
      } else {
        // Key is not pressed
        if(key_down_time[index] > 0) // Flip the sign to indicte "was held this long"
          key_down_time[index] = -key_down_time[index];
      }
    }
    GPIO_PinOutClear(row_ports[row], row_pins[row]);
  }
}

bool keypad_keydown(int key) {
  return key_down_time[key] >= MIN_HOLD_TIME;
}

bool keypad_keypressed(int key) {
  return key_down_time[key] == MIN_HOLD_TIME;
}

bool keypad_keyreleased(int key, int* was_down_frames) {
  if(was_down_frames) {
      *was_down_frames = -key_down_time[key] - MIN_HOLD_TIME + 1;
  }
  return key_down_time[key] <= -MIN_HOLD_TIME;
}
