#include "sd_card.h"
#include <stdbool.h>
#include "debug.h"

#include "vendored/ff.h"

static FATFS fs;
static bool has_fs;

#define ASSERT_RES(res, msg) do { if((res) != FR_OK) \
    { debug_print("sdcard error: " msg ": "); debug_printintln(res); return; } } while(false)

void sd_card_init() {
  FRESULT fr;
  has_fs = false;

  // Try mounting 5 times
  for (int i = 0; i < 5 && !has_fs; i++) {
    fr = f_mount(&fs, "", 0);
    has_fs |= fr == FR_OK;
  }
  if (has_fs)
    debug_println("SD card mounted");
  else
    debug_println("SD card mounting failed");

  FIL f;
  FRESULT res = f_open(&f, "test.txt", FA_READ|FA_OPEN_EXISTING);
  ASSERT_RES(res, "opening file");

  char data[128];
  unsigned int actually_read;
  res = f_read(&f, data, sizeof(data)-1, &actually_read);
  ASSERT_RES(res, "reading file");

  data[actually_read] = '\0';
  debug_print("read from test.txt: ");
  debug_println(data);

  f_close(&f);
}
