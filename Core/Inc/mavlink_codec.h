/*
 * mavlink_codec.h
 *
 *  Created on: May 14, 2025
 *      Author: D
 */
#include "main.h"
#ifndef INC_MAVLINK_CODEC_H_
#define INC_MAVLINK_CODEC_H_

#define SYS_ID 255		//GSC ID
#define TARGET_ID 1		//quadrotor ID
#define VFR_HUD 74		//message ID for HUD metrics
#include "stdint.h"
#include "string.h"
#include "all/mavlink.h"





uint8_t decode_mavlink_mssg(const unsigned char* byte, mavlink_message_t* msg);

uint16_t encode_mavlink_cmd(const uint8_t* conf_counter, mavlink_message_t* cmmd);

uint16_t mavlink_heartbeat(uint8_t* buffer);








#endif /* INC_MAVLINK_CODEC_H_ */
