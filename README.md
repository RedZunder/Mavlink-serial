# Mavlink-serial
Interface in C for Mavlink based on STM32


## Concept
The idea is to design an interface where the user can read information from and send data to a drone, using STM32 as the brains and the ELRS Micro TX as the sender/receiver module.

Using the [Mavlink v2 library for C](https://mavlink.io/en/mavgen_c/).


- [x] <details><summary>Basic decoding function</summary>
  `decode_mavlink_mssg`
</details>

  - [ ] Finish decoding function
- [ ] Basic encoding function
  - [ ] Finish encoding function  
