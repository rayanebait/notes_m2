#ifndef PROTO_H
#define PROTO_H

#include <stdint.h>

#define COMMAND_DDR 0
#define COMMAND_PORT 1
#define PORT_B 0
#define PORT_C 1
#define PORT_D 2

uint8_t send_command(uint8_t type, uint8_t port, uint8_t bit, uint8_t value);
uint8_t recv_command(uint8_t*, uint8_t*, uint8_t*, uint8_t*);

#endif
