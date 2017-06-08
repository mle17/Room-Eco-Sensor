/*
 * UARTChar.h
 *
 *      Created on: April 24, 2017
 *         Authors: Michael Le & Justin L'Heureux
 *      Assignment: 05
 *           Class: CPE 329 - Section 01
 *         Quarter: Spring 2017
 *
 *      Square Wave with 20 ms time period
 *      and 2 Vpp with 1 V DC offset
 */

#ifndef UARTCHAR_H
#define UARTCHAR_H

#define INPUT_SIZE  11
#define ESC         27

#define MAX_DAC     4095
#define MIN_DAC     0

void UART2_init(void);
unsigned int get_DAC_value(char input[INPUT_SIZE]);
int is_proper_DAC(unsigned int DAC_value);
void print_to_terminal(char *msg);
void print_float_to_terminal(float value);

void initSPI();
void Drive_DAC(unsigned int level);

#endif /* UARTCHAR_H_ */
