#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "proto.h"
#include "uart.h"

#include <stdio.h>

void exec_command(uint8_t, uint8_t, uint8_t, uint8_t);

/*
 * 0x24 : DDRB
 * 0x25 : PORTB
 * 0x27 : DDRC
 * 0x28 : PORTC
 * 0x2A : DDRD
 * 0x2B : PORTD
 */

void exec_command(uint8_t type, uint8_t port, uint8_t bit, uint8_t value) {
    uint8_t reg_value = type;
    if (port == PORT_B) {
        reg_value += 0x24;
    } else if (port == PORT_C) {
        reg_value += 0x27;
    } else if (port == PORT_D) {
        reg_value += 0x2A;
    } else {
        // Invalid port
        return;
    }

    uint8_t* reg = (uint8_t*) reg_value;

    if (value) {
        *reg |= _BV(bit);
    }
    else {
        *reg &= ~_BV(bit);
    }
}

int main(void)
{
    UART__init(BAUD_RATE);
    sei();

    uint8_t type = 0;
    uint8_t port = 0;
    uint8_t bit = 0;
    uint8_t value = 0;
    uint8_t res;

    while (1) {
        res = recv_command(&type, &port, &bit, &value);
        if (res == 0) {
            exec_command(type, port, bit, value);
        }
    }
}
