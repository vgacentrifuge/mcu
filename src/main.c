/***************************************************************************//**
 * @file main.c
 * @brief main() function.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include "sl_component_catalog.h"
#include "sl_system_init.h"
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif
#include "sl_system_process_action.h"
#include <stdbool.h>


#include "blink.h"
#include "gpio.h"
#include "debug.h"

volatile bool can_press = false;

void btn_handler( char* message) {
  GPIO_IntClear(0xFFFF);
  debug_send_string(message);
}

void GPIO_EVEN_IRQHandler(void) { btn_handler("Hello from even\n"); }

void GPIO_ODD_IRQHandler(void) { btn_handler("Hello from odd\n"); }

int main(void)
{
  // Initialize Silicon Labs device, system, service(s) and protocol stack(s).
  // Note that if the kernel is present, processing task(s) will be created by
  // this call.
  sl_system_init();

  blink_init();

  debug_init();

  initGPIO();

  while (true) {}

//  while (1) {
//    // Do not remove this call: Silicon Labs components process action routine
//    // must be called from the super loop.
//    sl_system_process_action();
//
//    //    bool button1 = GPIO_PinInGet(BUTTON_PORT, BUTTON_PIN);
//    //    bool button2 = GPIO_PinInGet(BUTTON_2_PORT, BUTTON_2_PIN);
//    //
//    //    if (button1 == 0) blink_process_action();
//    //    if (button2 == 0) debug_send_string("foo\n");
//
//
//#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
//    // Let the CPU go to sleep if the system allows it.
//    sl_power_manager_sleep();
//#endif
//  }
}
