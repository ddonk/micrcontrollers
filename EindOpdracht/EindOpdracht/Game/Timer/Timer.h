/*
 * Timer.h
 *
 * Created: 3/23/2022 3:16:55 PM
 *  Author: berke
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#define Timer_PRESCALER_COUNT 6
#define TIMER_CLOCK_RATE 8000

void set_timer(int time_ms, void (*fun_ptr)(void));
void abort_timer();

#endif /* TIMER_H_ */