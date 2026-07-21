/*
 * FileSystem.c
 *
 *  Created on: Jul 4, 2026
 *      Author: Dawid Sac
 */
#include "App/FileSystem.h"
#include "Drivers/sd_driver.h"
#include "Common/Utils/Log.h"

#include "fatfs.h"

//static const char *CONFIG_PATH = "/.MBPlayerConfig";
//
//static struct {
//} gFS_context;

static void fs_sd_event_handler(enum sd_state new_state);

void fs_init(struct sd_init_struct *init_struct) {
	init_struct->event_cb = fs_sd_event_handler;
	sd_init(init_struct);
}

void fs_process() {
	sd_process();
}

void fs_list_files(void) {
	DIR dir;
	FILINFO fno;
	FRESULT res;

	res = f_opendir(&dir, SDPath);

	if (res == FR_OK) {
		for (;;) {
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0)
				break;

			if (fno.fattrib & AM_DIR) {
				printf("Folder: %s\r\n", fno.fname);
			} else {
				printf("File: %s \r\n", fno.fname);
			}
		}
		f_closedir(&dir);
	} else {
		printf("f_opendir failed with Error Code: %d\r\n", res);
	}
}

static void fs_verify_directories(void) {
	FILINFO fno;

	// Check for the Config folder
	if (f_stat("/.MBPlayerConfig", &fno) != FR_OK) {
		LOG(FILESYSTEM, LOG_INFO, "Config folder missing. Creating...\r\n");
		f_mkdir("/.MBPlayerConfig");
		f_chmod("/.MBPlayerConfig", AM_HID | AM_SYS, AM_HID | AM_SYS); // Hide it
	}

	// Check for the Metadata/Database folder
	if (f_stat("/.MBPlayer_metadata", &fno) != FR_OK) {
		LOG(FILESYSTEM, LOG_INFO, "Metadata folder missing. Creating...\r\n");
		f_mkdir("/.MBPlayer_metadata");
		f_chmod("/.MBPlayer_metadata", AM_HID | AM_SYS, AM_HID | AM_SYS);
	}
}

static void fs_sd_event_handler(enum sd_state new_state) {
	if (new_state == SD_STATE_MOUNTED) {
		LOG(FILESYSTEM, LOG_INFO,
				"SD mounted. Verifying system folders...\r\n");
		fs_verify_directories();
	} else if (new_state == SD_STATE_MISSING) {
		LOG(FILESYSTEM, LOG_WARN, "SD removed! Clearing active paths...\r\n");
		// Clear your active playlist state, tell the audio DMA to stop, etc.
	}
}
