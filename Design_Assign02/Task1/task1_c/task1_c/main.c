#include <avr/io.h>
#define F_CPU 100000UL
#include <util/delay.h>

/*
	DA2_task1 in c
	This will toggle an led on/off with a period of .5 sec.
	NOTE: without using the timers I wasn't sure how else to do this in c
		  since there is no 'nop' command to create my own delay
*/

int main(void)
{
    DDRB = (1<<PB2);   // set pb2 as an output
	while(1)
	{
		PORTB ^= (1<<PB2);   // flip port2 on/off
		_delay_ms(2500);     // delay for .25 sec
		_delay_ms(1);
	}
}

