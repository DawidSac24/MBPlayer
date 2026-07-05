/*
 * FileSystem.c
 *
 *  Created on: Jul 4, 2026
 *      Author: Dawid Sac
 */
#include "App/FileSystem"
#include "fatfs.h"

void list_files(void) {
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
				printf("File: %s (size: %llu bytes)\r\n", fno.fname, fno.fsize);
			}
		}
		f_closedir(&dir);
	} else {
		printf("f_opendir failed with Error Code: %d\r\n", res);
	}
}

