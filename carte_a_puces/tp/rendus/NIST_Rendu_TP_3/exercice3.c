#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <stdint.h>

volatile char signal[35] = "=.=.=...===.===.===...=.=.=........";
volatile uint8_t signal_ctr = 0;

ISR(WDT_vect)
{
    if (signal[signal_ctr] == '=')
    {
        PORTB |= _BV(PORTB5);
    }
    else if (signal[signal_ctr] == '.')
    {
        PORTB &= ~_BV(PORTB5);
    }
    signal_ctr = (signal_ctr + 1) % 35;
}

int main(void)
{
    cli(); // Make sure interrupts are disabled during setup

    // Power management section
    power_all_disable();                 // Disable all modules
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set the sleep mode to powerdown

    // LED setup section
    DDRB |= _BV(DDB5); // Set DDRB to 1 for the LED

    // This setup was inspired by this StackExchange response :
    // https://electronics.stackexchange.com/a/74850
    // Set up Watch Dog Timer for Inactivity
    WDTCSR |= (_BV(WDCE) | _BV(WDE)); // Enable the WD Change Bit
    WDTCSR = _BV(WDIE) |              // Enable WDT Interrupt
             _BV(WDP2) | _BV(WDP0);   // Set Timeout to ~0.5 seconds

    while (1)
    {
        // Sleep between every interruption
        // Documentation recommends to enable and disable the SE bit every time
        // In power down mode, we can also disable the Brown Out detector
        cli(); // Prevent being interrupted during the sleep setup
        sleep_enable();
        sleep_bod_disable();
        sei(); // Enable interupts
        sleep_cpu();
        sleep_disable();
    }
}