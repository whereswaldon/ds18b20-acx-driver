/*
 * ACX-kernel.c
 *
 * Created: 1/28/2016 2:56:40 PM
 * Author : waldonck
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "acx.h"
#include "serial.h"

/*
 * A thread that blinks a green LED on output 11
 */
void green() {
	set_output(DDRB, led11);
	int wait = 2000;
	while (1)
	{
		output_hi(PORTB, led11);
		x_delay(wait);
		output_low(PORTB, led11);
		x_delay(wait);
	}
}

/*
 * A thread that blinks a red LED on output 12
 */
void red() {
	set_output(DDRB, led12);
	int wait = 3000;
	while (1)
	{
		output_hi(PORTB, led12);
		x_delay(wait);
		output_low(PORTB, led12);
		x_delay(wait);
	}
}

void serial_test() {
	serial_open(192000, SERIAL_8N1);
	char c;
	while(1) {
		c = serial_read();
		serial_write(c);
	}
}

int main(void)
{
	x_init();
	//x_new(1, red, 1);
	x_new(0, serial_test, 1);
}
