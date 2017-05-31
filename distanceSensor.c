/*
 * distanceSensor.c
 *
 *      Created on: May 31, 2017
 *         Authors: Michael Le & Justin L'Heureux
 *         Project: 45
 *           Class: CPE 329 - Section 01
 *         Quarter: Spring 2017
 */

#include "distanceSensor.h"

static int distance_flag = 0;

void distance_init() {

}

void start_meas_distance() {

}

unsigned int get_distance() {

}

void set_distance_flag() {
    distance_flag = 1;
}

void clear_distance_flag() {
    distance_flag = 0;
}

int check_distance_flag() {
    return distance_flag;
}
