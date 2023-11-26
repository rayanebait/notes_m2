#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "ring_buffer.h"

#define RX_BUFFER_SIZE 4
uint8_t rx_raw_buffer[RX_BUFFER_SIZE];
struct ring_buffer rx_buffer;

void
UART__init(uint32_t baud_rate)
{
    // Set Baud Rate
    UBRR0 = (F_CPU/16/baud_rate) - 1;

    // enable RX, TX for UART0
    UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);  // 8 bit data format

    ring_buffer__init(&rx_buffer, rx_raw_buffer, RX_BUFFER_SIZE);
}

uint8_t
UART__getc()
{
    uint8_t data;
    while (ring_buffer__pop(&rx_buffer, &data) == 1);
    return data;
}

void
UART__putc(uint8_t c)
{
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

ISR(USART_RX_vect) {
    ring_buffer__push(&rx_buffer, UDR0);
}
