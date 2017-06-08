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
#include "UARTChar.h"
#include "lcd.h"
#include <stdio.h>

static int i = 0;
static unsigned short init, final, diff;

void main(void) {
    float distance = 0;
    char msg[50];

    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    distance_init();    // Use P5.6 TA2.1 to send pulse
                        // Use P2.5 TA0.2 to measure pulse length
    init_UI();          // Use P4.0-2 P4.4-7 for LCD

    set_DCO(FREQ_48_MHz);
    UART2_init();       // Use P3.2 RX, P3.3 TX for UART

    P5->DIR |= BIT1;
    P5->OUT &= ~BIT1;


    /* Enable global interrupt */
    __enable_irq();

    /* Enable Timer A2 Interrupts */
    //NVIC_EnableIRQ(TA2_N_IRQn);
    NVIC_EnableIRQ(TA0_N_IRQn);
    NVIC_EnableIRQ(EUSCIA2_IRQn);

    delay_ms(20, FREQ_48_MHz);
    start_meas_distance();

    while (1) {
        if (check_distance_flag()) {
            distance = get_distance();
            P5->OUT |= BIT1;
            //delay_ms(74, FREQ_48_MHz);
            printf("Distance: %1.2f\n", distance);    // to delete
            P5->OUT &= ~BIT1;
            //delay_ms(3, FREQ_48_MHz);
            update_UI_distance(distance);
            start_meas_distance();
            clear_distance_flag();
            //TIMER_A2->CCTL[1] &= ~TIMER_A_CCTLN_OUTMOD_MASK;// mask output
            //TIMER_A2->CCTL[1] |= TIMER_A_CCTLN_CCIE |       // set interrupt flag
            //        TIMER_A_CCTLN_OUTMOD_7;                 // output reset/set mode
            //TIMER_A2->CTL |= TIMER_A_CTL_CLR;               // clear TA2R register
            //printf("Main IE flag: %d\n", TIMER_A2->CCTL[1] & TIMER_A_CCTLN_CCIE); // to delete
        }
    }
}

/*
 *  After pulse to trigger HC-SR04 elapsed,
 *  turn off pulse, then turn off interrupt.
 */
void TA2_N_IRQHandler(void) {
    //delay_ms(5, FREQ_48_MHz);
    TIMER_A2->CCTL[1] &= ~(TIMER_A_CCTLN_CCIE + // clear interrupt flag
            TIMER_A_CCTLN_OUTMOD_MASK);         // mask output
    TIMER_A2->CCTL[1] |= TIMER_A_CCTLN_OUTMOD_0;// output to outmod value
    TIMER_A2->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
}

/*
 *  Detect time elapsed when HC-SR04 ultrasonic sensor
 *  echos back distance.
 */
void TA0_N_IRQHandler(void) {
    if (i % 2) {    // start of pulse
        init = TIMER_A0->CCR[2];
    }
    else {          // end of pulse
        final = TIMER_A0->CCR[2];
        diff = final - init;
        save_pulse_time(diff);
        TIMER_A2->CCTL[1] &= ~TIMER_A_CCTLN_CCIE;
        set_distance_flag();
    }
    i++;

    // Clear the interrupt flag
    TIMER_A0->CCTL[2] &= ~(TIMER_A_CCTLN_CCIFG);
}

void EUSCIA2_IRQHandler(void) {
    char rec;
    printf("Enter UART interrupt\n"); // to delete

    rec = EUSCI_A2->RXBUF;
    printf("%c", rec);
}
