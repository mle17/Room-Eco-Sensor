/*
 * ui.c
 *
 *      Created on: May 31, 2017
 *         Authors: Michael Le & Justin L'Heureux
 *         Project: 45
 *           Class: CPE 329 - Section 01
 *         Quarter: Spring 2017
 *
 *      Using LCD to display the distance of the ultrasonic sensor
 *      and whether the sensor detects an object or person.
 */

#include "freqDelay.h"
#include "ui.h"
#include "lcd.h"
#include <stdio.h>

static int numOccupants = 0;

void init_UI() {
    LCD_init();
    delay_ms(20, FREQ_3_MHz);

    Clear_LCD();
    delay_ms(20, FREQ_3_MHz);

    Home_LCD();
    delay_ms(20, FREQ_3_MHz);

    WriteLine("Distance: ");
    delay_ms(10, FREQ_3_MHz);

    SetCursorLocation(SECOND_LINE);
    delay_ms(10, FREQ_3_MHz);

    WriteLine("Occupants:  0");
    delay_ms(10, FREQ_3_MHz);
}

void update_UI_distance(float distance) {
    char distance_str[LEN_DISTANCE];

    sprintf(distance_str, "%1.2f m", distance);

    SetCursorLocation(DISTANCE_CURSOR);
    delay_ms(10, FREQ_3_MHz);

    WriteLine(distance_str);
}

void inc_occupants_LCD() {
    char numOcc[10];

    SetCursorLocation(OCCUPANTS_CURSOR);
    delay_ms(10, FREQ_3_MHz);

    if (numOccupants < MAX_OCCUPANTS)
        numOccupants++;

    sprintf(numOcc, "%2d", numOccupants);
    WriteLine(numOcc);
}

void dec_occupants_LCD() {
    char numOcc[10];

    SetCursorLocation(OCCUPANTS_CURSOR);
    delay_ms(10, FREQ_3_MHz);

    if (numOccupants > MIN_OCCUPANTS)
        numOccupants--;

    sprintf(numOcc, "%2d", numOccupants);
    WriteLine(numOcc);
}

int get_num_occupants() {
    return numOccupants;
}
