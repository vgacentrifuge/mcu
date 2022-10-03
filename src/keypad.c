
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
 * Pin layout:
 * [ row0, row1, row2, row3, col0, col1, col2, col3 ]
 *
 *
 *
 * Wires are positioned in a grid, and each button will close the node.
 *
 * example: Pressing button 12 will close col0 and row3.
 *          Giving a signal in col0 will then result in the same signal being read from row3.
 *
 */

#include "em_gpio.h"
#include "stdio.h"
#include "sl_sleeptimer.h"

int latest_button_pressed = 0;

// Port 0=A, 1=B, 2=C and so on
int port = 3;
int ports_connected[8] = {3, 3, 3, 3, 3, 3, 3, 3};
int pins_connected[8] =  {0, 1, 2, 3, 4, 5, 6, 7};


void app_init(void) {

}


void button_pressed(int button) {
    printf("button %d pressed\n", button);
}


void app_process_action(void) {
    while(1) { // Check constantly
        for(int i = 0; i <= 3; i++) { // For each of the first 4 pins (rows)

            // Set pin i high, and wait for propagation?
            GPIO_PinOutSet(port, i);
            sl_sleeptimer_delay_millisecond(1);

            for(int j = 4; j <= 7; j++) { // For each of the first 4 pins (columns)

                int signal = GPIO_PinOutGet(port, j);; // Check pin j for signal
                if (signal) {
                    button_pressed(i*4 + (j-4));
                    latest_button_pressed = i*4 + (j-4);
                }
            }

            // Set pin i low
            GPIO_PinOutSet(port, i);
        }
    }
}

