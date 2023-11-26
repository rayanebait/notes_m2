
#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>

void
UART__init(uint32_t baud_rate);

int
UART__putchar(char c, FILE* _stream);

int
UART__getchar(FILE* _stream);

#endif /*UART_H*/