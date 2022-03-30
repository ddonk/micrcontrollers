#include <avr/io.h>
#include <stdbool.h>
#include "Accelerometer/Accelerometer.h"
#include "LCD/LCD.h"
#include <time.h>
#include "GameLogic.h"
#include <stdbool.h>
#include "Timer/Timer.h"


#define F_CPU 8e6

// How many procent of input needs to be correct for chech of input to be correct between 1-100
#define CORRECTTHRESHOLD 5
#define CORRECTINPUTVALUE 1

DIRECTION directions[] = {UP, DOWN, LEFT, RIGHT, FORWARD, BACKWARD};

bool reading = false;
int correctCount;
int totalCount;

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

GAMELOGIC_ERROR GameLogic_Init()
{
    if (CORRECTTHRESHOLD < 1 || CORRECTTHRESHOLD > 100)
    {
        return CORRECTPERC_INVALID;
    }

    if (CORRECTINPUTVALUE < 1)
    {
        return CORRECTINPUTVALUE_INVALID;
    }

	display_init();
	accelerometer_init();

	return UNKNOWN;
    display_text("---starting up game---");
}

int randomNumber()
{
    int rand_num;
    srand(time(NULL));
    // Will generate a random number between 0-5
    rand_num = rand() % 6;

    return rand_num;
}

DIRECTION randomDirection()
{
    return directions[randomNumber()];
}

void time_passed(){
	reading = false;
}

void GameLogic_Draw_Dir(DIRECTION dir){
	switch (dir)
	{
		case UP:
			display_text(" Go upwards");
			break;
		case DOWN:
			display_text(" Go downwards");
			break;
		case LEFT:
			display_text(" Go left");
			break;
		case RIGHT:
			display_text(" Go right");
			break;
		case FORWARD:
			display_text(" Go forward");
			break;
		case BACKWARD:
			display_text(" Go backward");
			break;
	}
}

void GameLogic_Round()
{
	// Setup of handling
    DIRECTION dir = randomDirection();
	set_timer(2000, time_passed);
	reading = true;
	GameLogic_Draw_Dir(dir);

	// Starting handling
	while(reading) {
		switch (dir)
		{
			case UP:
			case DOWN:
				if(accelerometer_read().x_geforce >= CORRECTINPUTVALUE)
				{
					correctCount++;
				}
				break;

			case LEFT:
			case RIGHT:
				if(accelerometer_read().y_geforce >= CORRECTINPUTVALUE)
				{
					correctCount++;
				}
				break;

			case FORWARD:
			case BACKWARD:
				if(accelerometer_read().z_geforce >= CORRECTINPUTVALUE)
				{
					correctCount++;
				}
				break;
		}
		totalCount++;
		wait(100);
	}

    bool inputCorrect = ((float)((float)100 / (float)totalCount) * (float)correctCount) > CORRECTTHRESHOLD;

    if(inputCorrect) {
        display_text(" Input Correct! ");
    } else {
        display_text("Input Incorrect!");
    }

	wait(1000);
}


