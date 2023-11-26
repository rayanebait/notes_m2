#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <avr/sleep.h>
#include "proto.h"
#include "uart.h"
#define BLINK_RESOLUTION 31250
#define BLINK_PATTERN 88569621UL

void timer1_init(void);

volatile uint8_t counter = 0;

ISR(TIMER1_COMPA_vect) {
    if (BLINK_PATTERN & (1UL << counter)) {
        send_command(COMMAND_PORT, PORT_B, 5, 1);
    }
    else {
        send_command(COMMAND_PORT, PORT_B, 5, 0);
    }

    // Increment counter
    counter++;
    if (counter >= 35) {
        counter = 0;
    }
}


void timer1_init() {
    // Counter resolution
    OCR1A = BLINK_RESOLUTION;

    // Enable COMPA interrupt
    TIMSK1 |= _BV(OCIE1A);

    // Mode CTC with /256 prescaler
    TCCR1B |= _BV(WGM12) | _BV(CS12);
}



int main(void)
{
    DDRB |= _BV(DDB5);
    PORTB &= ~_BV(PORTB5);

    UART__init(BAUD_RATE);
    timer1_init();

    sei();

    send_command(COMMAND_DDR, PORT_B, 5, 1);

    while (1) {
        set_sleep_mode(SLEEP_MODE_IDLE);
        sleep_mode();
    }
}
