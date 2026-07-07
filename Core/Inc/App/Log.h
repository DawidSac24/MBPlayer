/*
 * Log.h
 *
 *  Created on: Jul 7, 2026
 *      Author: Dawid Sac
 */

#ifndef INC_APP_LOG_H_
#define INC_APP_LOG_H_

#include <stdbool.h>

typedef enum {
	CORE, FILESYSTEM, AUDIO, NUM_SUBSYSTEMS
} eLogSubsystem;
typedef enum {
	LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_CRITICAL, NUM_LOG_LEVELS
} eLogLevel;

extern const char *const LogLevelStrings[];
extern const char *const SubsystemStrings[];

void log_init(const bool globalOn, const eLogLevel level);

void log_message(const eLogSubsystem sys, const eLogLevel level,
		const char *msg);

void log_globalOn();
void log_globalOff();
bool log_isGlobalOn();

enum eLogLevel log_getOutputLevel();
void log_setOutputLevel(const enum eLogLevel level);

#endif /* INC_APP_LOG_H_ */
