/*
 * Log.h
 *
 *  Created on: Jul 7, 2026
 *      Author: Dawid Sac
 */

#ifndef INC_APP_LOG_H_
#define INC_APP_LOG_H_

#include <stdbool.h>
#include <stdio.h>

#define LOG_BUFFER_SIZE 128

#define LOG(sys, level, format, ...) do { \
    char _log_buf[128]; \
    snprintf(_log_buf, sizeof(_log_buf), format, ##__VA_ARGS__); \
    log_message(sys, level, _log_buf); \
} while(0)

enum log_subsystem {
	CORE, ALLOCATOR, CONTAINER, FILESYSTEM, AUDIO, NUM_SUBSYSTEMS
};
enum log_level {
	LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_CRITICAL, NUM_LOG_LEVELS
};

extern const char *const LogLevelStrings[];
extern const char *const SubsystemStrings[];

void log_init(const bool globalOn, const enum log_level level);

void log_message(const enum log_subsystem sys, const enum log_level level,
		const char *msg);

void log_globalOn();
void log_globalOff();
bool log_isGlobalOn();

enum log_level log_getOutputLevel();
void log_setOutputLevel(const enum log_level level);

#endif /* INC_APP_LOG_H_ */
