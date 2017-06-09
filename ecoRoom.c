/*
 * ecoRoom.c
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
#include "ecoRoom.h"

static int outside_det_flag = 0;

void set_outside_flag() {
    outside_det_flag = 1;
}

void clear_outside_flag() {
    outside_det_flag = 0;
}

int check_outside_flag() {
    return outside_det_flag;
}
