/*
 * ds18b20.c
 *
 * Created: 3/31/2016 2:49:39 PM
 *  Author: waldonck
 */ 
#include <avr/io.h>
#include "ds18b20.h"

/************************************************************************/
/* Attempts to determine whether there is a sensor attached.            */
/************************************************************************/
unsigned char ow_reset(void)
{
	unsigned char presence;

 // pull DQ line low
	DDRB |= (1 << 2);
	PORTB |= (1 << 2);

 // leave it low for 480us
	delay_usec(479);

 // allow line to return high
	DDRB |= 1 << 2;
	PORTB |= (1 << 2);
	DDRB &= ~(1 << 2);
	PORTB &= ~(1 << 2);

 // wait for presence
	delay_usec(69);

 // get presence signal
	presence = PINB & (1 << 2);

// wait for end of timeslot
	delay_usec(229); 

	return(presence); // presence signal returned
} // 0=presence, 1 = no part