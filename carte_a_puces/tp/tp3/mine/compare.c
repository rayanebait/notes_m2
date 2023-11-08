#include <avr/io.h>
#include <avr/interrupt.h>


void WriteTCNT1( unsigned int );

ISR(TIMER1_COMPA_vect)
{
    PORTB ^= _BV(PORTB5);
    WriteTCNT1(0);
}


int main (void)
{
    DDRB |= _BV(DDB5);
    
    /*Choix de l'horloge, ici low power crystal oscillator (par défaut, 16 Mhz)*/

    // CKSEL |= (1) | (1<<1) | (1<<2) | (1<<3);

    /*Prescaler directement sur l'horloge (on réduit la vitesse du processeur directement(pas tres malin))*/
    /*Ici /128*/

    // CLKPR |= (1<<CLKPS2) | (1<<CLKPS1) | (1<<CLKPS0);

    TIMSK1 |= (1<<OCIE1A);

    TCCR1A = 0;
    /*/256*/
    TCCR1B |= (1<<CS12) | (1<<WGM12);

    OCR1A = 31250;
    TCNT1 = 0;
    sei();

    while(1){
        ;
    }
}

void WriteTCNT1( unsigned int i )
{
    unsigned char sreg;
    /*Save state*/
    sreg = SREG;
    /*Non atomic operation -> disable interrupts*/
    cli();
    
    TCNT1 = i;

    /*Restore state*/
    SREG = sreg;
}
