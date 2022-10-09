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

#include "blink.h"
#include "debug.h"
#include "gpio.h"
#include "i2c.h"

void btn_handler( char* message) {
  GPIO_IntClear(0xFFFF);
  debug_send_string(message);
}

void GPIO_EVEN_IRQHandler(void) { btn_handler("Hello from even\n"); }
void GPIO_ODD_IRQHandler(void)  { btn_handler("Hello from odd\n");  }

int main(void)
{
  // Initialize Silicon Labs device, system, service(s) and protocol stack(s).
  // Note that if the kernel is present, processing task(s) will be created by
  // this call.
  sl_system_init();
  debug_init();
  initGPIO();
  initI2C();

  while (1) {}
}
