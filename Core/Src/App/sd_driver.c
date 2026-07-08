/*
 * SD.c
 *
 *  Created on: Jul 8, 2026
 *      Author: Dawid Sac
 */

#include <App/sd_driver.h>
#include "App/Log.h"

#include "sdmmc.h"

static struct sSdState {
	uint16_t detect_pin;
	GPIO_TypeDef *detect_port;
	FATFS *fs;
	const char *root_filePath;
	eSdState state;
} gSdData;

extern SD_HandleTypeDef hsd1;

void sd_init(uint16_t detect_pin, GPIO_TypeDef *detect_port, FATFS *fs,
		const char *root_filePath) {
	gSdData.detect_pin = detect_pin;
	gSdData.detect_port = detect_port;
	gSdData.fs = fs;
	gSdData.root_filePath = root_filePath;
	if (sd_getDetectPinState() == SD_PRESENT)
		gSdData.state = SD_STATE_INSERTED;
	else
		gSdData.state = SD_STATE_MISSING;
}

void sd_mount() {
	if (gSdData.state == SD_STATE_MISSING) {
		LOG(FILESYSTEM, LOG_ERROR,
				"Unable to mount SD card: SD card is missing\r\n");
		return;
	}
	if (gSdData.state == SD_STATE_MOUNTED) {
		LOG(FILESYSTEM, LOG_ERROR,
				"Unable to mount SD card: SD card is already mounted\r\n");
		return;
	}
	if (gSdData.state == SD_STATE_UNMOUNTED)
		MX_SDMMC1_SD_Init();

	FRESULT res = f_mount(gSdData.fs, (TCHAR const*) gSdData.root_filePath, 1);
	if (res != FR_OK) {
		LOG(FILESYSTEM, LOG_ERROR, "Unable to mount SD card: %d\r\n", res);
		gSdData.state = SD_STATE_ERROR;
		return;
	}

	gSdData.state = SD_STATE_MOUNTED;
	LOG(FILESYSTEM, LOG_INFO, "SD card successfully mounted!\r\n");
}

void sd_unmount() {
	f_mount(0, (TCHAR const*) gSdData.root_filePath, 0);
	HAL_SD_DeInit(&hsd1);

	gSdData.state = SD_STATE_UNMOUNTED;
}

void sd_process() {
	if (sd_getDetectPinState() == SD_PRESENT) {
		if (gSdData.state == SD_STATE_MISSING) {
			gSdData.state = SD_STATE_INSERTED;
		}
		if (gSdData.state == SD_STATE_INSERTED
				|| gSdData.state == SD_STATE_UNMOUNTED) {
			LOG(FILESYSTEM, LOG_INFO, "SD Card detected!\r\n");
			sd_mount();
			return;
		}
	} else {
		if (gSdData.state != SD_STATE_UNMOUNTED) {
			LOG(FILESYSTEM, LOG_INFO, "No SD Card detected!\r\n");
			sd_unmount();
		}
	}
}

__IO uint8_t sd_getDetectPinState() {
	__IO uint8_t status = SD_PRESENT;

	if (HAL_GPIO_ReadPin(gSdData.detect_port, gSdData.detect_pin)
			!= GPIO_PIN_SET) {
		status = SD_NOT_PRESENT;
	}
	return status;
}
