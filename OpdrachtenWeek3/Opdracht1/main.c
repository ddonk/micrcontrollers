/*
 * Opdracht1.c
 *
 * Created: 2/16/2022 10:43:06 AM
 * Author : berke
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD/LCD.h"
#include <stdio.h>


void wait( int ms ) {
	for (int tms=0; tms<ms; tms++) {
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

int main(void)
{
	//Setting up lcd
	init();
	
	wait(20);
	//Setting up counter
	DDRD = ~(1<<7);
	TCCR2 = 0b00000111;		// counting via PD7, rising edge
	
	unsigned int previous = 0;
	
    /* Replace with your application code */
    while (1) 
    {	
		if (previous != TCNT2){
			//Clearing display
			clear();
		
			//Casting to a string
			char str[80];
			sprintf(str, "Count: %d", TCNT2);
			display_text(str);
			
	
		}
		previous = TCNT2;
		wait(10);
		
    }
}