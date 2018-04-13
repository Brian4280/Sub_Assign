/*
 * Task_2.c
 *
 * Created: 4/12/2018 6:43:31 PM
 * Author : Lopez
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int step;

void ADC_init() {
	ADMUX = 0;                 // read from port ADC0
	ADMUX |= (1<<REFS0);       // use AVcc for reference
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1); // prescalar of 64
	ADCSRA |= (1<<ADEN);       // enable ADC
	ADCSRB = 0;                // free running mode
}

unsigned int readADC()
{
	ADMUX &= ~(1<<ADLAR);              // clear the adc value
	ADCSRA |= (1 << ADSC);             // start adc
	while(ADCSRA & (1<<ADSC));         // wait until adc is done
	
	return ADC;
}

ISR(TIMER0_COMPA_vect)
{
	if (step == 0)           // change position of motor depending
		PORTB = (1 << 1);    // on which step it is currently at
	else if (step == 1)
		PORTB = (1 << 2);
	else if (step == 2)
		PORTB = (1 << 3);
	else if (step == 3)
		PORTB = (1 << 4);
		
		
	step++;
	if (step == 4)        // increment and reset step if necessary
		step = 0;

	_delay_us(500);           // small delay to let motor move
	OCR0A = 255  - readADC() / 4.02;      // change Compare value to ADC value
                                  // divide 4.02 so that max value is 254.
	TIFR0 |= (1 << OCF0A);       // reset interrupt flag
	
}

int main(void)
{
	ADC_init();          // initialize ADC
	DDRB  = 0xFF;        // set all port b ports as output
	//int val = 0;
	step = 0;
	TCCR0A |= (1 << WGM01);           // CTC mode
	TCCR0B |= (1 << CS01) | (1 << CS00);   // Prescalar of 64
	TIMSK0 |= (1 << OCIE0A);               // turn on overflow interrupt
	OCR0A = 200;                      // set initial match value
	sei();                   // turn on interrupt
	
    while (1) 
    {	
    }
}

