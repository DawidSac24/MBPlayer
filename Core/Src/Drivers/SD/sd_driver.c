/*
 * SD.c
 *
 *  Created on: Jul 8, 2026
 *      Author: Dawid Sac
 */

#include <Drivers/SD/sd_driver.h>
#include "Common/Utils/Log.h"

#include "sdmmc.h"

static struct {
	GPIO_TypeDef *detect_port;
	FATFS *fs;
	const char *root_filePath;
	sd_event_callback_t event_cb;
	uint16_t detect_pin;
	enum sd_state state;
} g_sd_context;

extern SD_HandleTypeDef hsd1;

void sd_setState(enum sd_state state);

void sd_init(const struct sd_init_struct *init_struct) {
	g_sd_context.detect_pin = init_struct->detect_pin;
	g_sd_context.detect_port = init_struct->detect_port;
	g_sd_context.fs = init_struct->fs;
	g_sd_context.root_filePath = init_struct->root_filePath;
	g_sd_context.event_cb = init_struct->event_cb;

	if (sd_getDetectPinState() == SD_PRESENT)
		g_sd_context.state = SD_STATE_INSERTED;
	else
		g_sd_context.state = SD_STATE_MISSING;
}

void sd_mount() {
	if (g_sd_context.state == SD_STATE_MISSING) {
		LOG(FILESYSTEM, LOG_ERROR,
				"Unable to mount SD card: SD card is missing\r\n");
		return;
	}
	if (g_sd_context.state == SD_STATE_MOUNTED) {
		LOG(FILESYSTEM, LOG_ERROR,
				"Unable to mount SD card: SD card is already mounted\r\n");
		return;
	}
	if (g_sd_context.state == SD_STATE_UNMOUNTED)
		MX_SDMMC1_SD_Init();

	FRESULT res = f_mount(g_sd_context.fs,
			(TCHAR const*) g_sd_context.root_filePath, 1);
	if (res != FR_OK) {
		LOG(FILESYSTEM, LOG_ERROR, "Unable to mount SD card: %d\r\n", res);
		g_sd_context.state = SD_STATE_ERROR;
		return;
	}

	sd_setState(SD_STATE_MOUNTED);
	LOG(FILESYSTEM, LOG_INFO, "SD card successfully mounted!\r\n");
}

void sd_unmount() {
	f_mount(0, (TCHAR const*) g_sd_context.root_filePath, 0);
	HAL_SD_DeInit(&hsd1);

	sd_setState(SD_STATE_UNMOUNTED);
}

void sd_process() {
	if (sd_getDetectPinState() == SD_PRESENT) {
		if (g_sd_context.state == SD_STATE_MISSING) {
			g_sd_context.state = SD_STATE_INSERTED;
		}
		if (g_sd_context.state == SD_STATE_INSERTED
				|| g_sd_context.state == SD_STATE_UNMOUNTED) {
			LOG(FILESYSTEM, LOG_INFO, "SD Card detected!\r\n");
			sd_mount();
			return;
		}
	} else {
		if (g_sd_context.state != SD_STATE_UNMOUNTED) {
			LOG(FILESYSTEM, LOG_INFO, "No SD Card detected!\r\n");
			sd_unmount();
		}
	}
}

__IO uint8_t sd_getDetectPinState() {
//	__IO uint8_t status = SD_PRESENT;
//
//	if (HAL_GPIO_ReadPin(g_sd_context.detect_port, g_sd_context.detect_pin)
//			!= GPIO_PIN_SET) {
//		status = SD_NOT_PRESENT;
//	}
//	return status;
	return SD_PRESENT;
}

void sd_setState(enum sd_state state) {
	g_sd_context.state = state;
	g_sd_context.event_cb(state);
}
