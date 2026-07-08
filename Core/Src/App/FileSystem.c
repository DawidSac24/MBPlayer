/*
 * FileSystem.c
 *
 *  Created on: Jul 4, 2026
 *      Author: Dawid Sac
 */
#include <App/sd_driver.h>
#include "App/FileSystem.h"

#include "fatfs.h"

//static struct sFileSystemState {
//	char config_filepath[24];
//} gFileSystemData;

//bool fs_init() {
//	if (sd_mount())
//		return 0;
//
//	sd_openRoot();
//}

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

