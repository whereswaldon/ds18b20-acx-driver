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
#include "ds18b20.h"

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
	serial_open(19200, SERIAL_8N1);
	char c;
	while(1) {
		//serial_write('x');
		//x_delay(1000);
		c = serial_read();
		serial_write(c);
	}
}

/*
 * Determine whether a sensor is connected.
 */
void sensor_test() {
	unsigned char present = ow_reset();
	serial_open(19200, SERIAL_8N1);
	
	//char read;
	present = ow_reset();
	while (!present) {
		x_delay(1000);
		present = ow_reset();
	}
	while(1) {
		write_bit(0);
		/*read = read_bit();
		if (read == 1) {
			serial_write('1');
		} else {
			serial_write('0');
		}*/
		x_delay(1000);
	}
}

int main(void)
{
	x_init();
	//x_new(1, red, 1);
	x_new(0, sensor_test, 1);
}
