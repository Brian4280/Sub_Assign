
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void init_uart(){
	// setting the baud rate for 9600 
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
	
	val = ADC;                    // obtain the ADC value
	
	return val;                  
}

int main(void)
{
	int beat;        // will contain the heart beat value that will be plotted
	char outs[40];   // string that will be transmitted the BT module
	init_uart();     // initialize the UART
	ADC_init();      // initialize ADC
	
	_delay_ms(250);

	// setting up the graph for meguno-link time graph.
	USART_Transmit("{TIMEPLOT|SET|title=Heartbeat Monitor}\n");  // set title
	USART_Transmit("{TIMEPLOT|SET|y-label=Heartbeat Value}\n");  // set y-value
	USART_Transmit("{TIMEPLOT|STYLE|Heartbeat:bs_2}\n");         // label the values
	
	while(1)
	{
		// simple smoothing algorithm
		// will read in seven ADC values with a small delay after each one, then
		// it will find the average of the values for a smoother value.
		for(int i = 0; i < 7; i++)
		{
			beat += readADC();
			_delay_us(250);
		}
		
		beat = beat / 7;     // find the average of the ADC values
		
		/*
		Used for "Super smoothing" the heart beat. This will only the R and S
		peaks, which can be used just to see the beats. It will just make all of 
		the middle values the same constant value so that it shows a straight line
		on the non-peak values.
		
		if (beat < 380 && beat > 210)
			num = 300;
		*/
		
		// Get the string for displaying the point to the graph.
		snprintf(outs, sizeof(outs), "{TIMEPLOT|DATA|Heartbeat|T|%d}\n", beat);
		USART_Transmit(outs);   // transmit it to the BT module
		beat = 0;       // reset the heart beat value.
	}
	
	return 0;
}

