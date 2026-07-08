/*
 * SD.c
 *
 *  Created on: Jul 8, 2026
 *      Author: Dawid Sac
 */

#include <App/sd_driver.h>
#include "App/Log.h"

static struct sSdState {
	uint16_t detect_pin;
	GPIO_TypeDef *detect_port;
	FATFS *fs;
	const char *root_filePath;
	eSdState state;
} gSdData;

static volatile uint8_t sd_interrupt_flag = 0;
static volatile uint32_t sd_interrupt_time = 0;

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

bool sd_mount() {
	if (gSdData.state != SD_STATE_INSERTED) {
		LOG(FILESYSTEM, LOG_ERROR,
				"Unable to mount SD card: SD card is missing\r\n");
		return false;
	}
	FRESULT res = f_mount(gSdData.fs, (TCHAR const*) gSdData.root_filePath, 1);
	if (res != FR_OK) {
		LOG(FILESYSTEM, LOG_ERROR, "Unable to mount SD card: %d\n", res);
		gSdData.state = SD_STATE_ERROR;
		return false;
	}
	gSdData.state = SD_STATE_MOUNTED;
	LOG(FILESYSTEM, LOG_INFO, "SD card successfully mounted!\r\n");

	return true;
}

void sd_unmount() {
	f_mount(0, (TCHAR const*) SDPath, 0);
	gSdData.state = SD_STATE_UNMOUNTED;
}

void sd_process() {
	if (sd_interrupt_flag == 0)
		return;

	if ((HAL_GetTick() - sd_interrupt_time) < 100)
		return;

	if (sd_getDetectPinState()
			== SD_PRESENT) {
		gSdData.state = SD_STATE_INSERTED;
		sd_mount();
	} else {
		gSdData.state = SD_STATE_MISSING;
		sd_unmount();
	}

	sd_interrupt_flag = 0;
}

__IO uint8_t sd_getDetectPinState() {
	__IO uint8_t status = SD_PRESENT;

	if (HAL_GPIO_ReadPin(SD_DET_GPIO_Port, SD_DET_Pin) != GPIO_PIN_SET) {
		status = SD_NOT_PRESENT;
	}
	return status;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == gSdData.detect_pin) {
		sd_interrupt_flag = 1;
		sd_interrupt_time = HAL_GetTick();
	}
}
