/* Function for initializing the ADC function of the MSP432
 * Using pins 4.7, 5.5, 5.4:
 * Moisture sensor, photoresistor, thermistor
 * Memory registers 28, 29, and 30 are used */

#include "msp.h"
#include "ADC.h"

void initADC() {
    ADC14->CTL0 &= ~ADC14_CTL0_ENC;
    ADC14->CTL0 = ADC14_CTL0_SHP
                | ADC14_CTL0_SSEL_4
                | ADC14_CTL0_CONSEQ_1   // Sequence of channels
                | ADC14_CTL0_SHT0_5
                | ADC14_CTL0_ON;
    ADC14->CTL1 = (28 << ADC14_CTL1_CSTARTADD_OFS) | ADC14_CTL1_RES_3;
    ADC14->MCTL[28] = ADC14_MCTLN_INCH_6;           // A6 = 4.7 MOISTURE SENSOR
    ADC14->MCTL[29] = ADC14_MCTLN_INCH_0;           // A0 = 5.5 PHOTORESISTOR
    ADC14->MCTL[30] = ADC14_MCTLN_INCH_1;           // A1 = 5.4 THERMISTOR
    ADC14->MCTL[30] |= ADC14_MCTLN_EOS;
    ADC14->IER0 |= ADC14_IER0_IE28;
    ADC14->IER0 |= ADC14_IER0_IE29;
    ADC14->IER0 |= ADC14_IER0_IE30;

    ADC14->CTL0 |= ADC14_CTL0_ENC;

    NVIC->ISER[0] = (1 << (ADC14_IRQn & 31));

    P5->SEL0 |= BIT4 | BIT5;           // A function
    P5->SEL1 |= BIT4 | BIT5;

    P4->SEL0 |= BIT7;
    P4->SEL1 |= BIT7;

}
