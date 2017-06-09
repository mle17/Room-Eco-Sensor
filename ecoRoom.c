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
#include "msp.h"

static int outside_det_flag = 0;

void init_outside_detection() {
    P1->SEL0 &= ~BIT6;  // configure P1.6 as simple I/O
    P1->SEL1 &= ~BIT6;
    P1->DIR &= ~BIT6;   // set P1.6 as input
    P1->REN |= BIT6;    // enable pull resistor
    P1->OUT &= ~BIT6;   // set pull-down resistor
    P1->IES &= ~BIT6;   // detect low-to-high transition
    P1->IFG &= ~BIT6;   // clear IFG
    P1->IE |= BIT6;     // enable interrupt for P1.6

    P3->SEL0 &= ~BIT0;  // configure P3.0 as simple I/O
    P3->SEL1 &= ~BIT0;
    P3->DIR |= BIT0;    // set P3.0 as output
    P3->OUT &= ~BIT0;   // default output of off
}

void set_outside_flag() {
    outside_det_flag = 1;
}

void clear_outside_flag() {
    outside_det_flag = 0;
}

int check_outside_flag() {
    return outside_det_flag;
}
