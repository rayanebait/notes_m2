#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

void UART__init(uint32_t);
void UART__putc(uint8_t);
uint8_t UART__getc(void);

#endif
