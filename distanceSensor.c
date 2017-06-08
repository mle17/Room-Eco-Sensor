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
#include "freqDelay.h"
#include "msp.h"
#include <stdio.h>

static int distance_flag = 0;
static int edge_flag = RISING_EDGE;

static unsigned short init_time = 0;
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

    TIMER_A2->CCR[0] = TIME_PERIOD;             // time period
    TIMER_A2->CCR[1] = INPUT_PULSE;             // input pulse duration
    TIMER_A2->CCTL[1] &= ~TIMER_A_CCTLN_OUT;    // set OUT value = 0

    TIMER_A2->CTL |= TIMER_A_CTL_SSEL__SMCLK |  // SMCLK
            TIMER_A_CTL_ID__8 |                 // Divide timer by 8
            TIMER_A_CTL_MC__UP;                 // up mode

    TIMER_A2->EX0 |= TIMER_A_EX0_IDEX__2;       // Divide timer by 2

    /* Configure P2.5 as Timer A0.2 input capture */
    P2->SEL0 |= BIT5;                           // TA0.CCI2A input capture pin
    P2->SEL1 &= ~BIT5;
    P2->DIR &= ~BIT5;

    TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_2 |     // use SMCLK as clock source,
            TIMER_A_CTL_MC_2 |                  // start timer in continuous mode
            TIMER_A_CTL_ID__8 |                 // divide timer by 8
            TIMER_A_CTL_CLR;                    // clear TA0R

    TIMER_A0->EX0 |= TIMER_A_EX0_IDEX__3;       // Divide timer by 4

    TIMER_A0->CCTL[2] &= ~TIMER_A_CCTLN_CM_MASK;// mask mode of capture
    TIMER_A0->CCTL[2] |= TIMER_A_CCTLN_CCIS_0 | // use CCI2A
            TIMER_A_CCTLN_CAP |                 // enable capture mode
            TIMER_A_CCTLN_SCS |                 // synchronous capture
            TIMER_A_CCTLN_CM_3 |                // capture edges
            TIMER_A_CCTLN_CCIE;                 // enable capture interrupt

    /* use Timer A0.2 (P2.5) as input capture */
    TIMER_A0->CCTL[2] &= ~TIMER_A_CCTLN_CM_MASK;// mask mode of capture
    TIMER_A0->CCTL[2] |= TIMER_A_CCTLN_CM_3 |   // capture edges
            TIMER_A_CCTLN_CCIE;                 // enable capture interrupt
}

/*
 *  Sending pulse to HC-SR04 ultrasonic sensor to start
 *  measuring distance
 */
void start_meas_distance() {
    /* configure Timer A2.1 (P5.6) as PWM */
    TIMER_A2->CCTL[1] &= ~TIMER_A_CCTLN_OUTMOD_MASK;// mask output
    TIMER_A2->CCTL[1] |= TIMER_A_CCTLN_CCIE |       // set interrupt flag
            TIMER_A_CCTLN_OUTMOD_7;                 // output reset/set mode
    TIMER_A2->CTL |= TIMER_A_CTL_CLR;               // clear TA2R register
}

void save_init_distance_time(unsigned short time) {
    init_time = time;
}

void save_final_distance_time(unsigned short time) {
    diff_time = time - init_time;
}

float get_distance() {
    float distance;

    printf("Diff: %hu\n", diff_time);   // to delete
    //delay_ms(74, FREQ_48_MHz);
    distance = (float) diff_time / CCR_PER_METER;

    return distance;
}

void detect_distance_rising_flag() {
    edge_flag = RISING_EDGE;
}

void detect_distance_falling_flag() {
    edge_flag = FALLING_EDGE;
}

int check_distance_rising_flag() {
    return (edge_flag == RISING_EDGE);
}

void save_pulse_time(unsigned short pulse_duration) {
    diff_time = pulse_duration;
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
