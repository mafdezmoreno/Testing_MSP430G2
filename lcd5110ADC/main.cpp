/*
 * Example code to get the analog read from A2 and print the result
 * to the lcd5110 display
 */

#include "msp430g2553.h"
#include "lcd.h"
#include "adc.h"
#include <stdio.h>

#define DISPLAY_LED_BIT BIT5
#define LED_BIT BIT4

void init();

void initLed();
void ledOn();
void ledOff();

void enableDisplayLight();
void displayLedOn();

int main(void)
{
    init();
    adc2 a;
    batLvl bL(&a);
    char counter = '0';
    for(;;)
    {
        char read[5]{"0000"};
        a.getRead();
        sprintf(read, "%d", a.getRead());

        initLcd();
        setAddr(0, 0);
        writeCharToLcd(counter);
        setAddr(0, 1);
        writeStringToLcd(read, 10);
        setAddr(0, 2);
        writeStringToLcd("done", 4);

        char batLvl[4]{"B:0"};
        batLvl[2] = bL.getBatLvl();
        setAddr(30, 0);
        writeStringToLcd(batLvl, 3);
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
