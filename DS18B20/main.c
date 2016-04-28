/*
 * ACX-kernel.c
 *
 * Created: 1/28/2016 2:56:40 PM
 * Author : waldonck
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "acx.h"
#include "serial.h"
#include "ds18b20.h"
#include "adc.h"

//Holds the value of the last target temperature provided
volatile int target;

//Holds the value of the last temperature measured in Celsius
volatile int lastMeasured;

/*
 * Constantly measure the temperature
 */
void sensor_controller() {
	unsigned char present = ow_reset();
	while (!present) {
		x_delay(1000);
		present = ow_reset();
	}
	while(1) {
		lastMeasured = ow_read_temperature();
		x_delay(1000);
	}
}

/*
 * Receive and respond to user commands
 */
void io_controller() {
	serial_open(19200, SERIAL_8N1);
	char * message;
	message = (char *) malloc(64);
	char input;
	while(1) {
		input = serial_read();
		switch (input) {
			case 'R': //report current box temperature	
				sprintf(message, "%d degrees C\n\r", lastMeasured); // print temp. C
				serial_write_string(message, strlen(message));

				break;
			case 'S': //set current target
				break;
			default:
				message = "Unrecognized Command";
				serial_write_string(message, strlen(message));
				break;	
		}
	}
	x_yield();
}

/*
 * Act on devices to affect box temperature.
 */
void box_controller() {
	x_yield();
}

/*
 * Kick off the whole shabang
 */
int main(void)
{
	x_init(); //kick off kernel
	serial_open(19200, SERIAL_8N1); //prep serial
	ADC_enable(); //enable ADC
	
	//declare locals
	int value;
	char * message1;
	char * message2;
	float fraction;
	message1 = (char *) malloc(64);
	message2 = (char *) malloc(64);
	//sample pot every 200ms and display results
	while(1){
		value = ADC_read(ADC0, 10, 1);
		fraction = ((float) value)/1023.0 * 5;//convert to number of volts
		dtostrf(fraction, 3, 2, message1);
		serial_write_string(message1, strlen(message1));
		sprintf(message2," volts (0x%x)\n\r", value);
		serial_write_string(message2, strlen(message2));
		x_delay(200);
	}
}
