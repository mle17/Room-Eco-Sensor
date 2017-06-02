/*
 * distanceSensor.c
 *
 *      Created on: May 31, 2017
 *         Authors: Michael Le & Justin L'Heureux
 *         Project: 45
 *           Class: CPE 329 - Section 01
 *         Quarter: Spring 2017
 *
 *      Using TimerA 2.1 (P5.6) to output a trigger
 *      signal for ultrasonic sensor.
 */

#include "distanceSensor.h"
#include "msp.h"

static int distance_flag = 0;

/*
 *  Initialize bits for pin used to send pulse to trigger
 *  HC-SR04 ultrasonic sensor
 */
void distance_init() {
    /* Configure P5.6 as Timer A2.1 output */
    P5->SEL0 |= BIT6;
    P5->SEL1 &= ~BIT6;
    P5->DIR |= BIT6;
}

/*
 *  Sending pulse to HC-SR04 ultrasonic sensor to start
 *  measuring distance
 */
void start_meas_distance() {
    /* configure Timer A2.1 as PWM */
    TIMER_A2->CCR[0] = TIME_PERIOD;             /* time period */
    TIMER_A2->CCR[1] = INPUT_PULSE;             /* input pulse duration */
    TIMER_A2->CCTL[1] |= TIMER_A_CCTLN_CCIE |   /* set interrupt flag */
            TIMER_A_CCTLN_OUTMOD_7;             /* output reset/set mode */
    TIMER_A2->CTL |= TIMER_A_CTL_SSEL__SMCLK    /* SMCLK */
            | TIMER_A_CTL_MC__UP                /* up mode */
            | TIMER_A_CTL_CLR;                  /* clear TA0R register */
}

void save_distance(unsigned int distance) {

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
