/*
 * MT2.c
 *
 * Created: 4/22/2018 5:04:22 PM
 * Author : Lopez
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "nrf24l01.h"

volatile bool rf_interrupt = false;


// Set up all of the SPI ports with the nRF struct and turn on int0 interrupt
nRF24L01 *setup_rf(void){
	nRF24L01 *rf = nRF24L01_init();
	rf->ss.port = &PORTB;
	rf->ss.pin = PB2;
	rf->ce.port = &PORTB;
	rf->ce.pin = PB1;
	rf->sck.port = &PORTB;
	rf->sck.pin = PB5;
	rf->mosi.port = &PORTB;
	rf->mosi.pin = PB3;
	rf->miso.port = &PORTB;
	rf->miso.pin = PB4;
	EICRA |= _BV(ISC01);        
	EIMSK |= _BV(INT0);       // turn on int0 interrupt
	nRF24L01_begin(rf);
	return rf;
}

void spi_init() {
	DDRB &= ~((1<<2)|(1<<3)|(1<<5));   //SCK, MOSI and SS as inputs
	DDRB |= (1<<4);     // MISO as output
	SPCR &= !(1<<MSTR);    // set as slave
	SPCR |= (1<<SPR0)|(1<<SPR1);   // divide clock by 128
	SPCR |= (1<<SPE);        // enable SPI
}

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
	ADMUX |= (1<<REFS0);       // use Vcc for reference
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

// nRF24L01 interrupt
ISR(INT0_vect) {
	rf_interrupt = true;    // turn on variable for while loop
	EIFR |= (INTF0);   // reset interrupt flag
}

int main(void)
{
    init_uart();         // set UART variables
	ADC_init();          // set ADC variables
	_delay_ms(150);
	USART_Transmit("Started!\r\n");
	uint8_t address[5] = {0x01, 0x01, 0x01, 0x01, 0x01 };   // address for nRF
	sei();               // turn on global interrupts
	nRF24L01 *rf = setup_rf();      // initialze and setup nRF struct
	nRF24L01_listen(rf, 0, address);
	uint8_t addr[5];
	nRF24L01_read_register(rf, 0x00, addr, 1);
	
	
    while (1) 
    {
		if (rf_interrupt)
		{
			rf_interrupt = false;
			while (nRF24L01_data_received(rf)) {    
				nRF24L01Message msg;
				nRF24L01_read_received_data(rf, &msg);   // gets data that was sent
				USART_Transmit((char *)msg.data);     // transmit the temperature value sent
				USART_Transmit("\r\n");               // transmit a line feed
			}
			
			nRF24L01_listen(rf, 0, address);
		}
    }
	
	return 0;
}
