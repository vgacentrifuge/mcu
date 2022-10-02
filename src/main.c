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
#include <em_device.h>
#include <em_chip.h>
#include <em_i2c.h>
#include <em_cmu.h>
#include <em_emu.h>
#include <em_gpio.h>
#include <bsp.h>

#include "blink.h"
#include "debug.h"
#include "lcd.h"

int main(void)
{
  // Initialize Silicon Labs device, system, service(s) and protocol stack(s).
  // Note that if the kernel is present, processing task(s) will be created by
  // this call.
  sl_system_init();

  // Chip init
  CHIP_Init();

  // Configuring clocks in the Clock Management Unit (CMU)
  initCMU();

  // Initializations
  initGPIO();

  // Setting up i2c
  initI2C();

  //LED ON to indicate operating
  GPIO_PinOutSet(BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN);


  blink_init();

  debug_init();

  while (1) {
    // Do not remove this call: Silicon Labs components process action routine
    // must be called from the super loop.
    sl_system_process_action();

    blink_process_action();

    debug_send_string("foo\n");

    if(i2c_rxInProgress) {
       // Receiving data
       receiveI2CData();
    } else if (i2c_startTx) {
       // Transmitting data
       performI2CTransfer();
       // Transmission complete
       i2c_startTx = false;
    }

        // Forever enter EM2. The RTC or I2C will wake up the EFM32
        EMU_EnterEM2(false);

    #if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
      // Let the CPU go to sleep if the system allows it.
      sl_power_manager_sleep();
    #endif
  }
}
