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

//reading variables
int32_t health=0, mav_alt=0, mav_lat=0, power=0, param1=0, device_type=0;
uint8_t componentID=0;
mavlink_sys_status_t sys_status;
mavlink_power_status_t pwr_status;
mavlink_command_long_t command_long;

uint8_t chan = MAVLINK_COMM_0;							//one data stream only
mavlink_status_t status;								//message status
mavlink_message_t cmmd;									//decoded message



/* @brief This function decodes a Mavlink message when looping on each byte
 *	A MESSAGE MAY CONTAIN MORE THAN ONE MAVLINK MESSAGE
 *	e.g. [MAVLINK_HEARTBEAT, MAVLINK_GLOBAL_POSITION_INT, MAVLINK_SYS_STATUS]
 *
 * @param byte:		Current byte of the message to decode
 *
 **/
void decode_mavlink_mssg(const unsigned char* byte, mavlink_message_t msg)
{
	if (mavlink_parse_char(chan, byte, &msg, &status))
	{
		// msg.msgid, msg.seq, msg.compid, msg.sysid);
/*

		HAL_UART_Transmit(&huart3, msg.msgid, 24, 100);
		HAL_UART_Transmit(&huart3, "\n", 1, 100);*/

		// ... DECODE THE MESSAGE PAYLOAD HERE ...
		 switch(msg.msgid) {
		 case MAVLINK_MSG_ID_HEARTBEAT:
			 	device_type=mavlink_msg_heartbeat_get_type(&msg);
			 break;



			case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
				mavlink_msg_global_position_int_decode(&msg, &global_position);
				mav_alt=global_position.alt;
				mav_lat=global_position.lat;

			break;

			case MAVLINK_MSG_ID_SYS_STATUS:
				mavlink_msg_sys_status_decode(&msg, &sys_status);
				health = sys_status.onboard_control_sensors_health;
			break;

			case MAVLINK_MSG_ID_POWER_STATUS:
				mavlink_msg_power_status_decode(&msg, &pwr_status);
				power = pwr_status.Vcc;
			break;


			default:
			break;
			}
	}

}




/* @brief This function will encode instructions into a Mavlink message
 *
 * @param conf_counter	Coutner to keep track of attempts for sending the command
 *
 **/

void encode_mavlink_mssg(uint8_t conf_counter)
{
	//EXAMPLE: send command to request message of VFR_HUD(74)
	mavlink_msg_command_long_pack(SYS_ID, componentID, &cmmd, TARGET_ID,
			componentID, MAV_CMD_REQUEST_MESSAGE, &conf_counter,
			VFR_HUD, 0, 0, 0, 0, 0, 1);		//last '1' for target address

//same as pack but with premade struct
	//mavlink_msg_command_long_encode(system_id, component_id, msg, command_long)

}






/* @brief 	This function sends a HEARTBEAT signal to the buffer
 * 			and should be called every second(1Hz)
 *	@param	buffer:		uint8_t Empty array buffer
 *	@param	msg:		Empty Maavlink message struct for the message
 * 	@return 	Length of the HEARTBEAT byte message
 */
uint16_t broadcast_heartbeat(uint8_t* buffer, mavlink_message_t* msg)
{
	//prepare message
	mavlink_msg_heartbeat_pack(SYS_ID, MAV_COMP_ID_MISSIONPLANNER,
			msg, MAV_TYPE_GCS,	MAV_AUTOPILOT_INVALID, 0, 0, MAV_STATE_UNINIT);
	//prepare bytes
	return mavlink_msg_to_send_buffer(buffer, msg);
}









