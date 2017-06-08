/*
 * UARTChar.c
 *
 *      Created on: May 8, 2017
 *         Authors: Michael Le & Justin L'Heureux
 *      Assignment: 07
 *           Class: CPE 329 - Section 01
 *         Quarter: Spring 2017
 *
 *
 */

#include "msp.h"
#include "UARTChar.h"
#include <stdio.h>
#include <stdlib.h>

void UART2_init(void) {
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST;         /* put in reset mode for config */
    EUSCI_A2->MCTLW = 0;                            /* disable oversampling */
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SSEL__SMCLK;   /* SMCLK */
    EUSCI_A2->CTLW0 &= ~(EUSCI_A_CTLW0_SEVENBIT +   /* 8-bit data */
            EUSCI_A_CTLW0_SPB +                     /* 1 stop bit */
            EUSCI_A_CTLW0_PEN);                     /* no parity */
    EUSCI_A2->BRW = 5000;                           /* clock / BAUD_RATE = BRW */
    P3->SEL0 |= BIT2 + BIT3;                        /* P3.2, P3.3 for UART */
    P3->SEL1 &= ~(BIT2 + BIT3);                     /* P3.2 RX, P3.3 TX */
    EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;        /* take UART out of reset mode */
    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;                /* enable receive interrupt */
    EUSCI_A2->IE &= ~EUSCI_A_IE_TXIE;               /* disable transfer interrupt */
}

unsigned int get_DAC_value(char input[INPUT_SIZE]) {
    unsigned int DAC = 0;
    int i = 0;
    unsigned char c;

    while (1) {
        c = input[i];

        if (c == '\0')
            break;

        DAC = DAC*10 + (c - '0');
        i++;
    }
    return DAC;
}

int is_proper_DAC(unsigned int DAC_value) {
    if (DAC_value > MAX_DAC) {
        return 0;
    }
    else {
        return 1;
    }
}

void Drive_DAC(unsigned int level) {
    unsigned int DAC_Word = 0;

    DAC_Word = (0x1000) | (level & 0x0FFF);     // 0x1000 sets DAC for Write
                                                // to DAC, Gain = 2, /SHDN = 1
                                                // and put 12-bit level value
                                                // in low 12 bits.

    P5->OUT &= ~BIT6;                                     // Clear P5.6 (drive /CS low on DAC)
                                                          // Using a port output to do this for now

    EUSCI_B0->TXBUF = (unsigned char) (DAC_Word >> 8);    // Shift upper byte of DAC_Word
                                                          // 8-bits to right

    while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));              // USCI_A0 TX buffer ready?

    EUSCI_B0->TXBUF = (unsigned char) (DAC_Word & 0x00FF);     // Transmit lower byte to DAC

    while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));      // Poll the TX flag to wait for completion
    // Delay 200 16 MHz SMCLK periods (optional)

    P5->OUT |= BIT6;                                   // Set P5.6   (drive /CS high on DAC)

    return;
}

void initSPI() {
    // Configure port bits for SPI
    P5->DIR |= BIT6;                        // Will use BIT6 to activate /CE on the DAC
    P1->SEL0 |= BIT6 + BIT5;                // Configure P1.6 and P1.5 for UCB0SIMO and UCB0CLK
    P1->SEL1 &= ~(BIT6 + BIT5);

    // SPI Setup
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;   // Put eUSCI state machine in reset

    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST |   // Remain eUSCI state machine in reset
                    EUSCI_B_CTLW0_MST   |   // Set as SPI master
                    EUSCI_B_CTLW0_SYNC  |   // Set as synchronous mode
                    EUSCI_B_CTLW0_CKPL  |   // Set clock polarity high
                    EUSCI_B_CTLW0_MSB;      // MSB first

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK; // Use SMCLK as its clock
    EUSCI_B0->BRW = 0x01;                         // divide by 16, clock = fBRCLK/(UCBRx)
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;      // Initialize USCI state machine, SPI
                                                // now waiting for something to
                                                // be placed in TXBUF

    EUSCI_B0->IFG |= EUSCI_B_IFG_TXIFG;  // Clear TXIFG flag
}

void print_to_terminal(char *msg) {
    int i = 0;
    int j;

    while (msg[i] != '\0') {
        EUSCI_A0->TXBUF = msg[i];
        for (j = 0; j < 400; j++) {}
        i++;
    }
}

void print_float_to_terminal(float value) {
    char msg[10];
    int ret;

    ret = sprintf(msg, "%1.1f", value);
    if (ret < 0) {
        perror("Print float to terminal failed\n");
        exit(EXIT_FAILURE);
    }

    print_to_terminal(msg);
}
