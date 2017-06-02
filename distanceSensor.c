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

static unsigned short init_time = 0;
static unsigned short final_time = 0;
static unsigned short diff_time = 0;

/*
 *  Initialize bits for pin used to send pulse to trigger
 *  HC-SR04 ultrasonic sensor
 */
void distance_init() {
    /* Configure P5.6 as Timer A2.1 output */
    P5->SEL0 |= BIT6;
    P5->SEL1 &= ~BIT6;
    P5->DIR |= BIT6;

    TIMER_A2->CCTL[1] &= ~TIMER_A_CCTLN_OUT;    // set OUT value = 0

    /* Configure P2.5 as Timer A0.2 input capture */
    TIMER_A0->CCTL[2] |=
            TIMER_A_CCTLN_CCIS_0 |              // Use CCI2A
            TIMER_A_CCTLN_CAP |                 // Enable capture mode
            TIMER_A_CCTLN_SCS;                  // Synchronous capture
}

/*
 *  Sending pulse to HC-SR04 ultrasonic sensor to start
 *  measuring distance
 */
void start_meas_distance() {
    /* configure Timer A2.1 as PWM */
    TIMER_A2->CCR[0] = TIME_PERIOD;                 // time period
    TIMER_A2->CCR[1] = INPUT_PULSE;                 // input pulse duration
    TIMER_A2->CCTL[1] &= ~TIMER_A_CCTLN_OUTMOD_MASK;// mask output
    TIMER_A2->CCTL[1] |= TIMER_A_CCTLN_CCIE |       // set interrupt flag
            TIMER_A_CCTLN_OUTMOD_7;                 // output reset/set mode
    TIMER_A2->CTL |= TIMER_A_CTL_SSEL__SMCLK        // SMCLK
            | TIMER_A_CTL_MC__UP                    // up mode
            | TIMER_A_CTL_CLR;                      // clear TA0R register

    /* use Timer A0.2 as input capture */
    TIMER_A0->CCTL[2] &= ~TIMER_A_CCTLN_CM_MASK;    // mask mode of capture
    TIMER_A0->CCTL[2] |= TIMER_A_CCTLN_CM_1 |       // capture rising edge
            TIMER_A_CCTLN_CCIE;                     // enable capture interrupt
}

void save_init_distance_time(unsigned short time) {
    init_time = time;
}

void save_final_distance_time(unsigned short time) {
    final_time = time;
    diff_time = final_time - init_time;
}

unsigned int get_distance() {
    return diff_time;
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
