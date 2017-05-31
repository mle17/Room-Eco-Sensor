/*
 * distanceSensor.h
 *
 *      Created on: May 31, 2017
 *         Authors: Michael Le & Justin L'Heureux
 *         Project: 45
 *           Class: CPE 329 - Section 01
 *         Quarter: Spring 2017
 */
#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

#define TIME_PERIOD         60000   // 20 ms time period
#define INPUT_PULSE         30      // 10 us input pulse

void distance_init();

void start_meas_distance();
unsigned int get_distance();

void set_distance_flag();
void clear_distance_flag();
int check_distance_flag();

#endif /* DISTANCESENSOR_H_ */
