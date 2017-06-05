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
#include "freqDelay.h"
#include "ui.h"
#include "lcd.h"
#include <stdio.h>

void main(void) {
    unsigned int distance = 0;

    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    distance_init();        // Use P5.6 to send pulse
    init_UI();              // Use P4.0-2 P4.4-7 for LCD

    /* Enable global interrupt */
    __enable_irq();

    /* Enable Timer A2 Interrupts */
    NVIC_EnableIRQ(TA2_N_IRQn);
    NVIC_EnableIRQ(TA0_N_IRQn);

    delay_ms(20, FREQ_3_MHz);
    start_meas_distance();

    while (1) {
        if (check_distance_flag()) {
            distance = get_distance();
            printf("Distance: %u\n", distance); // to delete
            start_meas_distance();
        }
    }
}

/*
 *  After pulse to trigger HC-SR04 elapsed,
 *  turn off pulse, then turn off interrupt.
 */
void TA2_N_IRQHandler(void) {
    printf("Sending pulse\n"); // to delete
    TIMER_A2->CCTL[1] &= ~(TIMER_A_CCTLN_CCIE + // clear interrupt flag
            TIMER_A_CCTLN_OUTMOD_MASK);         // mask output
    TIMER_A2->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_0;// output to outmod value
}

/*
 *  Detect time elapsed when HC-SR04 ultrasonic sensor
 *  echos back distance.
 */
void TA0_N_IRQHandler(void) {
    printf("Entering interrupt\n"); // to delete
    if (TIMER_A0->CCTL[2] & TIMER_A_CCTLN_CM_1) {   // rising edge
        printf("Rising edge\n"); // to delete
        save_init_distance_time(TIMER_A0->CCR[2]);  // save initial time
        TIMER_A0->CCTL[2] &= ~TIMER_A_CCTLN_CM_MASK;// mask capture mode
        TIMER_A0->CCTL[2] |= TIMER_A_CCTLN_CM_2;    // capture falling edge
    }
    else {                                          // falling edge
        printf("Falling edge\n"); // to delete
        save_final_distance_time(TIMER_A0->CCR[2]); // save final time
        set_distance_flag();                        // indicate measurement is complete
        TIMER_A0->CCTL[2] &= ~TIMER_A_CCTLN_CCIE;   // disable capture interrupt
    }

    // Clear the interrupt flag
    TIMER_A0->CCTL[2] &= ~(TIMER_A_CCTLN_CCIFG);
}
