/*
 * serial.c
 *
 * Created: 2/29/2016 7:07:12 PM
 *  Author: Chris
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/portpins.h>
#include "acx.h"
#include "serial.h"

/************************************************************************/
/* Serial Communication Functions                                       */
/************************************************************************/

/*
 * Set the serial port to listen at a given speed with a given configuration
 * (use the ones defined in serial.h).
 */
void serial_open(long speed, int config) {
	//disable interrupts during config
	cli();
	
	//set the configuration
	UCSR0C = config;
	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	//U2X0 = 1 seems to have universally lower errors
	UCSR0A |= (1<<U2X0);
	
	//figure out correct UBRR0 value for baud rate
	long ubrr;
	switch(speed) {
		case 2400:
			ubrr = 832;
			break;
		case 4800:
			ubrr = 416;
			break;
		case 9600:
			ubrr = 207;
			break;
		case 14400:
			ubrr = 138;
			break;
		case 19200:
			ubrr = 103;
			break;
		case 28800:
			ubrr = 68;
			break;
		case 38400:
			ubrr = 51;
			break;
		case 57600:
			ubrr = 34;
			break;
		case 76800:
			ubrr = 25;
			break;
		case 115200:
			ubrr = 16;
			break;
		case 230400:
			ubrr = 8;
			break;
		case 250000:
			ubrr = 7;
			break;
		default:
			//assume 19200
			ubrr = 103;
			break;
	}
	
	//set the baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	//enable interrupts after config
	sei();
}

/*
 * Waits for an available character in the serial receive buffer, then
 * reads and returns the character.
 */
char serial_read(void) {
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) ) {
		x_yield();
	}
	/* Get and return received data from buffer */
	return UDR0;
}

/*
 * Waits for the write buffer to be available, then writes a byte value
 * to the buffer.
 */
void serial_write(char data) {
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) ) {
		x_yield();
	}
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

/*
 * Write a whole string to the serial console
 */
void serial_write_string(char* data, int length) {
	int i = 0;
	for (; i < length; i ++) {
		serial_write(data[i]);
	}
}