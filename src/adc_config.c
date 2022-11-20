/***************************************************************************//**
 * @file
 * @brief Configuration for the ADCs, using a lot of hard coded values,
 * to configure for 5-wire VGA RGA input with HSYNC and VSYNC.
 * This includes disabling a bunch of Sync-on-green stuff, clamping and
 *
 * Useful notes:
 *  - The HSYNC and VSYNC output signals will be active low no matter the input
 *  - The HSYNC signal delay is lined up with the RGB signal delay
 *  - VSYNC is a bit behind, but I'm not sure exactly how
 *
 * To support other resolutions:
 *  - The pixel clock PLL must know how many pixels there are per line.
 *  - The length of the HSYNC signal must be specified
 *  - If sample rate every becomes > 70MSPS, register 0x03 should be changed
 *  - If sample rate ever becomes > 110MSPS, register 0x2C should be changed
 * See the #define values in this file
 ******************************************************************************/

#include "adc_config.h"
#include "pin_config.h"

#include <stdint.h>
#include "sl_i2cspm.h"
#include "sl_i2cspm_instances.h"
#include "sl_sleeptimer.h"
#include "sl_udelay.h"
#include <stdio.h>
#include <stdint.h>
#include "debug.h"
#include "vgacentrifuge_config.h"

#define I2C sl_i2cspm_i2c1

// We store I2C addresses as 8-bit, with the LSB being changed for R/W (1/0)
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
    char out[30];
    snprintf(out, sizeof(out), "ADC %x write fail: %d", address, ret);
    debug_println(out);
  }
}

void configure_adc(uint8_t address) {
  // See ADC datasheet: https://www.ti.com/lit/ds/symlink/tvp7002.pdf

  write_register(address, 0x01, (HORIZONTAL_TOTAL>>4)&0xFF); // pixels per line [11:4]
  write_register(address, 0x02, (HORIZONTAL_TOTAL<<4)&0xFF); // pixels per line [3:0] into reg[7:4]

  // Should be changed if PCLK > 70MHz
  write_register(address, 0x03, 0b01101000); // Sets pixel clock range and charge pump current setting
  write_register(address, 0x04, 0x10); // PLL phase select and PLL divider, we do 180 degrees

  // Clamp settings taken from table 13, assuming "PC graphics"
  write_register(address, 0x05, 0x06); // clamp start
  write_register(address, 0x06, 0x10); // clamp width

  write_register(address, 0x07, HORIZONTAL_SYNC_WIDTH); // HSYNC output pixel width

  // Coarse (analog) and fine (digital) configuration
  write_register(address, 0x08, 0); // Blue fine gain control
  write_register(address, 0x09, 0); // Green fine gain control
  write_register(address, 0x0A, 0); // Red fine gain control
  // We try using 1.3 instead of 1.2 as course gain, since our input is 0.7V p-p, and the ADC takes 1V.
  write_register(address, 0x1B, 0xAA); // Green and blue coarse gain
  write_register(address, 0x1C, 0x0A); // Red coarse gain

  // Fine offset is 10 bits, an offset of 0 is achieved by [9:2]=0x80 [1:0]=0
  write_register(address, 0x0B, 0x80); // Blue fine offset MSBs
  write_register(address, 0x0C, 0x80); // Green fine offset MSBs
  write_register(address, 0x0D, 0x80); // Red fine offset MSBs
  write_register(address, 0x1D, 0x00); // Fine offset LSBs

  // The ALC description tells us that YRGB should target a bottom level of 32
  // We are supposed to decrement these until each color's ALC Out [9:0] ≤ 220h
  // ALC state is in read only registers 0x23, 0x24 and 0x25 as well as MSB in 0x27
  write_register(address, 0x1E, 0x08); // Blue coarse offset (Default 0x10)
  write_register(address, 0x1F, 0x08); // Green coarse offset (Default 0x10)
  write_register(address, 0x20, 0x08); // Red coarse offset (Default 0x10)

  const uint8_t HSPO = 0; // HSYNC input polarity determined by chip
  const uint8_t HSOP = 0; // HSYNC output polarity is active low
  const uint8_t AHSO = 1; // Active HSYNC input signal manually selected by AHSS
  const uint8_t AHSS = 0; // Active HSYNC is derived from the selected HSYNC input.
  const uint8_t VSOP = 0; // VSYNC output polarity is active low
  const uint8_t AVSO = 1; // Active VSYNC input signal manually selected by AVSS
  const uint8_t AVSS = 0; // Active VSYNC is derived from the selected VSYNC input.
  write_register(address, 0x0E, HSPO<<7 | HSOP<<5 | AHSO<<4 | AHSS<<3 | VSOP<<2 | AVSO << 1 | AVSS);

  // 0x0F is H-PLL and Clamp Control, super complicated, I have no idea what it does
  // I don't think we need any clamping, so we change the defaults of 0x2A
  //write_register(address, 0x2A, 0b00000100); // Fine Clamp Control. Disable it for R,G,B
  write_register(address, 0x2D, 0x00); // Course Clamp Control. All disabled (which is also default)
  write_register(address, 0x10, 0x58); // Set all colors to bottom-level fine clamp (not sure if matters)

  write_register(address, 0x11, 0x40); // Sync separator threshold
  // 0x14 is read only
  write_register(address, 0x15, 0x04); // The default, RGB 4:4:4 mode

  write_register(address, 0x17, 0x02); // Enable outputs, except for SOG out

  write_register(address, 0x19, 0x00); // Input MUX select, we use RIN_1, GIN_1, BIN_1
  write_register(address, 0x1A, 0xCA); // Input MUX select 2 VSYNC_A and HSYNC_A. Changed to use EXT_CLK for Sync Processing

  // Since RGB is 18 cycles delayed, there is a 18-5=13 cycle head start to the hsync signal
  write_register(address, 0x21, 13); //HSOUT Output Start delay  (in pixel clock cycles). + 5 extra cycles
  write_register(address, 0x35, 0x10); //VSYNC Alignment (offset between leading HSYNC and leading VSYNC) //Default 0x10
  write_register(address, 0x36, 0); // Don't enable any sync bypass (Default=0)

  const uint8_t MAC_EN = 0; // No "Macrovision", thank you
  const uint8_t COAST_DISABLE = 1; // We hopefully dont need coast when we have 5 wires.
  write_register(address, 0x22, MAC_EN << 3 | COAST_DISABLE << 2 );
  write_register(address, 0x12, 0x00); // H-PLL pre coast. We disable coast either way.
  write_register(address, 0x13, 0x00); // H-PLL post coast. We disable coast either way.

  // Automatic Level Control
  // There are read only registers for configuring, see the coarse offset registers above
  write_register(address, 0x26, 0x80); // Leave automatic level control enabled
  // ALC placement. Since our input is plain black during the entire blanking period, try 0
  write_register(address, 0x31, 0x00); // Default: 0x5A

  // We don't do any power control, leave 0x2B alone for now
  // Leave ADC setup on default, since sample rate < 110MSPS (reg. 0x2C)
}
