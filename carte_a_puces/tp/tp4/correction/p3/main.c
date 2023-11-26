#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "uart.h"


int main(void)
{
    UART__init(9600);
    sei();

    for (;;) {
        UART__putc(UART__getc());
    }
}
