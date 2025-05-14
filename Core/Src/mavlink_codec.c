/*
 * mavlink_codec.c
 *
 *  Created on: May 14, 2025
 *      Author: D
 *
 *
 */
#include "mavlink_codec.h"


mavlink_global_position_int_t global_position;
int32_t health=0, mav_alt=0, mav_lat=0, power=0, param1=0;	//reading variables

mavlink_sys_status_t sys_status;
mavlink_power_status_t pwr_status;
mavlink_command_long_t command_long;

uint8_t chan = MAVLINK_COMM_0;							//one data stream only
mavlink_status_t status;
mavlink_message_t msg;



/*		CALL THIS FUNCTION ITERATIVELY
 * @param byte:		Current byte of the message to decode
 *
 **/
void decode_mavlink_mssg(const unsigned char* byte)
{
	if (mavlink_parse_char(chan, byte, &msg, &status))
	{
		// msg.msgid, msg.seq, msg.compid, msg.sysid);
/*

		HAL_UART_Transmit(&huart3, msg.msgid, 24, 100);
		HAL_UART_Transmit(&huart3, "\n", 1, 100);*/

		// ... DECODE THE MESSAGE PAYLOAD HERE ...
		 switch(msg.msgid) {
			case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: // ID for GLOBAL_POSITION_INT
				mavlink_msg_global_position_int_decode(&msg, &global_position);
				mav_alt=mavlink_msg_global_position_int_get_alt(&msg);
				mav_lat=mavlink_msg_global_position_int_get_lat(&msg);
			break;
			case MAVLINK_MSG_ID_SYS_STATUS:
				mavlink_msg_sys_status_decode(&msg, &sys_status);
				health = mavlink_msg_sys_status_get_onboard_control_sensors_health(&msg);
			break;
			case MAVLINK_MSG_ID_POWER_STATUS:
				mavlink_msg_power_status_decode(&msg, &pwr_status);
				power = mavlink_msg_power_status_get_Vcc(&msg);
			break;



			case MAVLINK_MSG_ID_COMMAND_LONG:
				mavlink_msg_command_long_decode(&msg, &command_long);
				switch(command_long.command)
				{
					case MAV_CMD_DO_SET_MODE:
						param1=mavlink_msg_command_int_get_param1(&msg);
						break;
					default:break;
				}
			default:
			break;
			}
	}

}
