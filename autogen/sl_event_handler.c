#include "sl_event_handler.h"

#include "em_chip.h"
#include "sl_device_init_nvic.h"
#include "sl_device_init_clocks.h"
#include "sl_device_init_emu.h"
#include "sl_sleeptimer.h"
#include "sl_debug_swo.h"
#include "sl_i2cspm_instances.h"
#include "sl_spidrv_instances.h"

void sl_platform_init(void)
{
  CHIP_Init();
  sl_device_init_nvic();
  sl_device_init_clocks();
  sl_device_init_emu();
}

void sl_driver_init(void)
{
  sl_debug_swo_init();
  sl_i2cspm_init_instances();
  sl_spidrv_init_instances();
}

void sl_service_init(void)
{
  sl_sleeptimer_init();
}

void sl_stack_init(void)
{
}

void sl_internal_app_init(void)
{
}

void sl_platform_process_action(void)
{
}

void sl_service_process_action(void)
{
}

void sl_stack_process_action(void)
{
}

void sl_internal_app_process_action(void)
{
}

