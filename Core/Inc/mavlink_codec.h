/*
 * mavlink_codec.h
 *
 *  Created on: May 14, 2025
 *      Author: D
 */

#ifndef INC_MAVLINK_CODEC_H_
#define INC_MAVLINK_CODEC_H_

#define SYSID 255
#include "stdint.h"
#include "string.h"
#include "all/mavlink.h"





void decode_mavlink_mssg(const unsigned char* byte);

void encode_mavlink_mssg(const unsigned char* byte);

void broadcast_heartbeat();

#endif /* INC_MAVLINK_CODEC_H_ */
