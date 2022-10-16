#include "sd_card.h"
#include <string.h>
#include "debug.h"

#include "fatfs/ff.h"

#define LOGICAL_DRIVE_NUMBER ""
#define MOUNT_IMMEDIATLY 1

static FATFS fs;
static bool is_mounted;

#define ASSERT_RES(res, msg) do {               \
    if((res) != FR_OK) {                        \
        debug_print("sdcard error: " msg ": "); \
        debug_printintln(res);                  \
        is_mounted = false;                     \
        return res;                             \
    }                                           \
  } while(false)

int sd_card_init() {
  FRESULT res;

  res = f_mount(&fs, LOGICAL_DRIVE_NUMBER, MOUNT_IMMEDIATLY?1:0);
  ASSERT_RES(res, "mounting");
  is_mounted = true;
  debug_println("SD card mounted");

  // We don't really have any deinit code, since the MCU loops forever
  // but if we did, we could call
  // f_unmount(LOGICAL_DRIVE_NUMBER);

  return res;
}

bool sd_card_is_mounted() {
  return is_mounted;
}

int sd_card_list_files(const char* path, direntry_t *entries,
                       int max_entries, int* entries_read) {
  FRESULT res;
  DIR dir;
  static FILINFO fno;

  *entries_read = 0;

  res = f_opendir(&dir, path);
  ASSERT_RES(res, "list_files opendir");

  for(; *entries_read < max_entries; (*entries_read)++) {
    res = f_readdir(&dir, &fno);
    if(res != FR_OK || fno.fname[0] == 0) break; //Error, or end of dir reached
    memcpy(entries[*entries_read].fname, fno.fname, sizeof(fno.fname));
    entries[*entries_read].is_dir = fno.fattrib & AM_DIR;
    entries[*entries_read].fsize = fno.fsize;
  }
  f_closedir(&dir);

  ASSERT_RES(res, "list_files readdir");
  return res;
}
