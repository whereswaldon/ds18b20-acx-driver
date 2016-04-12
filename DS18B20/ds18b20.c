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
	DDRE |= (1 << PE4); //set for output
	PORTE &= ~(1 << PE4); //set value low

 // leave it low for 480us
	delay_usec(479);

 // allow line to return high
	PORTE |= (1 << PE4); //set value high
	DDRE &= ~(1 << PE4); //set for input
	//PORTE &= ~(1 << PE4); //set value low?

 // wait for presence
	delay_usec(69);

 // get presence signal
	presence = !(PINE & (1 << PE4)); //read for presence

// wait for end of timeslot
	delay_usec(229); 

	return(presence); // presence signal returned
} // 1=presence, 0=no sensor

/************************************************************************/
/* Read a single bit from the sensor                                    */
/************************************************************************/
unsigned char read_bit(void)
{
	//pull low
	DDRE |= (1 << PE4);
	PORTE &= ~(1 << PE4);
	//let return to high
	PORTE |= (1 << PE4); //set value high
	DDRE &= ~(1 << PE4); //set for input
	
	delay_usec(14); //wait 15us
	
	return(PINE & (1 << PE4));
}

/************************************************************************/
/* Write a single bit                                                   */
/************************************************************************/
void write_bit(char bitval)
{
	//pull low
	DDRE |= (1 << PE4);
	PORTE &= ~(1 << PE4);
	
	//if writing a one, go high
	if (bitval == 1) {
		PORTE |= (1 << PE4); //set value high
	}
	
	delay_usec(59); //wait 60us
	
	PORTE |= (1 << PE4); //set value high
	DDRE &= ~(1 << PE4); //set for input
}

unsigned char read_byte(void) 
{
	unsigned char value = 0;
	unsigned char i;
	for (i = 0; i < 8; i++) {
		if (read_bit()) {
			value |= 0x01 << i; //reads a bit and left-shifts it into place
		}
		delay_usec(19); //wait 20us
	}
}