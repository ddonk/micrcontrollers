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
bool isroundstarted = false;
bool isgamestarted = false;
int correctCount;
int totalCount;
int totalScore;
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
	accelerometer_init();
	
	DDRA = 0x0;
	display_text("Starting up game");
	display_set_cursor(0,1);
	display_text("Press PA7...");
	while(PINA != 0b10000000);//Waiting to press button
	TRESHOLD_MEASURMENT = accelerometer_read();
	
	return UNKNOWN;
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
	isroundstarted = false;
}

void GameLogic_draw_hearts(){
	// Setting logic for numbers
	char lives[3];
	int cursor_pos = 15;

	for (int i = 3; i> 0; i-- )
	{
		// Setting correct cursor pos
		display_set_cursor(cursor_pos, 0);
		cursor_pos--;
		
		// Setting hearts
		if (i <= lives_left){
			lives[i-1] = 1;
		} else {
			lives[i-1] = 2;
		}
	}
	
	// Displaying hearts
	display_text(lives);
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
	isroundstarted = true;
	display_clear();
	GameLogic_Draw_Dir(dir);
	GameLogic_draw_hearts();

	// Starting handling
	while(isroundstarted) {
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
	display_set_cursor(0,1);
    if(inputCorrect) {
	    display_text(" Input Correct! ");
		totalScore++;
	} else {
	    display_text("Input Incorrect!");
		lives_left--;
		if(lives_left <= 0) {
			display_clear();
			char gameoverString[20];
			sprintf(gameoverString, "Game Over! S:%d", totalScore);
			lives_left = 3;
			totalScore = 0;
			display_text(gameoverString);
			display_set_cursor(0,1);
			display_text("Press PA7...");
			while(PINA != 0b10000000);
			
			correctCount = 0;
			totalCount = 0;

			return;
		}
    }
	
	correctCount = 0;
	totalCount = 0;
					
	GameLogic_draw_hearts();



	wait(1000);
}


