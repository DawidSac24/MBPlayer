/*
 * track_record.h
 *
 *  Created on: 23 Jul 2026
 *      Author: majst
 */

#ifndef INC_APP_PLAYLIST_TRACK_RECORD_H_
#define INC_APP_PLAYLIST_TRACK_RECORD_H_

struct track_record {
	uint32_t id;
	uint16_t duration_s;
	uint16_t track_num;

	char *file_path;
	char *title;
	char *artist;
	char *album;
};

#endif /* INC_APP_PLAYLIST_TRACK_RECORD_H_ */
