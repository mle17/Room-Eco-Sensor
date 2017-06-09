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

#define MAX_OCCUPANTS       99
#define MIN_OCCUPANTS       0

void init_UI();
void update_UI_distance(float distance);
void inc_occupants_LCD();
void dec_occupants_LCD();

int get_num_occupants();

#endif /* UI_H_ */
