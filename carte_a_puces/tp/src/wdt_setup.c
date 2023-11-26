#include <avr/io.h>
#include <avr/wdt.h>
#include <stdint.h>

#include "wdt_setup.h"

/*For timeouts only, no system resets,
prescaler value=n implies 
(16)*2^(1+n)ms*/
void WDT_timeout_setup(uint8_t prescaler){
    /*
        From https://electronics.stackexchange.com/questions/74840/use-avr-watchdog-like-normal-isr/74850#74850
        To avoid loops caused by watchdog resets do this at the beginning :
    */
    if(MCUSR & _BV(WDRF)){            // If a reset was caused by the Watchdog Timer...
        MCUSR &= ~_BV(WDRF);                 // Clear the WDT reset flag
        WDTCSR |= (_BV(WDCE) | _BV(WDE));   // Enable the WD Change Bit
        WDTCSR = 0x00;                      // Disable the WDT
    }

    /*Setup part*/

    /*
        Permits changes of the prescaler, this sequence of
        instruction should be done exactly in this way.
    */
    if(prescaler > 9){
        /*No prescaler, equivalent to
        prescaler == 0*/

        WDTCSR = _BV(WDCE) | _BV(WDE);
        /*
            Enable interrupts + prescaler of 1<<prescaler
        */
        WDTCSR = _BV(WDIE);

    } else {
        WDTCSR = _BV(WDCE) | _BV(WDE);
        /*
            Enable interrupts + prescaler of 1<<prescaler
        */
        WDTCSR = _BV(WDIE) | (prescaler);
    }
}