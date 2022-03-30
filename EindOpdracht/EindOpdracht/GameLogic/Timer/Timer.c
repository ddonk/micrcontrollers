/*
 * Timer.c
 *
 * Created: 3/23/2022 3:16:41 PM
 *  Author: berke
 */ 
#define F_CPU 8e6
#include <avr/io.h>
#include "Timer.h"

const int prescaler_values[] = 
	{0b000, 0b001, 0b010,
	0b011, 0b100,  0b101};

void (*on_interupt)(void);

ISR( TIMER1_COMPA_vect ) {
	on_interupt();
	cli();
}

void set_timer(int time_ms, void (*fun_ptr)(void)){
	int prescaler, limit;

	// Looping through prescalers
	for (int i = 1; i < Timer_PRESCALER_COUNT; i++)
	{
		// Calculating max allowed value of prescaler and if in limit then use.
		if ((1 / TIMER_CLOCK_RATE) * prescaler_values[i] < time_ms){
			prescaler = prescaler_values[i];
			limit = time_ms / ((1 / TIMER_CLOCK_RATE) * prescaler_values[i]);
		}

		// Setting up all timers and other things
		setup_timer(prescaler, limit);
		on_interupt = fun_ptr;
	}
}

void setup_timer(int prescaler, int limit){
	OCR1A = limit;					// Setting calculated limit
	TIMSK |= (1 << 4);				// Setting on compare vect intr
	sei();							// Turning on interupts
	TCCR1B |= prescaler_values[0];	// Clearing prescaler
	TCCR1B |= prescaler;			// Setting new prescaler
}

void abort_timer(){
	cli();
}