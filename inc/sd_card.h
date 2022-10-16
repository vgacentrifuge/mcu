#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "fatfs/ffconf.h"

typedef struct {
#if FF_USE_LFN >= 1
  char fname[FF_LFN_BUF + 1]; //based on ff.h's FILINFO struct
#else
  char fname[12 + 1];
#endif
  uint32_t fsize;
  bool is_dir;
} direntry_t;

int sd_card_init();

bool sd_card_is_mounted();

/**
 * Lists files in the given directory.
 * If it somehow fails, the FATFS error code is printed to debug,
 * and a non 0 return value is given.
 *
 * @param path - [IN] directory path, e.g. "/"
 * @param entries - [OUT] the entries in the directory
 * @param max_entries - [IN] maximum entry count to
 * @param entries_read - [OUT] the number of entries read
 * @return res - 0 (=FR_OK) if everything went well
 */
int sd_card_list_files(const char* path, direntry_t *entries,
                       int max_entries, int* entries_read);
