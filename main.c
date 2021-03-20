#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "uart_init.h"
#include "uart_write.h"


void configure_adc() {

	//-- configure ADC
	ADCSRB = 0;
	ADMUX  =
	(0 << REFS1) |  // activate AVCC
	(1 << REFS0) |
	(0 << ADLAR) |	// aligned to LSB
	(1 << MUX3)  |	// select V_BG
	(1 << MUX2)  |
	(1 << MUX1)  |
	(0 << MUX0);

	ADCSRA =
	(1 << ADEN) | 	// Enable the ADC
	(0 << ADIE) |	// Enable interrupts
	(1 << ADSC) |	// start conversion
	(0 << ADATE) |	// Enable auto trigger
	(1 << ADPS2) |	//
	(1 << ADPS1) |	//  > P = 128 -> f(ADC) = 125kHz
	(1 << ADPS0);	//

	while(ADCSRA & _BV(ADSC));	// just wait it out, the first conversion and throw away the result
}



uint16_t check_battery_level() {
	ADCSRA |= _BV(ADSC);		// start another 13 clock cycles @ 125kHz conversion.
	while(ADCSRA & _BV(ADSC));	// wait till done

	return ADC;
}


int main(void) {
	uart_init();
	configure_adc();


	while(1) {

		uint16_t battery_level = check_battery_level();

		send_char((uint8_t)(battery_level >> 8));
		send_char((uint8_t)battery_level);

		_delay_ms(1000);
	}

	return 0;
}
