// Design Assignment 2, task 2 in c

#include <avr/io.h>


int main(void)
{
    DDRB |= (1<<2);          // set pinb.2 as an output
	DDRD |= (0<<2);          // set pind.2 as an input
	PORTB = 0x00;            // initially have the led off
	
	while(1)
	{
		if (!(PIND & 0x04))           // wait until the switch is pressed (pind.2 will be 0 when pressed)
		{
			PORTB ^= (1<<2);              // turn on the led
			for(int i = 0; i < 4; i++)    // the timer goes on for .25 seconds, so do that 4 times to get one second
			{
				TCNT0 = 61;               // load start value
				TCCR0A = 0x00;            // normal mode
				TCCR0B = 0x05;            // prescalar of 1024
				while((TIFR0 & 0x01) == 0)
				{
					// wait until overflow occurs
				}
				TCCR0B = 0;              // turn off the timer
				TIFR0 |= 1<<TOV0;        // reset the flag
			}
			PORTB ^= (1<<2);             // once the for loop is done, turn the led off
		}
		
	}
}

