/*
 * Task_3.c
 *
 * Created: 4/12/2018 7:29:54 PM
 * Author : Lopez
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

void Wait()
{
	uint8_t i;
	for(i=0;i<50;i++)
	{
		_delay_loop_2(0);
		_delay_loop_2(0);
		_delay_loop_2(0);
	}

}


int main(void)
{
	//set Timer1 in fast pwm mode 14
	TCCR1A = (1<<COM1A1) | (1<<WGM11);    //clear on compare
	TCCR1B = (1<<WGM12)  | (1<<WGM13) | (1<<CS11); //presc 8

	DDRB |= (1<<2) | (1 << 1);  //set OC1 as output
	ICR1=20000;      //period = 20 ms

	for(;;)
	{
		//go to end position
		OCR1A=2000;      //puls = 2000 us
		//and stay there for 2 seconds
		_delay_ms(2000);

		//go to center
		OCR1A=1500;      //puls = 1500 us
		_delay_ms(2000);

		//go to the other end
		OCR1A=1000;      //puls = 1000 us
		_delay_ms(2000);
	}
}
