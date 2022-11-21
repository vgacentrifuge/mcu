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

#include "fatfs/ff.h"
FRESULT f_open (
	FIL* fp,			/* Pointer to the blank file object */
	const TCHAR* path,	/* Pointer to the file name */
	BYTE mode			/* Access mode and open mode flags */
) {return FR_OK;}
FRESULT f_read (
	FIL* fp, 	/* Open file to be read */
	void* buff,	/* Data buffer to store the read data */
	UINT btr,	/* Number of bytes to read */
	UINT* br	/* Number of bytes read */
) {return FR_OK;}
FRESULT f_lseek (
	FIL* fp,		/* Pointer to the file object */
	FSIZE_t ofs		/* File pointer from top of file */
) {return FR_OK;}
FRESULT f_close (
	FIL* fp		/* Open file to be closed */
) {return FR_OK;}
