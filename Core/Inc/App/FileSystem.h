/*
 * FileSystem
 *
 *  Created on: Jul 4, 2026
 *      Author: Dawid Sac
 */

#ifndef INC_APP_FILESYSTEM_H_
#define INC_APP_FILESYSTEM_H_

#include "Drivers/sd_driver.h"

#include <stdbool.h>

void fs_init(struct sd_init_struct *init_struct);
void fs_process();

void fs_list_files();

#endif /* INC_APP_FILESYSTEM_H_ */
