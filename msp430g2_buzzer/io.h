/*
 * button.h
 *
 *  Created on: May 28, 2021
 *      Author: mafm
 */

#include "msp430g2452.h"

#ifndef BUTTON_H_
#define BUTTON_H_

#define BUTTON_PORT 1
#define BUTTON_PIN BIT0

#define LED_PORT 2
#define LED_PIN BIT5

void init_button()
{
    switch (BUTTON_PORT)
    {
    case 1 :
        P1DIR &= ~BUTTON_PIN; //set pin as input (by default it's setted as input)
        P1REN |= BUTTON_PIN; //resistor enable
        P1OUT |= BUTTON_PIN; //pull up resistor
        break;

    case 2  :
        P2DIR &= ~BUTTON_PIN;
        P2REN |= BUTTON_PIN;
        P2OUT |= BUTTON_PIN;
        break;
    }
}

int check_state()
{
    switch (BUTTON_PORT)
    {
    case 1 :
        if ((P1IN & BUTTON_PIN) != BUTTON_PIN)
        {
            for (unsigned int i = 300; i > 0; i--);     // delay
            if ((P1IN & BUTTON_PIN) != BUTTON_PIN)
            {
                return 1;
            }
        }

    case 2  :
        if ((P2IN & BUTTON_PIN) != BUTTON_PIN)
        {
            for (unsigned int i = 300; i > 0; i--);     // delay
            if ((P2IN & BUTTON_PIN) != BUTTON_PIN)
            {
                return 0;
            }
        }
    }
    return false;
}

void enable_led()
{
    switch (LED_PORT)
    {
    case 1 :
        P1DIR |= LED_PIN; //Set as output
        break;

    case 2 :
        P2DIR |= LED_PIN;
        break;
    }
}

void disable_led()
{
    switch (LED_PORT)
    {
    case 1 :
        P1DIR &= ~LED_PIN;
        break;

    case 2  :
        P2DIR &= ~LED_PIN;
        break;
    }
}

void led_on()
{
    switch (LED_PORT)
    {
    case 1 :
        P1OUT |= LED_PIN;
        break;

    case 2  :
        P2OUT |= LED_PIN;
        break;
    }
}

void led_off()
{
    switch (LED_PORT)
    {
    case 1 :
        P1OUT &= ~LED_PIN;
        break;

    case 2  :
        P2OUT &= ~LED_PIN;
        break;
    }
}

#endif /* BUTTON_H_ */
