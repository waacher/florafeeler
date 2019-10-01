/* Main file for Flora Feeler project
 * Includes main function as well as ADC interrupt handler (two main logic blocks)
 */

#include "msp.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "LCD.h"
#include "ADC.h"
#include "freq.h"

#define DEG_SYM 0b11011111

static uint16_t light_value = 0;
static uint16_t temp_value = 0;
static uint16_t moisture_value = 0;

void ADC14_IRQHandler(void) {
    uint16_t analogValue;

    // Moisture
    analogValue = ADC14->MEM[28]; // A8 = 4.5 = moisture sensor
    moisture_value = analogValue * 3300 / 16320;
    moisture_value = 100 - ((moisture_value - 1520) * 100 / 1660); // low values = wet, high values = dry
    if (moisture_value > 65000) {
        moisture_value = 0;
    }

    // Light
    analogValue = ADC14->MEM[29];
    light_value = analogValue / 124;

    // Temperature
    analogValue = ADC14->MEM[30];
    temp_value = analogValue / 100 + 55;
}

int main(void) {
    char buffer[5] = { '\0' };

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;       // Stop watchdog timer

    initADC();
    initLCD();
    __enable_irq();   // Enable global interrupt

    clearDisplay();
    writeString("Water Light Temp");

    ADC14->CTL0 |= ADC14_CTL0_SC;   // start a conversion
    while (1) {
        delay_ms(200, SYS_FREQ);
        clearLine(LINE2);

        sprintf(buffer, "%3d%%  ", moisture_value);
        writeString(buffer);

        sprintf(buffer, "%3d%%  ", light_value);
        writeString(buffer);

        sprintf(buffer, "%3d", temp_value);
        writeString(buffer);
        writeChar(DEG_SYM);

        ADC14->CTL0 |= ADC14_CTL0_SC;
    }
}
