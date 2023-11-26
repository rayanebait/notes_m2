#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>

#include "ring_buffer.h"
#include "uart.h"

static FILE STDOUT = FDEV_SETUP_STREAM(UART__putc, NULL, _FDEV_SETUP_WRITE);
static FILE STDIN = FDEV_SETUP_STREAM(NULL, UART__getc, _FDEV_SETUP_READ);

#define BAUD_RATE 9600
#define LEN_BUF 100

ISR(USART_RX_vect){
    ring_buffer__push(&rb, UDR0);
    PORTB ^= _BV(PORTB5);
}

int main(void){
    DDRB |= _BV(DDB5);
    stdout = &STDOUT;
    stdin = &STDIN;

    /* Set the sleep mode to IDLE so the USART can continue working*/
    set_sleep_mode(SLEEP_MODE_IDLE); 

    uint8_t buf[LEN_BUF];
    ring_buffer__init(&rb, buf, LEN_BUF);

    UART__init(BAUD_RATE);

    sei();

    while(1){
        sleep_mode();
        char str[LEN_BUF];
        gets(str);
        printf("Caractère entré:%s, %s",str, rb.buffer+rb.head);
        /*devrait flush le ring buffer a partir de head jusqu'a tail?*/
    }
}