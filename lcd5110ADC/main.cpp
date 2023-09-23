/*
 * Example code to get the analog read from A2 and print the result
 * to the lcd5110 display
 */

#include "msp430g2553.h"
#include "lcd.h"
#include <stdio.h>

#define DISPLAY_LED_BIT BIT5
#define LED_BIT BIT4

void init();

void initLed();
void ledOn();
void ledOff();

void enableDisplayLight();
void displayLedOn();

void initAdc2();
unsigned a2Read();
void stopAdc2();

int main(void)
{
    init();
    char counter = '0';
    for(;;)
    {
        char read[5]{"0000"};
        initAdc2();
        sprintf(read, "%d", a2Read());
        stopAdc2();

        initLcd();
        setAddr(0, 0);
        writeCharToLcd(counter);
        setAddr(0, 1);
        writeStringToLcd(read, 10);
        setAddr(0, 2);
        writeStringToLcd("done", 4);
        __delay_cycles(500000);

        counter++;
        if (counter > '9')
        {
            counter = '0';
        }
    }
}

void init()
{
    WDTCTL = WDTPW + WDTHOLD;   // Stop WDT
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;

    initLed();
    ledOff();
    initLcd();
    enableDisplayLight();
    displayLedOn();
    setAddr(0, 0);
    writeStringToLcd("start", 5);
    __delay_cycles(100000);
}

void initAdc2()
{
    ADC10CTL1 = INCH_2;     // input A2
    ADC10CTL0 = SREF_1 + ADC10SHT_2 + REFON + ADC10ON + ADC10IE;
    ADC10AE0 |= BIT2;
}

unsigned a2Read()
{
    ADC10CTL0 |= ENC + ADC10SC;         // Sampling and conversion start
    __bis_SR_register(CPUOFF + GIE);    // LPM0, ADC10_ISR will force exit
    return ADC10MEM;
}

void stopAdc2()
{
    ADC10CTL0 = 0x00;
    ADC10CTL1 = 0x00;
    ADC10AE0 = 0x00;
}

void initLed()
{
    P2DIR |= LED_BIT;
}

void ledOn()
{
    P2OUT |= LED_BIT;
}

void ledOff()
{
    P2OUT &= ~LED_BIT;
}

void enableDisplayLight()
{
    P2DIR |= DISPLAY_LED_BIT;
}

void displayLedOn()
{
    P2OUT &= ~DISPLAY_LED_BIT;
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    __bic_SR_register_on_exit(CPUOFF);
}