#include "board_io.h"
#include "em_gpio.h"

#ifdef ON_VGA_CENTRIFUGE_PCB
#include "pin_config.h"
#define BSP_GPIO_LED0_PORT GPIO_LED0_PORT
#define BSP_GPIO_LED0_PIN GPIO_LED0_PIN
#else
// This header only works for the devboard
#include "bspconfig.h"
#endif

void board_io_init() {
#ifdef BSP_GPIO_LED0_PIN
  GPIO_PinModeSet(BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN, gpioModePushPull, 0);
#endif
#ifdef BSP_GPIO_LED1_PIN
  GPIO_PinModeSet(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN, gpioModePushPull, 0);
#endif
#ifdef BSP_GPIO_PB0_PIN
  GPIO_PinModeSet(BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN, gpioModeInputPullFilter, 1);
#endif
#ifdef BSP_GPIO_PB1_PIN
  GPIO_PinModeSet(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN, gpioModeInputPullFilter, 1);
#endif

  /* To enable interrupts for buttons
  // Enable IRQ for even numbered GPIO pins
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);

  // Enable IRQ for odd numbered GPIO pins
  NVIC_EnableIRQ(GPIO_ODD_IRQn);

  // Enable falling-edge interrupts for PB pins
  GPIO_ExtIntConfig(BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN, BSP_GPIO_PB0_PIN, 1, 1, true);
  GPIO_ExtIntConfig(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN, BSP_GPIO_PB1_PIN, 1, 1, true);
   */
}

static bool button0, button1;
void board_io_sample() {
#ifdef BSP_GPIO_PB0_PIN
  button0 = !GPIO_PinInGet(BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN);
#endif
#ifdef BSP_GPIO_PB1_PIN
  button1 = !GPIO_PinInGet(BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN);
#endif
}

/**
 * Returns true if button 0 exists and is pressed
 */
bool board_io_get_button0() {
  return button0;
}

/**
 * Returns true if button 1 exists and is pressed
 */
bool board_io_get_button1() {
  return button1;
}

/**
 * Will set the state of LED0, if it exists
 * @param state - a boolean value, true to light the LED
 */
void board_io_set_led0(bool state) {
#ifdef BSP_GPIO_LED0_PIN
  if (state)
    GPIO_PinOutSet(BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN);
  else
    GPIO_PinOutClear(BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN);
#else
  (void)state;
#endif
}

/**
 * Will set the state of LED1, if it exists
 * @param state - a boolean value, true to light the LED
 */
void board_io_set_led1(bool state) {
#ifdef BSP_GPIO_LED1_PIN
  if (state)
    GPIO_PinOutSet(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN);
  else
    GPIO_PinOutClear(BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN);
#else
  (void)state;
#endif
}
