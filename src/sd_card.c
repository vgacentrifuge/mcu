#include "sd_card.h"
#include <stdbool.h>
#include "debug.h"

#include "vendored/ff.h"

static FATFS fs;
char data[4097];

#define ASSERT_RES(res, msg) do { if((res) != FR_OK) \
    { debug_print("sdcard error: " msg ": "); debug_printintln(res); return; } } while(false)

void sd_card_init() {
  FRESULT fr;

  // Try mounting 5 times
  for (int i = 0; i < 5; i++) {
    fr = f_mount(&fs, "", 0);
    if(fr == FR_OK)
      break;
  }
  debug_println("SD card mounted");

  FRESULT res;
  FIL f;
  res = f_open(&f, "test.txt", FA_READ|FA_OPEN_EXISTING);
  ASSERT_RES(res, "opening file");

  unsigned int actually_read;
  res = f_read(&f, data, sizeof(data)-1, &actually_read);
  ASSERT_RES(res, "reading file");

  data[actually_read] = '\0';
  debug_print("read from test.txt: ");
  debug_println(data);

  f_close(&f);
}
