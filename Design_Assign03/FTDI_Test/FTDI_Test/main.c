/*
 * FTDI_Test.c
 *
 * Created: 4/1/2018 7:52:55 PM
 * Author : Lopez
 */ 


#define F_CPU 8000000
#define BAUD 9600 
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

volatile int count;

void init_uart(){
    // setting the baud rate  based on FCPU and baudrate
    UBRR0H =0x00; 
    UBRR0L =0x0C;
    // enabling TX & RX 
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);              // enable receive and transmit
    UCSR0A = (1<<UDRE0)|(1<<U2X0);
    UCSR0C =  (1 << UCSZ01) | (1 << UCSZ00);    // Set frame: 8data, 1 stop

}

void ADC_init() {
	ADMUX = 0;                 // read from port ADC0
	ADMUX |= (1<<REFS0);       // use AVcc for reference
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1); // prescalar of 64
	ADCSRA |= (1<<ADEN);       // enable ADC
	ADCSRB = 0;                // free running mode
}

void USART_Transmit( char *data)
{
    while((*data != '\0')) {    // transmits all chars but null
		while(!(UCSR0A & (1<<UDRE0)));  // waits for transmit flag to clear
		UDR0 = *data;           // transmit next char
		data++;                 // move to next char
	}
}

unsigned int readADC() 
{
	ADMUX &= ~(1<<ADLAR);              // clear the adc value
	unsigned int val = 0;
	ADCSRA |= (1 << ADSC);             // start adc
	while(ADCSRA & (1<<ADSC));         // wait until adc is done
	
	val = ADC;
	val = val * 0.427;               // doing (5 * 100 * adc) / 1024, just simplified
	
	return val;
}

ISR (TIMER1_COMPA_vect)
{
	int temp = 0;
	char out[20];                 // will have the string
	count++;                      // increment counter
	temp = readADC();             // get adc value
	snprintf(out, sizeof(out), "Time(sec): %d\r\n", count);
	USART_Transmit(out);
	snprintf(out, sizeof(out), "%3d F\r\n", temp); // put string into out
	USART_Transmit(out);          // send string to be transmitted
	TIFR1 |= (1<<OCF1A);          // clear flag
}

int main (){
	count = 0;                    // start count at 0
	init_uart();                  // initialize UART
	ADC_init();                   // initialize ADC
	
	_delay_ms(250);               // delay to wait for everything to catch up
	
	OCR1A = 3125;                 // set compare value
	TCCR1B |= (1 << WGM12);       // set CTC mode
	TIMSK1 |= (1 << OCIE1A);      // enable timer interupt 
	TCCR1B |= (1<<CS12);          // prescalar of 256
	
	sei();               // enable interrupts
	while(1)
	{
		// do nothing
	}
    return 0;
}

