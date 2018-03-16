/*
 Design Assignment 2, Task 4 in C
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


int main(void)
{
    DDRB |= (1<<2);    // set portb.2 as an output
	PORTB = 0x00;      // turn led off at beginning
	TCNT0 = 61;        // load timer value
	TCCR0A = 0x00;     // normal mode
	TCCR0B = 0x05;     // 1024 prescalar
	TIMSK0 = (1<<TOIE0); // set overflow interrupt
	sei();             // turn on global interrupts
	while(1)
	{
		// do nothing now :)
	}
}

ISR (TIMER0_OVF_vect)
{
	TCNT0 = 61;      // reload timer value
	PORTB ^= (1<<2);   // switch led on/off
	TIFR0 |= (1<<TOV0);   // clear the flag
}

