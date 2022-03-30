#include <avr/io.h>
#include <stdbool.h>
#include "Accelerometer/Accelerometer.h"
#include "LCD/LCD.h"
#include <time.h>
#include "GameLogic.h"


#define F_CPU 8e6

// How many procent of input needs to be correct for chech of input to be correct between 1-100
#define CORRECTPERCENTAGE 75
#define BUFFERSIZE 40
#define CORRECTINPUTVALUE 5

int inputbuffer[BUFFERSIZE];

GAMELOGIC_ERROR GameLogic_Init()
{
    if (CORRECTPERCENTAGE < 1 || CORRECTPERCENTAGE > 100)
    {
        return CORRECTPERC_INVALID;
    }

    if (BUFFERSIZE < 1)
    {
        return BUFFERSIZE_INVALID;
    }

    if (CORRECTINPUTVALUE < 1)
    {
        return CORRECTINPUTVALUE_INVALID;
    }

	return UNKNOWN;
    display_text("---starting up game---");
}

int randomNumber()
{
    int rand_num;
    srand(time(NULL));
    // Will generate a random number between 0-5
    rand_num = rand() % 5;

    return rand_num;
}

DIRECTION randomDirection()
{
    return dirs[randomNumber()];
}

bool CheckArray()
{
    int count = 0;
    for (int i = 0; i < BUFFERSIZE; i++)
    {
        int result = inputbuffer[i];
		if (result > CORRECTINPUTVALUE)
        {
            count += 1;
        }
    }

    int percResult = (count / BUFFERSIZE) * 100;

    if (percResult >= CORRECTPERCENTAGE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void GameLogic_Round()
{
    DIRECTION dir = randomDirection();

    switch (dir)
    {
    case UP:
	case DOWN:

        for (int i = 0; i < BUFFERSIZE; i++)
        {
            inputbuffer[i] = accelerometer_read().x_geforce;
        }
        break;
    case LEFT:
	case RIGHT:
        for (int i = 0; i < BUFFERSIZE; i++)
        {
            inputbuffer[i] = accelerometer_read().y_geforce;
        }
        break;
    case FORWARD:
	case BACKWARD:
        for (int i = 0; i < BUFFERSIZE; i++)
        {
            inputbuffer[i] = accelerometer_read().z_geforce;
        }
        break;
    }

    bool inputCorrect = CheckArray();

    if(inputCorrect) {
        display_text("---Input Correct!---");
    } else {
        display_text("---Input Incorrect---");
    }
}

