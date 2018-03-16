/*
 Design Assignment 2, Task 3 in C
 */ 

#include <avr/io.h>


int main(void)
{
    DDRB |= (1<<2);
	PORTB = 0;
	while(1)
	{
		TCNT0 = 61;
		TCCR0A = 0x00;               // normal mode
		TCCR0B = 0x05;               // 1024 prescalar
		while((TIFR0 & 0x01) == 0)
		{
			// wait until overflow occurs
		}	
		TCCR0B = 0;                  // stop the clock
		TIFR0 |= 1<<TOV0;            // clear the flag
		PORTB ^= (1<<2);             // switch the led on/off
	}
}

