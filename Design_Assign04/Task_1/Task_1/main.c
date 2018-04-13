/*
 * Task_1.c
 *
 * Created: 4/12/2018 5:09:15 PM
 * Author : Lopez
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int val = 0;

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

int main(void)
{
	ADC_init();
	DDRB |= (1 << 1) | (1 << 2);   // set pins portb.1 and 2 as output
	PORTB = (1 << 2);              // set B2 as high for clockwise rotation
	DDRD |= (1 << 6);              // set D6 as output to control enable on driver
	
	OCR0A = 243;
	
	TCCR0A |= (1 << COM0A1);               // non-inverted mode
	TCCR0A |= (1 << WGM02) | (1 << WGM00); // phase correct PWM
	TCCR0B |= (1 << CS01);                 // prescalar of 8
		
	
	
	while(1)
	{
		val = readADC() / 4.20;  // read ADC and divide by 4.2 to not exceed 243
		OCR0A = val;             // which is 95% of PWM value. then update PWM compare
	}
}

