# Mavlink-serial
Interface in C for Mavlink based on STM32


## Concept
The idea is to design an interface where the user can read information from and send data to a drone ([type quadrotor](https://mavlink.io/en/messages/common.html#MAV_TYPE)), using STM32 as the brains and the ELRS Micro TX as the sender/receiver module.

Using the [Mavlink v2 library for C](https://mavlink.io/en/mavgen_c/).


- [x] <details open><summary>Basic decoding function</summary>
  `decode_mavlink_mssg` decodes [messages of type Mavlink](https://mavlink.io/en/messages/common.html)</details>
  - [ ] Finish decoding function
- [ ] Basic encoding function
  - [ ] Finish encoding function  
