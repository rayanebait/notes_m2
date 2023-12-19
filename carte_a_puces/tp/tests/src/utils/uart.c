#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "uart.h"
#include "ring_buffer.h"

#define RX_BUFFER_SIZE 100

int UART__putchar(char, FILE*);
int UART__getchar(FILE*);

uint8_t rx_raw_buffer[RX_BUFFER_SIZE];
struct ring_buffer rx_buffer;
char UART_receive_buffer;

static FILE mystdout = FDEV_SETUP_STREAM(UART__putchar, NULL, _FDEV_SETUP_WRITE);
static FILE mystderr = FDEV_SETUP_STREAM(UART__putchar, NULL, _FDEV_SETUP_WRITE);
static FILE mystdin = FDEV_SETUP_STREAM(NULL, UART__getchar, _FDEV_SETUP_READ);

void
UART__init(uint32_t baud_rate)
{
    // Set Baud Rate
    UBRR0 = (F_CPU/16/baud_rate) - 1;

    // enable RX, TX for UART0
    UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);  // 8 bit data format

    ring_buffer__init(&rx_buffer, rx_raw_buffer, RX_BUFFER_SIZE);

    stdout = &mystdout;
    stdin = &mystdin;
    stderr = &mystderr;
}

ISR(USART_RX_vect) {
    UART_receive_buffer = UDR0;
    if(UART_receive_buffer == '\r'){
        ring_buffer__push(&rx_buffer, '\n');
    } else {
        ring_buffer__push(&rx_buffer, UART_receive_buffer);
    }
}

int UART__putchar(char c, FILE* _stream) {
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}

int UART__getchar(FILE* _stream) {
    uint8_t data;
    while (ring_buffer__pop(&rx_buffer, &data) == 1);
    return (int) data; 
}

