#include "ddc_data.h"
#include "sl_i2cspm.h"
#include "sl_sleeptimer.h"
#include "debug.h"
#include <stdint.h>
#include <stdio.h>

// Init structs defined in ddc_i2c_config.c
extern I2CSPM_Init_TypeDef init_eeprom1;
extern I2CSPM_Init_TypeDef init_eeprom2;
#define I2C_EEPROM_PERIPHERAL I2C0

// We use the 8-bit version of the address
#define EEPROM_ADDRESS 0b10100000

#define PIXEL_CLOCK 4000 //40MHz
// In pixels
#define HORIZONTAL_ACTIVE 800
#define HORIZONTAL_BLANKING 256
#define HORIZONTAL_SYNC_OFFSET 40 // From blanking to HSYNC = front porch
#define HORIZONTAL_SYNC_WIDTH 128
// In Lines
#define VERTICAL_ACTIVE 600
#define VERTICAL_BLANKING 28
#define VERTICAL_SYNC_OFFSET 1 // From blanking to VSYNC = front porch
#define VERTICAL_SYNC_WIDTH 4

// In mm
#define HORIZONTAL_IMAGE_SIZE 320
#define VERTICAL_IMAGE_SIZE 240

const uint8_t DDC_DATA[128] = {
    // ===  Header ===
    [0] = 0x00, [1 ... 6] = 0xFF, [7] = 0x00,

    // === Vendor / ProductID bytes ===
    // ID Manufacturer Name
    [8] = 0b01011000,
    [9] = 0b11100001,
    // Model number
    [0xA ... 0xB] = 0,
    // Serial number
    [0xC ... 0xF] = 0,
    [0x10] = 45, [0x11] = 32, // Week and year

    // EDID Structure Version
    [0x12] = 1, [0x13] = 3, //Version 1 Rev 3

    // Basic display Parameters
    // Video Input Definition
    [0x14] = 0b01101000, // Only support analog, 5-wire
    [0x15] = 32, [0x16] = 24, // Max width, max height
    // Display gamma
    [0x17] = 150,
    // Feature support
    [0x18] = 0b00001111,

    // Chromaticity, copied from the APPENDIX A - Sample EDID
    [0x19] = 0xEE, [0x1A] = 0x91, [0x1B] = 0xA3, [0x1C] = 0x54,
    [0x1D] = 0x4C, [0x1E] = 0x99, [0x1F] = 0x26, [0x20] = 0x0F,
    [0x21] = 0x50, [0x22] = 0x54,

    // Established timing I
    [0x23] = 0b00000001, // 800x600 @ 60Hz
    [0x24] = 0,
    [0x25] = 0,

    // Standard timing - we set all to NOT USED
    [0x26 ... 0x35] = 0x01,

    // Detailed timing section
    // Our first block is preferred detailed timing
    [0x36] = PIXEL_CLOCK&0xFF, [0x37] = (PIXEL_CLOCK>>8)&0xFF, //40MHz -> 4000 -> 0x0FA0
    [0x38] = HORIZONTAL_ACTIVE&0xFF,
    [0x39] = HORIZONTAL_BLANKING&0xFF,
    [0x3A] = ((HORIZONTAL_ACTIVE>>4)&0xF0) | ((HORIZONTAL_BLANKING>>8)&0x0F),

    [0x3B] = VERTICAL_ACTIVE&0xFF,
    [0x3C] = VERTICAL_BLANKING&0xFF,
    [0x3D] = ((VERTICAL_ACTIVE>>4)&0xF0) | ((VERTICAL_BLANKING>>8)&0x0F),

    [0x3E] = HORIZONTAL_SYNC_OFFSET&0xFF,
    [0x3F] = HORIZONTAL_SYNC_WIDTH&0xFF,

    [0x40] = ((VERTICAL_SYNC_OFFSET<<4)&0xF0) | (VERTICAL_SYNC_WIDTH & 0xF),

    [0x41] = ((HORIZONTAL_SYNC_OFFSET>>2)&0xC0) | ((HORIZONTAL_SYNC_WIDTH>>4)&0x30)
    | ((VERTICAL_SYNC_OFFSET>>6)&0x0c) | ((VERTICAL_SYNC_WIDTH>>8)&0x03),

    [0x42] = HORIZONTAL_IMAGE_SIZE&0xFF,
    [0x43] = VERTICAL_IMAGE_SIZE&0xFF,
    [0x44] = ((HORIZONTAL_IMAGE_SIZE>>4)&0xF0) | ((VERTICAL_IMAGE_SIZE>>8)&0x0F),

    [0x45] = 0, [0x46] = 0, // No borders
    // flags. Vsync and Hsync have positive polarity
    [0x47] = 0b00011110,

    // Detailed timing / monitor descriptor #2
    [0x48 ... 0x4A] = 0, // flags 0 to indicate monitor descriptor
    [0x4B] = 0xFF, // Monitor Serial Number
    [0x4C] = 0, // flag 0 when used as descriptor
    // 13 bytes of serial number
    [0x4D] = 'A', [0x4E] = 'B', [0x4F] = 'C', [0x50] = 'D',
    [0x51] = 'a', [0x52] = 'b', [0x53] = 'c', [0x54] = 'd',
    [0x55] = 0x0A, // End of ASCI
    [0x56 ... 0x59] = 0x20, // Padding

    // Detailed timing / monitor descriptor #3
    [0x5A ... 0x5C] = 0, // flags 0 to indicate monitor descriptor
    [0x5D] = 0xFC, // Monitor Name
    [0x5E] = 0, // flag 0 when used as descriptor
    // 13 bytes of serial number
    [0x5F] = 'V', [0x60] = 'G', [0x61] = 'A', [0x62] = 'c',
    [0x63] = 'e', [0x64] = 'n', [0x65] = 't', [0x66] = 'r',
    [0x67] = 'i', [0x68] = 'f', [0x69] = 'u', [0x6A] = 'g',
    [0x6B] = 'e',

    // Detailed timing / monitor descriptor #4
    [0x6C ... 0x6E] = 0, // flags 0 to indicate monitor descriptor
    [0x6F] = 0x10, // Descriptor space is unused
    [0x70] = 0, // flag 0 when used as descriptor
    [0x71 ... 0x7D] = 0, // Dummy data

    [0x7E] = 0 // How many blocks of extension we have
    //0x7F is the checksum, which must be correct on the EEPROM
};

