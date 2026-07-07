/*
 * Log.c
 *
 *  Created on: Jul 7, 2026
 *      Author: Dawid Sac
 */
#include "App/Log.h"

#include <stdio.h>

const char *const LogLevelStrings[] = { "DEBUG", "INFO", "WARN", "ERROR",
		"CRITICAL" };
const char *const SubsystemStrings[] = { "CORE", "FILESYSTEM", "AUDIO" };

static struct sLogState {
	bool globalLogOn;
	eLogLevel outputLevel;
} gLogData;

static struct sLogState gLogData;

void log_init(const bool globalOn, const eLogLevel level) {
	gLogData.globalLogOn = globalOn;
	gLogData.outputLevel = level;
}

void log_message(const eLogSubsystem sys, const eLogLevel level,
		const char *msg) {
	if (!gLogData.globalLogOn || level < gLogData.outputLevel)
		return;
	if (level >= NUM_LOG_LEVELS || sys >= NUM_SUBSYSTEMS)
		return;

	static char log_buffer[128];
	snprintf(log_buffer, sizeof(log_buffer), "[%s][%s] %s\n",
			LogLevelStrings[level], SubsystemStrings[sys], msg);

	printf("%s", log_buffer);

//	sd_write(log_buffer);
}
