#include "image.h"
#include "fatfs/ff.h"
#include "fpga_spi.h"
#include "debug.h"
#include <stdbool.h>
#include "vgacentrifuge_config.h"

// Structs stolen from https://arduino.stackexchange.com/questions/19795/how-to-read-bitmap-image-on-arduino
struct __attribute__((__packed__)) bmp_file_header_t {
  uint16_t signature;
  uint32_t file_size;
  uint16_t reserved[2];
  uint32_t image_offset;
};

struct bmp_image_header_t {
  uint32_t header_size;
  uint32_t image_width;
  uint32_t image_height;
  uint16_t color_planes;
  uint16_t bits_per_pixel;
  uint32_t compression_method;
  uint32_t image_size;
  uint32_t horizontal_resolution;
  uint32_t vertical_resolution;
  uint32_t colors_in_palette;
  uint32_t important_colors;
};

static bool has_open_file_handle;
static FIL image_file_handle;

static uint16_t image_width;
static uint16_t image_height;
// How many lines have currently been sent
static uint16_t sent_lines;

// If the clause is false, closes any open file descriptor, and returns error
#define IM_ASSERT(clause) do{if(!(clause)) {          \
    debug_print("image.c IM_ASSERT failed on line "); \
    debug_printintln(__LINE__);                       \
    if(has_open_file_handle)                          \
      f_close(&image_file_handle);                    \
    has_open_file_handle = false;                     \
    return IMAGE_UPLOAD_ERROR;                        \
  }}while(false)

int image_open_for_upload(char *path) {
  if (has_open_file_handle)
    f_close(&image_file_handle);
  has_open_file_handle = false;

  int res = f_open(&image_file_handle, path, FA_READ);
  IM_ASSERT(res == FR_OK);
  has_open_file_handle = true;

  UINT bytes_read;
  struct bmp_file_header_t header;
  res = f_read(&image_file_handle, &header, sizeof(header), &bytes_read);
  IM_ASSERT(res == FR_OK && bytes_read == sizeof(header));

  // Little endian "BM" as signature
  IM_ASSERT(header.signature == 0x4D42);

  struct bmp_image_header_t image_header;
  res = f_read(&image_file_handle, &image_header, sizeof(image_header), &bytes_read);
  IM_ASSERT(res == FR_OK && bytes_read == sizeof(image_header));

  IM_ASSERT(image_header.header_size >= 40); // The headers should at least be recent!
  image_width = image_header.image_width;
  image_height = image_header.image_height;
  // Make sure the image isn't bigger than the resolution
  IM_ASSERT(image_width <= HORIZONTAL_ACTIVE && image_height <= VERTICAL_ACTIVE);
  IM_ASSERT(image_header.bits_per_pixel == 24); // Only support 24 bit (BGR) images
  IM_ASSERT(image_header.compression_method == 0); // No compression support!

  // Seek to the first byte of the pixel array
  f_lseek(&image_file_handle, header.image_offset);
  sent_lines = 0;

  return IMAGE_UPLOAD_ONGOING;
}

// The buffer used to prepare SPI transmissions
// The first 3 bytes are used for SPI command and line number
// Then 3 bytes per pixel since it is also used to read BMP lines
// Add an extra 3 bytes to allow padding the BMP reads
static uint8_t send_buffer[3 + HORIZONTAL_ACTIVE * 3 + 3];

int image_upload_next_lines(uint16_t lines, uint16_t *uploaded, uint16_t *total) {
  if (sent_lines >= image_height)
    return IMAGE_UPLOAD_DONE;
  if (!has_open_file_handle)
    return IMAGE_UPLOAD_ERROR;

  uint8_t *pixel_buffer = &send_buffer[3];

  for(int i = 0; i < lines; i++) {

    // first load the line from the file into the buffer
    // round the read length up to eat any BMP row padding
    uint16_t read_length = (image_width*3+3) % 4;
    UINT bytes_read;
    int res = f_read(&image_file_handle, pixel_buffer, read_length, &bytes_read);
    IM_ASSERT(res == FR_OK && read_length == bytes_read);

    // Now turn the 3-byte BGR colors into 16-bit, with red as MSBs
    for(int x = 0; x < image_width; x++) {
        uint16_t color16 = 0;
        color16 |= (pixel_buffer[3*x+2]<<8) & 0b1111100000000000; //red
        color16 |= (pixel_buffer[3*x+1]<<3) & 0b0000011111100000; //green
        color16 |= (pixel_buffer[3*x+0]>>3) & 0b0000000000011111; //blue

        // But the 16 bit color back in the buffer, MSB first
        pixel_buffer[2*x] = (color16>>8) & 0xFF;
        pixel_buffer[2*x+1] = color16 & 0xFF;
    }

    // Use bytes [1 ... 2] to pass the line number
    send_buffer[1] = (sent_lines>>8) & 0xFF;
    send_buffer[2] = sent_lines & 0xFF;
    fpga_spi_sendimageline(send_buffer, image_width);

    sent_lines++;

    if (sent_lines >= image_height) {
        f_close(&image_file_handle);
      has_open_file_handle = false;
      return IMAGE_UPLOAD_DONE;
    }
  }

  *total = image_height;
  *uploaded = sent_lines;
  return IMAGE_UPLOAD_ONGOING;
}

void image_get_size(uint16_t *width, uint16_t *height) {
  *width = image_width;
  *height = image_height;
}
