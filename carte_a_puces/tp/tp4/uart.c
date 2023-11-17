#include "ring_buffer.h"
#include "uart.h"


void UART__init(uint32_t baud_rate){
    uint32_t ubrr = F_CPU/(16UL * baud_rate) - 1;
    /*Set baud rate*/
    UBRR0L = (unsigned char) (ubrr);
    UBRR0H = (unsigned char) (ubrr>>8);

    /*Set size of the data portion of a frame*/
    UCSR0C |= _BV(UCSZ00) | _BV(UCSZ01);

    /*Enable transmitter and receiver */
    UCSR0B |= _BV(TXEN0) | _BV(RXEN0);

    /*Enable receive complete interrupt flag*/
    UCSR0B |= _BV(RXCIE0);
}

uint8_t UART__getc(void){
    /*save context*/
    unsigned char sreg = SREG;
    /*disable interrupts*/
    cli();

    uint8_t data;
    if(ring_buffer__pop(&rb, &data)){
        /*empty buffer*/
        SREG = sreg;
        sei();
        return 0;
    }
    SREG = sreg;
    sei();
    return data;
}

void UART__putc(uint8_t c){
    /*Waiting for data transmit register to be empty*/
    while(!(UCSR0A & _BV(UDRE0))) ;

    /*Fill the buffer so it can be shifted*/
    UDR0 = c;
    while(!(UCSR0A & _BV(TXC0))) ;
}

