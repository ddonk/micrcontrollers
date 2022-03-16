/*
 * Project name:
     Demo4_1 : Free running AD conversion on channel 1, PORTF.1
 * Author: Avans-TI, JW
 * Revision History: 
     20101229: - initial release;
 * Description:
     This program gives an interrupt on each ms
 * Test configuration:
     MCU:             ATmega128
     Dev.Board:       BIGAVR6
     Oscillator:      External Clock 08.0000 MHz
     Ext. Modules:    -
     SW:              AVR-GCC
 * NOTES:
     - Turn ON the PORT LEDs at SW12.1 - SW12.8
*/

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>

#include <avr/interrupt.h>
#include "LCD/LCD.h"
#include <stdio.h>

#define BIT(x)	(1 << (x))

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}


// Initialize ADC: 10-bits (left justified), free running
void adcInit( void )
{
	ADMUX = 0b11000011;			// AREF=VCC, result left adjusted, channel1 at pin PF1
	ADCSRA = 0b10000110;		// ADC-enable, no interrupt, start, free running, division by 64
}




// Main program: ADC at PF1
int main( void )
{

	init();
	DDRF = 0x00;
	DDRA = 0xff;
	DDRB = 0xff;				
	
	adcInit();					// initialize ADC

	while (1)
	{
		// Reading with adc
		ADCSRA |= BIT(6);
		while(ADCSRA & BIT(6));
		char data[20];
		PORTA = ADCL;
		PORTB = ADCH;
		int value = ADCL + (ADCH<<8);
		
		
		sprintf(data, "Acc: %dMG",  (int)scaled);
		clear();
		display_text(data);
		
		wait(100);
	}
}


// This function changes the channel of the accelerometer.
int channel;
void switch_channel(){
	if (channel == NULL){
		channel = 3;
	} else {
		channel++;
		if (channel > 5){
			channel = 3;
		}
	}
	// Clearing
	ADMUX =  0b11000000;
	
	// Adding
	ADMUX |= channel; 
}

// This function translates raw data to geforce.
// Still needs some work.
int translate_to_geforce(int raw){
	float fraction =  ((float)raw - (1170.0 / 2.0)) / 1170.0;
	float scaled = fraction * 30000.0;
	return (int) scaled;
}
