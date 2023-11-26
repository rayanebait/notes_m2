#include <stdint.h>
#include "uart.h"

int main(void)
{
    UART__init(9600);

    for (;;) {
        UART__putc(UART__getc());
    }
}
