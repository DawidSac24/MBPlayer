/*
 * FileSystem
 *
 *  Created on: Jul 4, 2026
 *      Author: Dawid Sac
 */

#ifndef INC_APP_FILESYSTEM_H_
#define INC_APP_FILESYSTEM_H_

#include "App/sd_driver.h"

#include "App/sd_driver.h"
#include <stdbool.h>

void fs_init(SD_initStruct *init_struct);

void fs_list_files();

#endif /* INC_APP_FILESYSTEM_H_ */
