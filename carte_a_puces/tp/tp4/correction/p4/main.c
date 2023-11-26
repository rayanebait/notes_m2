#include <avr/interrupt.h>
#include <stdio.h>

#include "uart.h"

int main(void)
{
    UART__init(9600);
    sei();

    printf("Hello World !\r\n");
    while (1) {
        printf("Caractère reçu: %c\r\n", getchar());
    }
}
