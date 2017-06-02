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
#include <stdio.h>

void main(void) {
    unsigned int distance;

    WDTCTL = WDTPW | WDTHOLD;           /* Stop watchdog timer */

    distance_init();        /* Use P5.6 to send pulse */

    /* Enable global interrupt */
    __enable_irq();

    /* Enable Timer A2 Interrupts */
    NVIC_EnableIRQ(TA2_N_IRQn);

    start_meas_distance();

    while (1) {
        if (check_distance_flag()) {
            distance = get_distance();
            start_meas_distance();
        }
    }
}

/*
 *  After pulse to trigger HC-SR04 elapsed,
 *  turn off pulse, then turn off interrupt.
 */
void TA2_N_IRQHandler(void) {
    TIMER_A2->CCTL[1] &= ~(TIMER_A_CCTLN_CCIE + /* clear interrupt flag */
            TIMER_A_CCTLN_OUTMOD_MASK);         /* mask output */
    TIMER_A2->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_0;/* output to outmod value */
}
