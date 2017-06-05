/*
 * ui.h
 *
 *      Created on: May 31, 2017
 *         Authors: Michael Le & Justin L'Heureux
 *         Project: 45
 *           Class: CPE 329 - Section 01
 *         Quarter: Spring 2017
 */

#ifndef UI_H
#define UI_H

#define LEN_DISTANCE        7
#define DISTANCE_CURSOR     0x8A
#define OCCUPANTS_CURSOR    0xCB

void init_UI();
void update_UI_distance(float distance);
void set_UI_sensor_status();
void clear_UI_sensor_status();

#endif /* UI_H_ */
