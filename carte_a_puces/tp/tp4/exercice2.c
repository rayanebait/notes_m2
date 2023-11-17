#include <avr/io.h>
#include <util/delay.h>

#include "./uart.h"

#define BAUD_RATE 9600
#define BLINK_DELAY_MS 2000

void delay500ms(){
    PORTB ^= _BV(PORTB5);
	_delay_ms(BLINK_DELAY_MS);
}

int main(void){
    // SPCR |= _BV(MSTR) | _BV(SPE);
    DDRB |= _BV(DDB5);

    UART__init(BAUD_RATE);
    
    for(;;){
        uint8_t c = UART__getc();
        delay500ms();
        UART__putc(c);
    }
}