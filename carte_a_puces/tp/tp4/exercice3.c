#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "uart.h"
#include "ring_buffer.h"

#define BAUD_RATE 9600

#define LEN_BUF 100


ISR(USART_RX_vect){
    ring_buffer__push(&rb, UDR0);
    PORTD ^= _BV(PORTD4);
}

int main(void){
    DDRD |= _BV(DDD4);

    /* Set the sleep mode to IDLE so the USART can continue working*/
    set_sleep_mode(SLEEP_MODE_IDLE); 

    uint8_t buf[LEN_BUF];
    ring_buffer__init(&rb, buf, LEN_BUF);

    UART__init(BAUD_RATE);

    /*Enable interrupts*/
    sei();

    while(1){
        sleep_mode();
        uint8_t c = UART__getc();
        UART__putc(c);
    }
}
