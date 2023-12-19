#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define BLINK_PATTERN 88569621UL
#define BLINK_PATTERN_LEN 35

volatile uint8_t counter = 0;

ISR(WDT_vect) {
    if (BLINK_PATTERN & (1UL << counter))
    {
        PORTB |= _BV(PORTB5);
    }
    else
    {
        PORTB &= ~_BV(PORTB5);
    }

    /* Increment counter */
    counter++;
    if (counter >= BLINK_PATTERN_LEN) {
        counter = 0;
    }
}

int main (void)
{
    /* Output mode for PB5 */
    DDRB |= _BV(DDB5);
    /* Clear PB5 */
    PORTB &= ~_BV(PORTB5);

    /*
     * The application software should always clear the Watchdog System Reset
     * Flag (WDRF) and the WDE control bit in the initialization routine,
     * even if the Watchdog is not in use.
     */

    MCUSR &= ~_BV(WDRF);
    WDTCSR &= ~_BV(WDE);

    /*
     * The sequence for clearing WDE and changing time-out configuration is
     * as follows:
     * 1.In the same operation, write a logic one to the Watchdog change
     * enable bit (WDCE) and WDE. A logic one must be written to WDE
     * regardless of the previous value of the WDE bit.
     */

    WDTCSR |= _BV(WDE) | _BV(WDCE);

    /*
     * 2.Within the next four clock cycles, write the WDE and Watchdog
     * prescaler bits (WDP) as desired, but with the WDCE bit cleared. This
     * must be done in one operation.
     * Interrupt Mode: WDTON=1 (fuse OK), WDE=0, WDIE=1
     * Prescaler 0.5s: WDP3=0,WDP2=1,WDP1=0,WDP0=1
     */

    WDTCSR = _BV(WDIE) | _BV(WDP2) | _BV(WDP0);

    // Poweroff ADC
    ADCSRA &= ~_BV(ADEN);

    // Global Interrupt enable
    sei();

    while(1) {
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode();
    }
}
