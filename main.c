/*
 * main.c
 *
 *      Created on: May 31, 2017
 *         Authors: Michael Le & Justin L'Heureux
 *         Project: 45
 *           Class: CPE 329 - Section 01
 *         Quarter: Spring 2017
 */
#include "msp.h"
#include "distanceSensor.h"

void main(void) {
    unsigned int distance;

    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    while (1) {
        if (check_distance_flag()) {
            distance = get_distance();
            start_meas_distance();
        }
    }
}

void TA2_N_IRQHandler(void) {

}
