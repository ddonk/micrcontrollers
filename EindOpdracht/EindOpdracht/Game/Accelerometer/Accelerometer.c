/*
 * Accelerometer.c
 *
 * Created: 3/16/2022 3:44:01 PM
 *  Author: berke
 */ 
#define F_CPU 8e6
#include <avr/io.h>
#include "Accelerometer.h"
#define BIT(x)	(1 << (x))

// Constructions
static void read_acc_direction(int* ptr);
static void switch_channel();
static int translate_to_geforce(int raw);


void accelerometer_init(){
	DDRF = 0x00;				// Setting ADC pins to input.
	ADMUX = 0b11000000;			// AREF=VCC, result left adjusted, channel1 at pin PF1
	ADCSRA = 0b10000110;		// ADC-enable, no interrupt, start, free running, division by 64
}

// Reads the accelerometer.
accelerometer_measurment_t accelerometer_read(){
	accelerometer_measurment_t measurment;
	
	// Reading all values
	read_acc_direction(&measurment.x_geforce);
	read_acc_direction(&measurment.y_geforce);
	read_acc_direction(&measurment.z_geforce);
	
	return measurment;
}

// Reads one channel at a time.
static void read_acc_direction(int* ptr){
	switch_channel();
	ADCSRA |= BIT(6);
	while(ADCSRA & BIT(6));
	int value = ADCL + (ADCH<<8);
	*ptr = translate_to_geforce(value);
}

// This function changes the channel of the accelerometer.
static void switch_channel(){
	static int channel = -1;
	if (channel == -1){
		channel = 3;
		} else {
		channel++;
		if (channel > 5){
			channel = 3;
		}
	}
	// Clearing
	int temporary =  0b11000000;
	temporary |= channel;
	
	// Adding
	ADMUX = temporary;
}

// This function translates raw data to geforce.
// Still needs some work.
static int translate_to_geforce(int raw){
	//float fraction =  ((float)raw - (1170.0 / 2.0)) / 1170.0;
	//float scaled = fraction * 30000.0;
	//return (int) scaled;
	return raw;
}

