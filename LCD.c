/* LCD functions for initializing and writing ascii characters */

#include <msp.h>
#include <string.h>
#include "LCD.h"
#include "delay.h"
#include "freq.h"

// Pulse enable > 460 ns
void toggleEnable() {
    P5->OUT |= BIT2; // set E low
    delay_us(0, SYS_FREQ);
    P5->OUT &= ~BIT2; // set E low
}

// Send data for output to LCD
void dataOutput(uint8_t c) {
    // c used for upper 4 bits, copy used for lower 4 bits
    uint8_t copy = c;
    copy &= ~(BIT0 | BIT1 | BIT2 | BIT3); // clear copy lower 4 bits
    P2->OUT &= ~(BIT7 | BIT6 | BIT5 | BIT4); //clear P2 upper 4 bits
    P2->OUT |= copy; // clear lower 4 bits
    toggleEnable();
    P2->OUT &= ~(BIT7 | BIT6 | BIT5 | BIT4); //clear P2 upper 4 bits
    c = c << 4; // clear c upper 4 bits by bit shifting
    //c |= (BIT0 | BIT1 | BIT2 | BIT3);
    P2->OUT |= c;
    toggleEnable();
}

// move cursor to top left of LCD
void cursorHome() {
    P5->OUT &= ~(BIT0 | BIT1 | BIT2); // set RS, R/W, and E to 0
    P2->OUT &= ~(BIT7 | BIT6 | BIT5 | BIT4); // bit 0 holds value
    toggleEnable();
    P2->OUT &= ~(BIT7 | BIT6 | BIT5);
    P2->OUT |= BIT5; // set bit 1 to 1
    toggleEnable();
    delay_us(2000, SYS_FREQ);
}

// clear all characters on the LCD
void clearDisplay() {
    P5->OUT &= ~(BIT0 | BIT1 | BIT2); // set RS, R/W, and E to 0
    P2->OUT &= ~(BIT7 | BIT6 | BIT5 | BIT4);
    toggleEnable();
    P2->OUT &= ~(BIT7 | BIT6 | BIT5 | BIT4); // clear all bits
    P2->OUT |= BIT4; // set bit 0 to 1
    toggleEnable();
    delay_us(2000, SYS_FREQ);
}

void clearLine(uint8_t lineNum) {
    setDDRAM(lineNum);
    writeString("                  ");      // Figure out exact number?
    setDDRAM(lineNum);
}

// Change memory in LCD (cursor location)
void setDDRAM(uint8_t lineNum) {
    uint8_t upperBits = lineNum;
    P5->OUT &= ~(BIT0 | BIT1 | BIT2); // set RS, R/W, and E to 0
    upperBits &= ~(BIT0 | BIT1 | BIT2 | BIT3); // clear lower 4 bits
    P2->OUT &= ~(BIT7 | BIT6 | BIT5 | BIT4); //clear P2 upper 4 bits
    P2->OUT |= BIT7; // bit 7 is high
    P2->OUT |= upperBits; // set upper 3 bits
    toggleEnable();
    P2->OUT &= ~(BIT7 | BIT6 | BIT5 | BIT4); //clear P2 lower 4 bits
    lineNum = lineNum << 4;
    lineNum |= (BIT0 | BIT1 | BIT2 | BIT3);
    toggleEnable();
}

// Display character to LCD
void writeChar(char c) {
    P5->OUT |= BIT0; // set RS high
    P5->OUT &= ~BIT1; // set RW low
    P5->OUT &= ~BIT2; // set E low
    dataOutput(c);
    delay_us(40, SYS_FREQ); // what should this be?
}

// Display string to LCD
void writeString(char* string) {
    int i;
    for (i=0; i < strlen(string); i++) {
        if (string[i] != '\0') {// change line support?
            writeChar(string[i]);
        }
    }
}

void writeDegree() {
    P2->OUT |= (BIT0 | BIT1 | BIT2 | BIT3);
    toggleEnable();
    P2->OUT |= (BIT7 | BIT6 | BIT4);
    P2->OUT &= ~BIT5;
    toggleEnable();
}

