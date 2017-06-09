/*
 * ecoRoom.h
 *
 *      Created on: June 6, 2017
 *         Authors: Michael Le & Justin L'Heureux
 *         Project: 45
 *           Class: CPE 329 - Section 01
 *         Quarter: Spring 2017
 *
 *      Using TimerA 2.1 (P5.6) to output a trigger
 *      signal for ultrasonic sensor.
 */

#ifndef ECOROOM_H
#define ECOROOM_H

#define DOOR_DISTANCE       0.91    // distance in meter
#define MIN_DISTANCE        0.01    // prevent noise

void set_outside_flag();
void clear_outside_flag();
int check_outside_flag();

#endif /* ECOROOM_H_ */
