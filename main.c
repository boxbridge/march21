
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

signed long int count=256;
unsigned int button;
int i=0;
int main(void)
{
	DDRC=0b0000000;  //0x00;		//define as PC0 as input lines 
	DDRB=0b00100000;	            //define PB1 lines as output 
	PORTB=0b00000000;			   //initially keep pulse low at PB1
	TCCR0A|=(1<<CS00)|(1<<CS01);    //define clock source & prescaler=64;
	TCNT0=0X00;					   // initial value of timer0=0;
	TIMSK|=(1<<TOIE0);			   //enable local interrupt
	sei();					       //enable global interrupt
	
    while (1) 
    {		
				
		while(TCNT0<count)
		{
			PORTB=0b00000000; //keep pulse low if Timer value less than count
		}
		while(TCNT0>=count)
		{
			PORTB|=(1<<PB1); //keep pulse at PB1 high if Timer value greater than count
		}	
    }
}

ISR(TIMER0_OVF_vect)
{		
		button=PINC & 0X01;	//Store status of button1
		
		if(button==0X01)		//check button condition
		{
			PORTB=0x00;			  //while delay of 250 ms, pulse should be low...else status of pulse remain high and will give 100% duty cycle
			_delay_ms(250);		  //Debounce time for button1
			count=count-64;		  //decrease value of count towards 0
				i++;
				if (i=3)		
				{
				count=256;
				i=0;
				}
		}
		

}