// Setup for LCD operation
void initLCD() {
    P2->DIR |= (BIT4 | BIT5 | BIT6 | BIT7);
    P5->DIR |= (BIT0 | BIT1 | BIT2);

    delay_us(50000, SYS_FREQ);

    P5->OUT &= ~BIT2;           // Clear enable

    P5->OUT &= ~BIT0;           // Clear RS
    P5->OUT &= ~BIT1;           // Clear R/W
    P5->OUT &= ~BIT2;
    P2->OUT &= ~BIT7;           // Data = 0011
    P2->OUT &= ~BIT6;
    P2->OUT |= BIT5;
    P2->OUT |= BIT4;
    toggleEnable();

    delay_us(40, SYS_FREQ);     // Wait > 37 us

    P5->OUT &= ~BIT0;           // Clear RS
    P5->OUT &= ~BIT1;           // Clear R/W
    P5->OUT &= ~BIT2;
    P2->OUT &= ~BIT7;           // Data = 0010
    P2->OUT &= ~BIT6;
    P2->OUT |= BIT5;
    P2->OUT &= ~BIT4;
    toggleEnable();

    P5->OUT &= ~BIT0;           // Clear RS
    P5->OUT &= ~BIT1;           // Clear R/W
    P2->OUT |= BIT7;            // Data = NFXX = 10XX
    P2->OUT &= ~BIT6;
    toggleEnable();

    delay_us(40, SYS_FREQ);     // Wait > 37 us

    P5->OUT &= ~BIT0;           // Clear RS
    P5->OUT &= ~BIT1;           // Clear R/W
    P5->OUT &= ~BIT2;
    P2->OUT &= ~BIT7;           // Data = 0010
    P2->OUT &= ~BIT6;
    P2->OUT |= BIT5;
    P2->OUT &= ~BIT4;
    toggleEnable();

    P5->OUT &= ~BIT0;           // Clear RS
    P5->OUT &= ~BIT1;           // Clear R/W
    P5->OUT &= ~BIT2;
    P2->OUT |= BIT7;            // Data = NFXX = 10XX
    P2->OUT &= ~BIT6;
    toggleEnable();

    delay_us(40, SYS_FREQ);     // Wait > 37 us

    P5->OUT &= ~BIT0;           // Clear RS
    P5->OUT &= ~BIT1;           // Clear R/W
    P5->OUT &= ~BIT2;
    P2->OUT &= ~BIT7;           // Data = 0000
    P2->OUT &= ~BIT6;
    P2->OUT &= ~BIT5;
    P2->OUT &= ~BIT4;
    toggleEnable();

    P5->OUT &= ~BIT0;           // Clear RS
    P5->OUT &= ~BIT1;           // Clear R/W
    P5->OUT &= ~BIT2;
    P2->OUT |= BIT7;            // Data = 1DCB = 1100
    P2->OUT |= BIT6;
    P2->OUT &= ~BIT5;
    P2->OUT &= ~BIT4;
    toggleEnable();

    delay_us(40, SYS_FREQ);     // Wait > 37 us

    P5->OUT &= ~BIT0;           // Clear RS
    P5->OUT &= ~BIT1;           // Clear R/W
    P5->OUT &= ~BIT2;
    P2->OUT &= ~BIT7;           // Data = 0000
    P2->OUT &= ~BIT6;
    P2->OUT &= ~BIT5;
    P2->OUT &= ~BIT4;
    toggleEnable();

    P5->OUT &= ~BIT0;           // Clear RS
    P5->OUT &= ~BIT1;           // Clear R/W
    P5->OUT &= ~BIT2;
    P2->OUT &= ~BIT7;           // Data = 0001
    P2->OUT &= ~BIT6;
    P2->OUT &= ~BIT5;
    P2->OUT |= BIT4;
    toggleEnable();

    delay_us(2000, SYS_FREQ);     // Wait > 1.52 ms

    P5->OUT &= ~BIT0;           // Clear RS
    P5->OUT &= ~BIT1;           // Clear R/W
    P5->OUT &= ~BIT2;
    P2->OUT &= ~BIT7;           // Data = 0000
    P2->OUT &= ~BIT6;
    P2->OUT &= ~BIT5;
    P2->OUT &= ~BIT4;
    toggleEnable();

    P5->OUT &= ~BIT0;           // Clear RS
    P5->OUT &= ~BIT1;           // Clear R/W
    P5->OUT &= ~BIT2;
    P2->OUT &= ~BIT7;           // Data = 01I/DS = 0110
    P2->OUT |= BIT6;
    P2->OUT |= BIT5;
    P2->OUT &= ~BIT4;
    toggleEnable();
}
