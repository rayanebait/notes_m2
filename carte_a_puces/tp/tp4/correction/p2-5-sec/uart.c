#include <avr/io.h>
#include <avr/sfr_defs.h>

#include "uart.h"

void
UART__init(uint32_t baud_rate)
{
    // Set Baud Rate
    UBRR0 = (F_CPU/16/baud_rate) - 1;

    // enable RX, TX for UART0
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // 8 bit data format
}

uint8_t
UART__getc()
{
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}

void
UART__putc(uint8_t c)
{
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}
