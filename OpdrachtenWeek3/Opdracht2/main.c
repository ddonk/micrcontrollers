/*
 * main.c
 *
 * Created: 16/02/2022 13:20:32
 * Author : berke
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define BIT(x)			(1 << (x))

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms ) {
	for (int tms=0; tms<ms; tms++) {
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

void timer2Init( void ) {
	OCR2 = 195;				// Compare value of counter 2
	TIMSK |= BIT(7);		// T2 compare match interrupt enable
	sei();					// turn_on intr all
	TCCR2 = 0b00001101;		// Initialize T2: timer, prescaler=1024, compare output disconnected,CTC,RUN
}

ISR( TIMER2_COMP_vect ) {
	if (OCR2 == 195){
		OCR2 = 117;			// When the register is set to 195 then set it to 117.
		PORTD = BIT(7);		// Turning led on
	} else {
		OCR2 = 195;			// When the register is set to 117 then set it to 195.
		PORTD = ~BIT(7);	// Turning led off
	}
}

int main( void ) {
	DDRD = BIT(7);			// set PORTD for output (toggle PD7)
	timer2Init();			// Initializing board

	while (1) {
		// do something else
		wait(10);			// every 10 ms (busy waiting
	}
}
