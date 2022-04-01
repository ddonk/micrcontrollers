#include <avr/io.h>
#include <stdbool.h>
#include "Accelerometer/Accelerometer.h"
#include "LCD/LCD.h"
#include <time.h>
#include "GameLogic.h"
#include <stdbool.h>
#include "Timer/Timer.h"
#include <stdio.h>


#define F_CPU 8e6

// How many procent of input needs to be correct for chech of input to be correct between 1-100
#define CORRECTTHRESHOLD 10
#define MEASURMENT_OFSETT 2

DIRECTION directions[] = {UP, DOWN, LEFT, RIGHT, FORWARD, BACKWARD};
bool reading = false;
int correctCount;
int totalCount;
accelerometer_measurment_t TRESHOLD_MEASURMENT;
int lives_left = 3;

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
	display_init();
	//accelerometer_init();
//
	//TRESHOLD_MEASURMENT = accelerometer_read();
//
	//return UNKNOWN;
    //display_text("---starting up game---");
}

int randomNumber()
{
    int rand_num;
    srand(accelerometer_read().x_geforce * accelerometer_read().y_geforce * accelerometer_read().z_geforce);
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

void GameLogic_draw_hearts(){
	char hearts[] = {1}; 

	display_text(hearts);
	//if (lives_left == 3){
		//
	//} else if (lives_left == 2)
	//{
		//
	//} else if (lives_left == 1)
	//{
	//} else {
//
	//}
}

void GameLogic_Draw_Dir(DIRECTION dir){
	display_clear();
	switch (dir)
	{
		case UP:
			display_text("Go upwards");
			break;
		case DOWN:
			display_text("Go downwards");
			break;
		case LEFT:
			display_text("Go left");
			break;
		case RIGHT:
			display_text("Go right");
			break;
		case FORWARD:
			display_text("Go forward");
			break;
		case BACKWARD:
			display_text("Go backward");
			break;
	}
}

void GameLogic_Round()
{
	// Setup of handling
    DIRECTION dir = randomDirection();
	set_timer(2000, time_passed);
	reading = true;
	//GameLogic_Draw_Dir(dir);
	display_clear();
	GameLogic_draw_hearts();

	// Starting handling
	while(reading) {
		accelerometer_measurment_t measurement = accelerometer_read();
		display_set_cursor(0, 1);
		char debugout[100];
		sprintf(debugout, "%d;%d;%d,%d,%d", measurement.x_geforce, measurement.y_geforce, measurement.z_geforce, correctCount, totalCount);
		display_text(debugout);
		switch (dir)
		{

			case UP:
				if(measurement.z_geforce <= TRESHOLD_MEASURMENT.z_geforce - MEASURMENT_OFSETT)
				{
					correctCount++;
				}
				break;
			case DOWN:
				if(measurement.z_geforce >= TRESHOLD_MEASURMENT.z_geforce + MEASURMENT_OFSETT)
				{
					correctCount++;
				}
				break;

			case LEFT:
				if(measurement.y_geforce <= TRESHOLD_MEASURMENT.y_geforce - MEASURMENT_OFSETT)
				{
					correctCount++;
				}
				break;
			case RIGHT:
				if(measurement.y_geforce >= TRESHOLD_MEASURMENT.y_geforce + MEASURMENT_OFSETT)
				{
					correctCount++;
				}
				break;
			case FORWARD:
				if(measurement.x_geforce <= TRESHOLD_MEASURMENT.x_geforce - MEASURMENT_OFSETT)
				{
					correctCount++;
				}
				break;
			case BACKWARD:
				if(measurement.x_geforce >= TRESHOLD_MEASURMENT.x_geforce + MEASURMENT_OFSETT)
				{
					correctCount++;
				}
				break;
		}
		totalCount++;
		wait(100);
	}
	
	float percentage = ((float)correctCount / (float)totalCount) * 100.0;
    bool inputCorrect = (int)percentage > CORRECTTHRESHOLD;

	display_clear();
    if(inputCorrect) {
	    display_text(" Input Correct! ");
	    } else {
	    display_text("Input Incorrect!");
		lives_left--;
    }
	GameLogic_draw_hearts();

	correctCount = 0;
	totalCount = 0;

	wait(1000);
}


