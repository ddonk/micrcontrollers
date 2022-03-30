/*
 * Accelerometer.h
 *
 * Created: 3/16/2022 3:44:18 PM
 *  Author: berke
 */ 


#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

typedef struct{
	int x_geforce;
	int y_geforce;
	int z_geforce;
} accelerometer_measurment_t;

void accelerometer_init();
accelerometer_measurment_t accelerometer_read();

#endif /* ACCELEROMETER_H_ */