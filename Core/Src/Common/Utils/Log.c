/*
 * Log.c
 *
 *  Created on: Jul 7, 2026
 *      Author: Dawid Sac
 */
#include "Common/Utils/Log.h"

#include <stdio.h>

const char *const LogLevelStrings[] = { "DEBUG", "INFO", "WARN", "ERROR",
		"CRITICAL" };
const char *const SubsystemStrings[] = { "CORE", "FILESYSTEM", "AUDIO" };

static struct {
	bool globalLogOn;
	enum log_level outputLevel;
} g_log_context;

void log_record(const enum log_subsystem sys, const enum log_level level, const char *msg);

void log_init(const bool globalOn, const enum log_level level) {
	g_log_context.globalLogOn = globalOn;
	g_log_context.outputLevel = level;

}

void log_message(const enum log_subsystem sys, const enum log_level level,
		const char *msg) {
	if (!g_log_context.globalLogOn || level < g_log_context.outputLevel)
		return;
	if (level >= NUM_LOG_LEVELS || sys >= NUM_SUBSYSTEMS)
		return;

	static char log_buffer[LOG_BUFFER_SIZE];
	snprintf(log_buffer, sizeof(log_buffer), "[%s][%s] %s\n",
			LogLevelStrings[level], SubsystemStrings[sys], msg);

	printf("%s", log_buffer);

	log_record(sys, level, msg);
}

void log_record(const enum log_subsystem sys, const enum log_level level, const char *msg) {
	return;
}
