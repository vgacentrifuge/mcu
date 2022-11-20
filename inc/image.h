#pragma once
#include <stdint.h>

enum {
  IMAGE_UPLOAD_DONE = 0,
  IMAGE_UPLOAD_ONGOING = 1,
  IMAGE_UPLOAD_ERROR = 2
};

// Starts uploading an image
// Doesn't send any actual pixel data,
// but will retrieve the width and height from the image.
// Returns IMAGE_UPLOAD_ONGING if the image was valid
// Returns IMAGE_UPLOAD_ERROR if not a valid .BMP file
// or if the image is too large.
int image_open_for_upload(char *path);

// Uploads up to lines more lines.
// When done with all lines in the picture, IMAGE_UPLOAD_DONE is returned,
// If there are more left, IMAGE_UPLOAD_ONGOING is returned.
// Finally, if something went wrong, IMAGE_UPLOAD_ERROR is returned.
//
// If IMAGE_UPLOAD_ONGOING was returned, the pointers uploaded and total
// will contain the currently uploaded lines, and total amount of lines in the image
int image_upload_next_lines(uint16_t lines, uint16_t *uploaded, uint16_t *total);

// When the picture is smaller than the default video resolution,
// Clipping is necessary to avoid showing garbage.
void image_get_size(uint16_t *width, uint16_t *height);