static void write(uint8_t* buffer, int len) {
  I2C_TransferSeq_TypeDef transfer = {
    .addr = EEPROM_ADDRESS,
    .flags = I2C_FLAG_WRITE,
    .buf[0]={buffer, len},
  };
  int ret = I2CSPM_Transfer(I2C_EEPROM_PERIPHERAL, &transfer);
  if (ret != 0) {
    char out[30];
    snprintf(out, sizeof(out), "EEPROM write fail: %d", ret);
    debug_println(out);
  }
}

static void read(uint8_t address, uint8_t* buffer, int len) {
  I2C_TransferSeq_TypeDef transfer = {
      .addr = EEPROM_ADDRESS,
      .flags = I2C_FLAG_WRITE_READ,
      .buf[0]={&address, 1},
      .buf[1]={buffer, len},
    };
    int ret = I2CSPM_Transfer(I2C_EEPROM_PERIPHERAL, &transfer);
    if (ret != 0) {
      char out[30];
      snprintf(out, sizeof(out), "EEPROM read fail: %d", ret);
      debug_println(out);
    }
}

void flash_ddc_eeprom(int eeprom) {
  switch(eeprom) {
    case DDC_EEPROM1:
      debug_println("Flashing EEPROM1");
      I2CSPM_Init(&init_eeprom1);
      break;
    case DDC_EEPROM2:
      debug_println("Flashing EEPROM2");
      I2CSPM_Init(&init_eeprom2);
      break;
    default: return;
  }

  // Write 16 bytes (a page) at once. First byte is address
  uint8_t buffer[17];
  for(unsigned int i = 0; i < sizeof(DDC_DATA); i+=16) {
    buffer[0] = i;
    for(int j = 0; j < 16; j++)
      buffer[j+1] = DDC_DATA[i+j];
    write(buffer, 17);
    sl_sleeptimer_delay_millisecond(5); // Give it 5ms to write the page
  }

  buffer[0] = 0x7F; // Fix the checksum byte
  uint8_t checksum = 0;
  for(unsigned int i = 0; i < sizeof(DDC_DATA); i++)
    checksum += DDC_DATA[i];
  buffer[1] = 256-checksum;
  write(buffer, 2);
  sl_sleeptimer_delay_millisecond(5); // Give it 5ms to write the page
  debug_println("Flashing done");

  // Now we want to check if the EEPROM is written correctly
  for(unsigned int i = 0; i < sizeof(DDC_DATA)-16; i+=16) {
    read(i, buffer, 16);
    for(int j = 0; j < 16; j++) {
      if(buffer[j] != DDC_DATA[i+j]) {
        debug_print("Incorrect byte read at");
        debug_printintln(i+j);
      }
    }
  }
}
