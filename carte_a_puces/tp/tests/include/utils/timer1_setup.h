#ifndef TIMER1_SETUP_H
#define TIMER1_SETUP_H

#include <avr/io.h>
#include <stdint.h>


void Write_Timer1(unsigned int);

void Timer1_overflow_setup(uint8_t prescaler);

#endif /*TIMER1_SETUP_H*/