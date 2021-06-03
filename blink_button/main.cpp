#include <msp430.h> 


/**
 * main.c
 */

#define BUTTON1   BIT3 // Port 1.0
#define BUTTON2   BIT0 // Port 2.0
#define REDLED   BIT4 // Port 2.4
#define GRNLED   BIT5 // Port 2.5

void main(void)
{

    WDTCTL = WDTPW + WDTHOLD; // stop watchdog (password + hold counter)

    // LED initial setup
    P2DIR |= REDLED + GRNLED;             // set outputs pins for leds
    P2OUT |= REDLED;                      // Enable REDLED
    P2OUT &= ~GRNLED;                     // Disable GRNLED

    // Button1 setup
    P1DIR &= ~BUTTON1;                     // button is an input
    P1OUT |= BUTTON1;                      // pull-up resistor
    P1REN |= BUTTON1;                      // resistor enabled

    // Button2 setup
    P2DIR &= ~BUTTON2;                     // button is an input
    P2OUT |= BUTTON2;                      // pull-up resistor
    P2REN |= BUTTON2;                      // resistor enabled


    while(1)
    {
        if((P1IN & BUTTON1)!=BUTTON1)
        {
            __delay_cycles(220000);
            P2OUT ^= (REDLED+GRNLED);
        }
        if((P2IN & BUTTON2)!=BUTTON2)
        {
            __delay_cycles(220000);
            P2OUT ^= (REDLED+GRNLED);
        }
    }
}
