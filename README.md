# Mavlink-serial
*----------------------------------------Control in C for Mavlink based on STM32----------------------------------------*


# First steps
We began by trying to get the ELRS Micro TX with a laptop, connecting it to the same local Wi-Fi network as the drone. We could successfully read information from the drone via **Mission Planner**. 
The next step is to develop an interface between user and drone, through STM32F7.

## Concept
The idea is to design an interface where the user can read information from and send commands to a drone ([type quadrotor](https://mavlink.io/en/messages/common.html#MAV_TYPE)), using STM32F7 as the brains and the ELRS Micro TX as the sender/receiver module. We power the MicroTX from the laptop, and connect it via UART to the STM32F7. For now, the STM32 will handle all operations. Later, we will use a Python script to send the user's input to the STM32 board.





We will use the [Mavlink v2 library for C](https://mavlink.io/en/mavgen_c/), and its packet structure is defined [here](https://mavlink.io/en/guide/serialization.html#mavlink2_packet_format).


- [x] Basic decoding function
  - [x] `decode_mavlink_mssg` decodes [messages of type Mavlink](https://mavlink.io/en/messages/common.html)</details>
  - [ ] Finish decoding function
  - [ ] Fix live decoding (Half-duplex)
- [x] HEARTBEAT protocol 
- [x] Basic command encoding function
  - [ ] Finish encoding function  



## Heartbeat
The `mavlink_establish_conversation` function creates and transmits the HEARTBEAT message with basic information of the device. This is ensured to be called every 1 second thanks to a constant timer (TIM4) set with interrupt. 
The `SYSTEM_ID` is 255, as [recommended by Mavlink](https://mavlink.io/en/messages/common.html#MAV_COMPONENT), and the `COMPONENT_ID` is `MAV_COMP_ID_MISSIONPLANNER(190)`.
The function saves the `HEARTBEAT` information into a `mavlink_message_t` struct, and then encodes the message in bytes into the `uint8_t* buffer`. Finally it's transmitted via UART in interrupt mode.

## Decoding messages
The `decode_mavlink_mssg` will be called inside a loop, taking every byte from a live transmission and translate it to a mavlink message structure. 

Currently the [possible messages](https://mavlink.io/en/messages/common.html) are **(*IN PROGRESS*)**:

<details>
<summary>Messages</summary>
  
- MAVLINK_MSG_ID_ATTITUTE
- MAVLINK_MSG_ID_GLOBAL_POSITION_INT
- MAVLINK_MSG_ID_SYS_STATUS
- MAVLINK_MSG_ID_POWER_STATUS
- MAVLINK_MSG_ID_AUTOPILOT_VERSION
- MAVLINK_MSG_ID_COMMAND_ACK
- MAVLINK_MSG_ID_HEARTBEAT
- MAVLINK_MSG_ID_BATTERY_STATUS
- MAVLINK_MSG_ID_COLLISION
</details>

## [Encoding command](https://mavlink.io/en/services/command.html#MAV_CMD)

We can use `COMMAND_INT` for [position related commands](https://mavlink.io/en/messages/common.html#COMMAND_INT) that need a frame `MAV_FRAME`, or `COMMAND_LONG` for other commands.
For a test, using the function `mavlink_msg_command_long_pack` with command ID `COMMAND_LONG` and `MAV_CMD_REQUEST_MESSAGE` as the type of command, we will request `VFR_HUD(74)` information from the quadrotor.












