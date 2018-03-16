/*
 Design Assignment 2, Task 5 in C
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


int main(void)
{
    DDRB |= (1<<2);      // set pin b.2 as an output
	PORTD = 1<<2;
	EICRA = 0x02;        // falling edge
	EIMSK = (1<<INT0);   // turn on int0 interrupt 
	sei();               // enable global interrupts
	
	while(1)
	{
		
	}
	
}

ISR (TIMER1_COMPA_vect)
{
	PORTB ^= (1<<2);         // turn off the LED
	TIFR1 |= (1<<OCF1A);     // clear the timer flag
	TCCR1B = 0;              // turn off the timer
}

ISR(INT0_vect)
{
	PORTB ^=  (1<<2);       // turn on the LED
	OCR1A = 3125;           // Load the compare value
	TCNT1 = 0;
	TCCR1B |= (1<<WGM12);   // CTC mode
	TIMSK1 |= (1<<OCIE1A);  // turn on timer1 interrupt
	TCCR1B |= (1<<CS12);	// prescalar of 256
	EIFR |= (1<<INTF0);     // clear int0 flag
}



