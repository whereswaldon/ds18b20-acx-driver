/*
 * adc.c
 *
 * Created: 4/27/2016 10:43:04 PM
 *  Author: waldonck
 */ 

#include <avr/io.h>
#include "acx.h"

/*
 * Configure and enable the ADC
 */
void ADC_enable() {
	//DIDR0 = 0;//Clear this data register
	//DDRF &= 0xFE;//set the pin for input
	//PORTF &= 0xFE;//set the port to zero to avoid internal pullup
	ADMUX |= 0x40; //set the reference voltage to VCC
	ADCSRA |= 0x7; //set the prescaler to 32
	ADCSRA |= 0x1 << ADEN; //enable conversion
}

/*
 * Turn off the ADC
 */
void ADC_disable() {
	ADCSRA ^= 0x1 << ADEN; //disable conversion
}

/*
 * Read from the given channel (as defined in adc.h) at the given
 * resolution and single-ended if the singleEnded flag is true.
 * NOTE: currently only supports 10-bit resolution and single-ended
 * conversions.
 */
int ADC_read(byte channel, byte resolution, bool singleEnded) {
	if (resolution != 10 || !singleEnded) {	return 0; /*invalid options*/}

	//break the channel into pieces and set the MUX
	byte low, high;
	char mux40 = channel & 0x1F;
	char mux5 = (channel & 0x20)>>2; //this shift gives it the correct position within ADCSRB
	ADMUX |= mux40;//set bits zero through four of the MUX
	ADCSRB |= mux5;//set bit five of the MUX
	
	//sum of total samples thus far
	int totalOfFive = 0;
	for (int i = 0; i < 5; i++) {
		ADCSRA |= 0x1 << ADSC; //start single conversion

		while (ADCSRA & (0x40)) {/*busy wait for bit to clear (indicates conversion complete)*/}
		low = ADCL;
		high = ADCH;
		
		totalOfFive += (((int) high) << 8) + low; //attach two bytes together
	}
	
	return totalOfFive/5; //we'll just deal with the lost precision
}