/*
 * SD.h
 *
 *  Created on: Jul 8, 2026
 *      Author: Dawid Sac
 */

#ifndef INC_APP_SD_DRIVER_H_
#define INC_APP_SD_DRIVER_H_

#include "fatfs.h"
#include <stdbool.h>

typedef enum {
	SD_STATE_MISSING, SD_STATE_INSERTED, SD_STATE_UNMOUNTED, SD_STATE_MOUNTED, SD_STATE_ERROR
} eSdState;

void sd_init(uint16_t detect_pin, GPIO_TypeDef *detect_port, FATFS *fs,
		const char *root_filePath);

void sd_mount();
void sd_unmount();
bool sd_isMounted();

void sd_process();

__IO uint8_t sd_getDetectPinState();
#endif /* INC_APP_SD_DRIVER_H_ */
