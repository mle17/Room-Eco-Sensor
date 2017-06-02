/*
 * lcd.h
 *
 *      Created on: April 13, 2017
 *         Authors: Michael Le & Justin L'Heuruex
 *         Project: 01
 *           Class: CPE 329 - Section 01
 *         Quarter: Spring 2017
 */
#ifndef LCD_H
#define LCD_H

#include "msp.h"

#define P4_LCD_PORTS    (BIT0 + BIT1 + BIT2 + BIT4 + BIT5 + BIT6 + BIT7)

#define RS              0x01    /* P4.0 mask */
#define RW              0x02    /* P4.1 mask */
#define EN              0x04    /* P4.2 mask */

#define FIRST_LINE      0x80
#define SECOND_LINE     0xC0

void Clear_LCD();
void Home_LCD();

void SetCursorLocation(int location);
void WriteCharacter(char letter);
void WriteLine(char *line);

void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);

#endif /* LCD_H_ */
