#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>

#include "uart.h"


int main(void)
{
    DDRB |= _BV(DDB5);
    PORTB &= ~_BV(PORTB5);

    UART__init(9600);

    while (1) {
        uint8_t c = UART__getc();
        UART__putc(c);
        PORTB |= _BV(PORTB5);
        _delay_ms(5000);
        PORTB &= ~_BV(PORTB5);
    }
}
