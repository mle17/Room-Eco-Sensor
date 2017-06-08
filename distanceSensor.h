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

#define RISING_EDGE         0
#define FALLING_EDGE        1

#define CCR_PER_METER       11750.0

void distance_init();

void start_meas_distance();
void save_init_distance_time(unsigned short time);
void save_final_distance_time(unsigned short time);
float get_distance();

void detect_distance_rising_flag();
void detect_distance_falling_flag();
int check_distance_rising_flag();

void save_pulse_time(unsigned short pulse_duration);

void set_distance_flag();
void clear_distance_flag();
int check_distance_flag();

#endif /* DISTANCESENSOR_H_ */
