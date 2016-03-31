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


void forever() {
	while(1) {
		
	}
}

int main(void)
{
	x_init();
	x_yield();
	x_new(1, forever, 1);
	_delay_ms(100);
	DDRB=0x80;
	while (1) 
	{
		PORTB ^= 0x80;
		_delay_ms(100);
		PORTB = 0;
		_delay_ms(100);
	}
	return 0;
}
