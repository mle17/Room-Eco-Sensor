/*
 * freqDelay.c
 *
 *      Created on: April 13, 2017
 *         Authors: Michael Le & Justin L'Heuruex
 *         Project: 01
 *           Class: CPE 329 - Section 01
 *         Quarter: Spring 2017
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "msp.h"
#include "freqDelay.h"

void delay_ms(int num_ms, int sys_freq) {
    int ms;

    switch (sys_freq) {
        case FREQ_1_5_MHz:
            ms = num_ms / 2;
            delayMs(ms);
        break;

        case FREQ_3_MHz:
            ms = num_ms;
            delayMs(ms);
        break;

        case FREQ_6_MHz:
            ms = num_ms * 2;
            delayMs(ms);
        break;

        case FREQ_12_MHz:
            ms = num_ms * 4;
            delayMs(ms);
        break;

        case FREQ_24_MHz:
            ms = num_ms * 8;
            delayMs(ms);
        break;

        case FREQ_48_MHz:
            ms = num_ms * 16;
            delayMs(ms);
        break;

        default:
            fprintf(stderr, "Frequency not supported\n");
            exit(EXIT_FAILURE);
    }
}

void delay_ns(int num_ns, int sys_freq) {
    int ns;

    switch (sys_freq) {
        case FREQ_1_5_MHz:
            ns = num_ns / 2;
            delayNs(ns);
        break;

        case FREQ_3_MHz:
            ns = num_ns;
            delayNs(ns);
        break;

        case FREQ_6_MHz:
            ns = num_ns * 2;
            delayNs(ns);
        break;

        case FREQ_12_MHz:
            ns = num_ns * 4;
            delayNs(ns);
        break;

        case FREQ_24_MHz:
            ns = num_ns * 8;
            delayNs(ns);
        break;

        case FREQ_48_MHz:
            ns = num_ns * 16;
            delayNs(ns);
        break;

        default:
            fprintf(stderr, "Frequency not supported\n");
            exit(EXIT_FAILURE);
    }
}

void set_DCO(int sys_freq) {
    switch (sys_freq) {
        case FREQ_1_5_MHz:
            set_CLK_freq(CS_CTL0_DCORSEL_0);    /* setting to 1.5 MHz */
        break;

        case FREQ_3_MHz:
            set_CLK_freq(CS_CTL0_DCORSEL_1);    /* setting to 3 MHz */
        break;

        case FREQ_6_MHz:
            set_CLK_freq(CS_CTL0_DCORSEL_2);    /* setting to 6 MHz */
        break;

        case FREQ_12_MHz:
            set_CLK_freq(CS_CTL0_DCORSEL_3);    /* setting to 12 MHz */
        break;

        case FREQ_24_MHz:
            set_CLK_freq(CS_CTL0_DCORSEL_4);    /* setting to 24 MHz */
        break;

        case FREQ_48_MHz:
            set_48_MHz();
        break;

        default:
            fprintf(stderr, "Frequency not supported\n");
            exit(EXIT_FAILURE);
    }
}

void set_48_MHz() {
    /* Step 1: Transition to VCORE Level 1: AM0_LDO --> AM1_LDO */
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
        PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));

    /* Step 2: Configure Flash wait-state to 1 for both banks 0 & 1 */
    FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) |
            FLCTL_BANK0_RDCTL_WAIT_1;
    FLCTL->BANK1_RDCTL  = (FLCTL->BANK0_RDCTL & ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) |
            FLCTL_BANK1_RDCTL_WAIT_1;

    /* Step 3: Configure DCO to 48MHz, ensure MCLK uses DCO as source*/
    CS->KEY = CS_KEY_VAL ;                  // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_5;           // Set DCO to 48MHz
    /* Select MCLK = DCO, no divider */
    CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK) |
            CS_CTL1_SELM_3;
    CS->KEY = 0;                            // Lock CS module from unintended accesses
}

void set_CLK_freq(int clk_freq) {
    CS->KEY = CS_KEY_VAL;                                           // unlock CS registers
    CS->CTL0 = 0;                                                   // clear register CTL0
    CS->CTL0 = clk_freq;                                            // set CTL0 register to configure system frequency
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;    // select clock sources
    CS->KEY = 0;                                                    // lock the CS registers
}

/* delay milliseconds when system clock is at 3 MHz for Rev C MCU */
void delayMs(int n) {
    int i, j;

    for (j = 0; j < n; j++)
        for (i = 300; i > 0; i--);      /* Delay 1 ms*/
}

void delayNs(int n) {
    int j;
    int m = n/3400;

    for (j = 0; j < m; j++) {}
}

int is_proper_time_ms(int time_ms, int sys_freq) {
    double time_per = (double) 1.0/sys_freq;
    double input_ms = (double) time_ms/1000.0;

    /* checking if user time is >= clock period */
    if (input_ms < time_per) {
        return 0;
    }

    /* checking if user time is multiple of clock period */
    if (!isEqual(fmod(input_ms, time_per), 0.0, MS)) {
        return 0;
    }

    return 1;
}

int is_proper_time_ns(int time_ns, int sys_freq) {
    double time_per = (double) 1.0/sys_freq;
    double input_ns = (double) time_ns/1000000000.0;

    /* checking if user time is >= clock period */
    if (input_ns < time_per) {
        return 0;
    }

    /* checking if user time is multiple of clock period */
    if (!isEqual(fmod(input_ns, time_per), 0.0, NS)) {
        return 0;
    }

    return 1;
}

int isEqual(double a, double b, double precision) {
    if(fabs(a - b) < precision)
        return 1;
    else
        return 0;
}
