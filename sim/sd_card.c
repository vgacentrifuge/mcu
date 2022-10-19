#include "sd_card.h"

static bool is_mounted;

int sd_card_init() {
    is_mounted = true;
    return 0;
}

bool sd_card_is_mounted() {
    return is_mounted;
}

int sd_card_list_files(const char* path, direntry_t *entries,
                       int max_entries, int* entries_read) {
    (void)path; (void) entries; (void) max_entries;

    //TODO Simulate sd card from a folder
    *entries_read = 0;
    return 0;
}
