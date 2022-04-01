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

// Percentage of input to be correct (1-100)
#define CORRECTTHRESHOLD 10

// Defines how much is counted as moving.
#define MEASURMENT_OFSETT 2

// Possible direction
DIRECTION directions[] = {UP, DOWN, LEFT, RIGHT, FORWARD, BACKWARD};
	
// Calibration measurement 
accelerometer_measurment_t TRESHOLD_MEASURMENT;

/** Game logic variables **/
bool isroundstarted = false;
bool isgamestarted = false;
int correctCount;
int totalCount;
int totalScore;
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

/* This function is used for initing the game*/
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

/* This function is used for generating random numbers from noise*/
int randomNumber()
{
    int rand_num;
    srand(accelerometer_read().x_geforce * accelerometer_read().y_geforce * accelerometer_read().z_geforce);
    // Will generate a random number between 0-5
    rand_num = rand() % 6;

    return rand_num;
}

/* This function generates a random direction*/
DIRECTION randomDirection()
{
    return directions[randomNumber()];
}

/* This is a funtion called by the interrupt of the timer and kills the loop of the game.*/
void time_passed(){
	isroundstarted = false;
}

/* Draws the hearts on the screen based on the score*/
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

/*Draws the direction on the screen based on the input of dir*/
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

/*This function is called in a loop and executes a game round*/
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
	
	// Checking score
	float percentage = ((float)correctCount / (float)totalCount) * 100.0;
    bool inputCorrect = (int)percentage > CORRECTTHRESHOLD;

	// Displaying score or lives
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
	
	// Reseting things
	correctCount = 0;
	totalCount = 0;		
	GameLogic_draw_hearts();


	// Waiting a sec
	wait(1000);
}


