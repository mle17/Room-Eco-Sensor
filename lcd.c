/*
 * lcd.c
 *
 *      Created on: April 13, 2017
 *         Authors: Michael Le & Justin L'Heuruex
 *         Project: 01
 *           Class: CPE 329 - Section 01
 *         Quarter: Spring 2017
 */
#include <string.h>
#include "msp.h"
#include "lcd.h"
#include "freqDelay.h"

void Clear_LCD() {
    P4->OUT &= ~P4_LCD_PORTS;       /* clear ports to be used for LCD */
    LCD_command(0x01);
}

void Home_LCD() {
    P4->OUT &= ~P4_LCD_PORTS;       /* clear ports to be used for LCD */
    LCD_command(0x02);
}

void SetCursorLocation(int location) {
    P4->OUT &= ~P4_LCD_PORTS;       /* clear ports to be used for LCD */
    LCD_command(location);
}

void WriteCharacter(char letter) {
    P4->OUT &= ~P4_LCD_PORTS;       /* clear ports to be used for LCD */
    LCD_data(letter);
}

void WriteLine(char *line) {
    int i;

    for (i = 0; i < strlen(line); i++) {
        WriteCharacter(line[i]);
    }
}

void LCD_init(void) {
    P4->DIR |= P4_LCD_PORTS;    /* make P4 pins output for data and controls */
    delay_ms(30, FREQ_3_MHz);   /* initialization sequence */
    LCD_nibble_write(0x30, 0);
    delay_ms(10, FREQ_3_MHz);
    LCD_nibble_write(0x30, 0);
    delay_ms(1, FREQ_3_MHz);
    LCD_nibble_write(0x30, 0);
    delay_ms(1, FREQ_3_MHz);
    LCD_nibble_write(0x20, 0);  /* use 4-bit data mode */
    delay_ms(1, FREQ_3_MHz);

    LCD_command(0x28);      /* set 4-bit data, 2-line, 5x7 font */
    LCD_command(0x06);      /* move cursor right after each char */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}

/*
 * Data is the command/data to be sent to the LCD
 * Sends only the upper nibble for data
 *
 * Control are the bits for Enable, R/W, and RS
 */
void LCD_nibble_write(unsigned char data, unsigned char control) {
    data &= 0xF0;                   /* clear lower nibble for control */
    control &= 0x0F;                /* clear upper nibble for data */

    P4->OUT = data | control;       /* RS = 0, R/W = 0 */
    P4->OUT = data | control | EN;  /* pulse E */
    delay_ms(1, FREQ_3_MHz);

    P4->OUT = data;                 /* clear E */
    P4->OUT = 0;
}

void LCD_command(unsigned char command) {
    LCD_nibble_write(command & 0xF0, 0);    /* upper nibble first */
    LCD_nibble_write(command << 4, 0);      /* then lower nibble */

    if (command < 4)
        delay_ms(4, FREQ_3_MHz);            /* commands 1 and 2 need up to 1.64ms */
    else
        delay_ms(1, FREQ_3_MHz);            /* all others 40 us */
}

void LCD_data(unsigned char data) {
    LCD_nibble_write(data & 0xF0, RS);    /* upper nibble first */
    LCD_nibble_write(data << 4, RS);      /* then lower nibble  */

    delay_ms(1, FREQ_3_MHz);
}
