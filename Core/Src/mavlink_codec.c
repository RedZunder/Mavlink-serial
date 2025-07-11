/*
 * mavlink_codec.c
 *
 *  Created on: May 14, 2025
 *      Author: D
 *
 *
 */
#include "mavlink_codec.h"


//testing variables
int32_t param1=0, device_type=0;
uint8_t componentID=0;
mavlink_message_t hb_mssg;							//message struct for HEARTBEAT


uint8_t chan = MAVLINK_COMM_0;						//one data stream only
mavlink_status_t status;

//----------------------DECODED MESSAGES----------------------
mavlink_global_position_int_t global_position;
mavlink_sys_status_t sys_status;
mavlink_power_status_t pwr_status;
mavlink_message_t cmmd;
mavlink_heartbeat_t hb;
mavlink_attitude_t att;
mavlink_command_ack_t cmd_ack;
mavlink_battery_status_t bat_stat;
mavlink_collision_t col;
mavlink_autopilot_version_t	autopilot_v;
mavlink_obstacle_distance_t obstacle_distance;
mavlink_vfr_hud_t vfr_hud;

//-------------------------COMMANDS----------------------

mavlink_command_long_t command_long;




/* @brief This function decodes a Mavlink message when looping on each byte
 *	A MESSAGE MAY CONTAIN MORE THAN ONE MAVLINK MESSAGE
 *	e.g. [MAVLINK_HEARTBEAT, MAVLINK_GLOBAL_POSITION_INT, MAVLINK_SYS_STATUS]
 *
 * @param byte:		Current byte of the message to decode
 *	@retval			1 if success, 0 if message was not found
 **/
uint8_t decode_mavlink_mssg(const unsigned char* byte, mavlink_message_t* msg)
{
	if (mavlink_parse_char(chan, *byte, msg, &status))
	{
		// ... DECODE THE MESSAGE PAYLOAD HERE ...
		 switch(msg->msgid)
		 {
			case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
				mavlink_msg_global_position_int_decode(msg, &global_position);
				break;

			case MAVLINK_MSG_ID_SYS_STATUS:
				mavlink_msg_sys_status_decode(msg, &sys_status);
				break;

			case MAVLINK_MSG_ID_POWER_STATUS:
				mavlink_msg_power_status_decode(msg, &pwr_status);
				break;

			case MAVLINK_MSG_ID_HEARTBEAT:
				mavlink_msg_heartbeat_decode(msg, &hb);
			 	device_type=mavlink_msg_heartbeat_get_type(msg);
			 	break;

			case MAVLINK_MSG_ID_ATTITUDE:
				mavlink_msg_attitude_decode(msg, &att);
				break;

			case MAVLINK_MSG_ID_COMMAND_ACK:
				mavlink_msg_command_ack_decode(msg, &cmd_ack);
				break;

			case MAVLINK_MSG_ID_BATTERY_STATUS:
				mavlink_msg_battery_status_decode(msg, &bat_stat);
				break;

			case MAVLINK_MSG_ID_COLLISION:
				mavlink_msg_collision_decode(msg, &col);
				break;

			case MAVLINK_MSG_ID_AUTOPILOT_VERSION:
				mavlink_msg_autopilot_version_decode(msg, &autopilot_v);
				break;
			case MAVLINK_MSG_ID_OBSTACLE_DISTANCE:
				mavlink_msg_obstacle_distance_decode(msg, &obstacle_distance);
				break;
			case MAVLINK_MSG_ID_VFR_HUD:
				mavlink_msg_vfr_hud_decode(msg, &vfr_hud);
			default:return 0;break;

			}
	}
	return 1;

}


/* @brief This function will encode instructions into a Mavlink message
 *
 * @param conf_counter	Counter to keep track of attempts for sending the command
 * @param cmmd			Message struct to encode the message into
 * @param cmd_id		Command ID
 * @param rqst_id		Desired message ID in case the command is REQUEST_MESSAGE,
 * 						otherwise ignored
 * @retval 				Length of the encoded command structure, 0 if
 * 						message was not recognised
 **/

uint16_t encode_mavlink_cmd(const uint8_t* conf_counter, mavlink_message_t* cmmd, uint16_t cmd_id, uint16_t rqst_id)
{
	switch(cmd_id)
	{
		default:break;
		case MAV_CMD_REQUEST_MESSAGE:
			return	mavlink_msg_command_long_pack(SYS_ID, componentID, cmmd, TARGET_ID,
				componentID, MAV_CMD_REQUEST_MESSAGE, *conf_counter,
				rqst_id, 0, 0, 0, 0, 0, 1);
			//last '1' is to send message only to requester
			break;
		case MAV_CMD_NAV_PATHPLANNING:
			return	mavlink_msg_command_long_pack(SYS_ID, componentID, cmmd, TARGET_ID,
							componentID, MAV_CMD_NAV_PATHPLANNING, *conf_counter,
							2, 1, 0, 0, 0, 0, 0);
					//TEST enable obstacle avoidance, check with SYS_STATUS
			break;
	}
	return 0;

}


/* @brief 				This function packs a HEARTBEAT signal into the buffer
 *
 *	@param	buffer:		Empty array buffer
 * 	@retval 			Length of the HEARTBEAT message in bytes
 */
uint16_t mavlink_heartbeat(uint8_t* buffer)
{
	//prepare message
	mavlink_msg_heartbeat_pack(SYS_ID, MAV_COMP_ID_MISSIONPLANNER,
			&hb_mssg, MAV_TYPE_GCS,	MAV_AUTOPILOT_INVALID, 0, 0, MAV_STATE_UNINIT);

	//translate into bytes
	return mavlink_msg_to_send_buffer(buffer, &hb_mssg);
}








