#ifndef UART_H
#define UART_H

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>


void UART__init(uint32_t);
uint8_t UART__getc(void);
void UART__putc(uint8_t);


#endif /*UART_H*/