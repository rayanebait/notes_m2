#include <stdint.h>
#include <stdio.h>

#include "proto.h"

uint8_t validate(uint8_t, uint8_t, uint8_t, uint8_t);

/**
 * Returns 0 if the configuration is valid,
 * another value otherwise
 */
uint8_t validate(uint8_t type, uint8_t port, uint8_t bit, uint8_t value) {
    if (type > 1) {
        // Invalid type
        return 1;
    }
    if (value > 1) {
        // Invalid bit value
        return 2;
    }

    if (port > 2) {
        // Invalid port value
        return 3;
    }

    if (port == PORT_D && bit > 7) {
        // Invalid bit value
        return 4;
    }

    if (port != PORT_D && bit > 5) {
        // Invalid bit value
        return 5;
    }

    return 0;
}


/*
 * Command format:
 * 0b0CPPBBBX
 * C (command): 1 for set_port, 0 for set_ddr
 * PP : port number (0 <= port_number <= 2)
 * BBB : bit number (0 <= bit number <= 7)
 * X : bit value
 */
uint8_t send_command(uint8_t type, uint8_t port, uint8_t bit, uint8_t value) {
    if (validate(type, port, bit, value)) {
        return 1;
    }

    uint8_t command = (type << 6) | (port << 4) | (bit << 1) | value;
    putchar(command);
    return 0;
}

uint8_t recv_command(uint8_t *type, uint8_t *port, uint8_t *bit, uint8_t *value) {
    uint8_t command = getchar();
    *type = (command >> 6) & 1;
    *port = (command >> 4) & 0x3;
    *bit = (command >> 1) & 0x7;
    *value = command & 1;

    return validate(*type, *port, *bit, *value);
}
