#include "msp.h"

#ifndef LCD_H_
#define LCD_H_

#define LINE1 0x00
#define LINE2 0x40

/* Port Map
 * P5.0 -> RS
 * P5.1 -> R/W
 * P5.2 -> E
 * P2.4 -> DB4
 * P2.5 -> DB5
 * P2.6 -> DB6
 * P2.7 -> DB7
 */

// Functions in LCD.c
void toggleEnable();
void dataOutput(uint8_t c);
void cursorHome();
void clearDisplay();
void clearLine(uint8_t lineNum);
void setDDRAM(uint8_t lineNum);
void writeChar(char c);
void writeString(char* string);
void writeDegree();
void initLCD();

#endif /* DELAY_H_ */
