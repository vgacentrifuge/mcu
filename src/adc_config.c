#include "adc_config.h"
#include "pin_config.h"

#include <stdint.h>
#include "sl_i2cspm.h"
#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"
#include "sl_udelay.h"
#include <stdio.h>
#include "debug.h"

#define I2C sl_i2cspm_i2c1

// We store I2C adresses as 8-bit, with the LSB being changed for R/W (1/0)
#define ADC1_ADDRESS 0b10111000
#define ADC2_ADDRESS 0b10111010

void configure_adc(uint8_t address);

void adcs_init() {
  // Reset is active low, so set both chips to reset
  GPIO_PinModeSet(ADC1_RESET_PORT, ADC1_RESET_PIN, gpioModePushPull, 0);
  GPIO_PinModeSet(ADC2_RESET_PORT, ADC2_RESET_PIN, gpioModePushPull, 0);
  sl_sleeptimer_delay_millisecond(5); // Let the chips power on properly
  GPIO_PinOutSet(ADC1_RESET_PORT, ADC1_RESET_PIN);
  GPIO_PinOutSet(ADC2_RESET_PORT, ADC2_RESET_PIN);
  sl_udelay_wait(10); //Wait for the chips to activate

  configure_adc(ADC1_ADDRESS);
  configure_adc(ADC2_ADDRESS);
}

void write_register(uint8_t address, uint8_t reg, uint8_t value) {
  uint8_t buffer[2] = {reg, value};
  I2C_TransferSeq_TypeDef transfer = {
    .addr = address + 0, //LSB 0 is write
    .flags = I2C_FLAG_WRITE,
    .buf[0]={buffer, 2},
  };
  int ret = I2CSPM_Transfer(I2C, &transfer);
  if (ret != 0) {
    char out[24];
    snprintf(out, sizeof(out), "ADC write fail: %d", ret);
    debug_println(out);
  }
}

void configure_adc(uint8_t address) {
  // For SVGA 800x600 @ 60.317Hz, taken from the chart in the datasheet
  uint16_t pixels_per_line = 1056;
  write_register(address, 0x01, pixels_per_line>>4); // pixels per line [11:4]
  write_register(address, 0x02, pixels_per_line<<4); // pixels per line [3:0] into reg[7:4]
  write_register(address, 0x03, 0x58); // Sets pixel clock range and charge pump current setting
  write_register(address, 0x04, 0); // PLL phase select and PLL divider, we do 0 degrees

  // Clamp settings taken from table 13, assuming "PC graphics"
  write_register(address, 0x05, 0x06); // clamp start
  write_register(address, 0x06, 0x10); // clamp width

  write_register(address, 0x07, 128); // HSYNC output pixel width (taken from timing site)

  // Coarse (analog) and fine (digital) configuration
  write_register(address, 0x08, 0); // Blue fine gain control
  write_register(address, 0x09, 0); // Green fine gain control
  write_register(address, 0x0A, 0); // Red fine gain control
  // We try using 1.3 instead of 1.2 as course gain, since our input is 0.7V p-p, and the ADC takes 1V.
  write_register(address, 0x1B, 0xAA); // Green and blue coarse gain
  write_register(address, 0x1C, 0x0A); // Red coarse gain

  write_register(address, 0x0B, 0x80); // Blue fine offset MSBs
  write_register(address, 0x0C, 0x80); // Green fine offset MSBs
  write_register(address, 0x0D, 0x80); // Red fine offset MSBs
  write_register(address, 0x1D, 0x00); // Fine offset LSBs
  write_register(address, 0x1E, 0x10); // Blue coarse offset
  write_register(address, 0x1F, 0x10); // Green coarse offset
  write_register(address, 0x20, 0x10); // Red coarse offset

  const uint8_t HSPO = 0; // HSYNC input polarity determined by chip
  const uint8_t HSOP = 0; // HSYNC output polarity is active low
  const uint8_t AHSO = 1; // Active HSYNC input signal manually selected by AHSS
  const uint8_t AHSS = 0; // Active HSYNC is derived from the selected HSYNC input.
  const uint8_t VSOP = 0; // VSYNC output polarity is active low
  const uint8_t AVSO = 1; // Active VSYNC input singnal manually selected by AVSS
  const uint8_t AVSS = 0; // Active VSYNC is derived from the selected VSYNC input.
  write_register(address, 0x0E, HSPO<<7 | HSOP<<5 | AHSO<<4 | AHSS<<3 | VSOP<<2 | AVSO << 1 | AVSS);
  write_register(address, 0x36, 0); // A project on GitHub did this

  // 0x0F is H-PLL and Clamp Control, super complicated, I have no idea what it does
  write_register(address, 0x10, 0x58); // Set all colors to bottom-level fine clamp
  write_register(address, 0x11, 0x40); // Sync separator threshold
  // 0x14 is read only
  write_register(address, 0x15, 0x04); // The default, RGB 4:4:4 mode

  write_register(address, 0x17, 0x02); // Enable outputs, except for SOG out

  write_register(address, 0x19, 0x00); // Input MUX select, we use RIN_1, GIN_1, BIN_1
  write_register(address, 0x1A, 0xCA); // Input MUX select 2 VSYNC_A and HSYNC_A. Changed to use EXT_CLK for Sync Processing

  write_register(address, 0x21, 0x00); // HSOUT Output Start delay (in pixel clock cycles). +5 cycles

  const uint8_t MAC_EN = 0; // No "Macrovision", thank you
  const uint8_t COAST_DISABLE = 1; // We hopefully dont need coast when we have 5 wires.
  write_register(address, 0x22, MAC_EN << 3 | COAST_DISABLE << 2 );
  write_register(address, 0x12, 0x00); // H-PLL pre coast. We disable coast either way.
  write_register(address, 0x13, 0x00); // H-PLL post coast. We disable coast either way.

  write_register(address, 0x26, 0x80); // Leave automatic level control enabled

  // We don't do any power control, leave 0x2B alone for now
  // Leave ADC setup on default, since sample rate < 110MSPS (reg. 0x2C)

  
}
