/*
 * adc.c
 *
 * Created: 4/27/2016 10:43:04 PM
 *  Author: waldonck
 */ 

#include <avr/io.h>
#include "acx.h"

int ADC_read(byte channel, byte resolution, bool singleEnded) {
	if (resolution != 10 || !singleEnded) {
		return 0; //invalid options
	}
	DIDR0 = 0;
	DDRF &= 0xFE;//set the pin for input
	PORTF &= 0xFE;//set the port to zero to avoid internal pullup
	
	//break the channel into pieces and set the MUX
	byte low, high;
	char mux40 = channel & 0x1F;
	char mux5 = (channel & 0x20)>>2; //this shift gives it the correct position within ADCSRB

	ADCSRA |= 0x5; //set the prescaler to 32
	ADCSRA |= 0x1 << ADEN; //enable conversion
	
	ADMUX |= mux40;
	ADCSRB |= mux5;
	low = ADCL;
	high = ADCH;
	
	ADCSRA |= 0x1 << ADSC; //start single conversion
	x_delay(1);
	while (ADCSRA & (0x40)) {
		//busy wait for bit to clear (indicates conversion complete)
	}
	low = ADCL;
	high = ADCH;
	
	//ADCSRA ^= 0x1 << ADEN; //disable conversion
	
	return (((int) high) << 8) + low; //attach two bytes together
}