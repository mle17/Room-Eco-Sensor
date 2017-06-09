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
#include "ecoRoom.h"
#include <stdio.h>

static int i = 0;
static unsigned short init, final, diff;

void main(void) {
    float distance = 0;
    int num_occupants;

    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    distance_init();            // Use P5.6 TA2.1 to send pulse
                                // Use P2.5 TA0.2 to measure pulse length
    init_UI();                  // Use P4.0-2 P4.4-7 for LCD
    init_outside_detection();   // Use P1.6 to check if other sensor trigger

    set_DCO(FREQ_48_MHz);
    UART2_init();       // Use P3.2 RX, P3.3 TX for UART

    P5->DIR |= BIT1;
    P5->OUT &= ~BIT1;


    /* Enable global interrupt */
    __enable_irq();

    /* Enable Timer A2 Interrupts */
    //NVIC_EnableIRQ(TA2_N_IRQn);
    NVIC_EnableIRQ(TA0_N_IRQn);
    NVIC_EnableIRQ(PORT1_IRQn);
    //NVIC_EnableIRQ(EUSCIA2_IRQn);

    delay_ms(20, FREQ_48_MHz);
    start_meas_distance();

    while (1) {
        if (check_distance_flag()) {
            distance = get_distance();

            //delay_ms(74, FREQ_48_MHz);
            printf("Distance: %1.2f\n", distance);    // to delete
            update_UI_distance(distance);

            /* checking if someone entered door */
            if (distance < DOOR_DISTANCE && distance > MIN_DISTANCE) {
                /* check if someone enter or exit room */
                if (check_outside_flag()) { // enter room
                    clear_outside_flag();
                    inc_occupants_LCD();
                }
                else {                      // exit room
                    dec_occupants_LCD();
                }

                /* adjust LED based on number of occupants */
                num_occupants = get_num_occupants();
                if (num_occupants > 0) {
                    P3->OUT |= BIT0;
                }
                else {
                    P3->OUT &= ~BIT0;
                }
                delay_ms(2000, FREQ_48_MHz);
            }

            start_meas_distance();
            clear_distance_flag();
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

void PORT1_IRQHandler(void) {
    set_outside_flag();
    P1->IFG &= ~BIT6;       // clear interrupt flag
}
