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

enum sd_state {
	SD_STATE_MISSING,
	SD_STATE_INSERTED,
	SD_STATE_UNMOUNTED,
	SD_STATE_MOUNTED,
	SD_STATE_ERROR
};

typedef void (*sd_event_callback_t)(enum sd_state new_state);

struct sd_init_struct {
	GPIO_TypeDef *detect_port;
	uint16_t detect_pin;
	FATFS *fs;
	const char *root_filePath;
	sd_event_callback_t event_cb;
};

void sd_init(const struct sd_init_struct *init_struct);

void sd_mount();
void sd_unmount();
bool sd_isMounted();

void sd_process();

__IO uint8_t sd_getDetectPinState();
#endif /* INC_APP_SD_DRIVER_H_ */
