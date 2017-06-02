/*
 * freqDelay.h
 *
 *      Created on: April 13, 2017
 *         Authors: Michael Le & Justin L'Heuruex
 *         Project: 01
 *           Class: CPE 329 - Section 01
 *         Quarter: Spring 2017
 *
 *  At the highest supported frequency of 24 MHz,
 *  the shortest pulse generated is 1.25 us.
 */
#ifndef FREQDELAY_H
#define FREQDELAY_H

/* Supported frequencies of the MSP432R */
#define FREQ_1_5_MHz    1500000
#define FREQ_3_MHz      3000000
#define FREQ_6_MHz      6000000
#define FREQ_12_MHz    12000000
#define FREQ_24_MHz    24000000
#define FREQ_48_MHz    48000000

/* Conversion constants from integer to float */
#define MS                    0.001
#define NS                    0.000000001

/* Delay functions and their helper functions */
void delay_ms(int num_ms, int sys_freq);
void delay_ns(int num_ns, int sys_freq);

static void delayMs(int n);
static void delayNs(int n);

/* Set system clock */
void set_DCO(int sys_freq);
static void set_CLK_freq(int sys_freq);
static void set_48_MHz();

/* Time check functions */
static int is_proper_time_ms(int time_ms, int sys_freq);
static int is_proper_time_ns(int time_ns, int sys_freq);

static int isEqual(double a, double b, double precision);

#endif /* FREQDELAY_H_ */
