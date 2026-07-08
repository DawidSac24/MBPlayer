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

static struct {
	bool globalLogOn;
	eLogLevel outputLevel;
} gLog_context;

void log_record(const eLogSubsystem sys, const eLogLevel level, const char *msg);

void log_init(const bool globalOn, const eLogLevel level) {
	gLog_context.globalLogOn = globalOn;
	gLog_context.outputLevel = level;


}

void log_message(const eLogSubsystem sys, const eLogLevel level,
		const char *msg) {
	if (!gLog_context.globalLogOn || level < gLog_context.outputLevel)
		return;
	if (level >= NUM_LOG_LEVELS || sys >= NUM_SUBSYSTEMS)
		return;

	static char log_buffer[LOG_BUFFER_SIZE];
	snprintf(log_buffer, sizeof(log_buffer), "[%s][%s] %s\n",
			LogLevelStrings[level], SubsystemStrings[sys], msg);

	printf("%s", log_buffer);

	log_record(sys, level, msg);
}

void log_record(const eLogSubsystem sys, const eLogLevel level, const char *msg) {
	return;
}
