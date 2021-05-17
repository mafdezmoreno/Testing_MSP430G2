//#include <msp430.h>
#include "msp430g2452.h"
/**
 * blink.c
 */

#define PIN_LED BIT3    //BIT1 default msp430g2 launchpad
#define PORT_LED 2      //1 default msp430g2 launchpad

void toggle_led();
void set_output_dir();
void delay_ms(unsigned long ms);
void initTimer_A(void);
void delay_ms_timer(unsigned long ms);

volatile unsigned long OFCount;
const unsigned long pause = 500; //200ms

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; //Stop watchdog timer

    //MCLK=SMCLK=1Mhz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    initTimer_A();
    //_enable_interrupt();
    _BIS_SR(GIE);  //Do the same that the previous line (legacy code)

	set_output_dir();

	unsigned int i = 0;

	delay_ms(1000);

	while(i<50)
	{
	    toggle_led();
	    delay_ms(pause);
	    i++;
	}
	i = 0;
	delay_ms(1000);
	while(i<50)
	{
	    toggle_led();
	    delay_ms_timer(pause);
	    i++;
	}
	/*
	 * Energy consumption:
	 *      "delay_ms()": 8.4mW
	 *      "delay_ms_timer()": 8.4mW
	 */

}

void set_output_dir(){
    switch(PORT_LED) {
       case 1 :
          P1DIR |= PIN_LED;
          P1OUT |= PIN_LED;
          break;

       case 2  :
          P2DIR |= PIN_LED;
          P2OUT |= PIN_LED;
          break;
    }
}

void toggle_led(){

    switch(PORT_LED) {
           case 1 :
              P1OUT ^= PIN_LED;
              break;

           case 2  :
              P2OUT ^= PIN_LED;
              break;
        }
}

void delay_ms(unsigned long ms)
{
    while (ms)
    {
        __delay_cycles(1000);// 1000 for 1MHz and 16000 for 16MHz
        ms--;
    }
}

void initTimer_A(void)
{
    //Timer0_A3 Configuration
    TACCR0   = 0; //Initially, Stop the Timer
    TACCTL0 |= CCIE; //Enable interrupt for CCR0.
    TACTL    = TASSEL_2 + ID_0 + MC_1; //Select SMCLK, SMCLK/1, Up Mode
}

void delay_ms_timer(unsigned long ms)
{
    OFCount = 0; //Reset Over-Flow counter
    TACCR0 = 1000-1; //Start Timer, Compare value for Up Mode to get 1ms delay per loop
    //Total count = TACCR0 + 1. Hence we need to subtract 1.

    while(OFCount <= ms);

    TACCR0 = 0; //Stop Timer
}

//Timer ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR0_ISR(void)
{
    OFCount++; //Increment Over-Flow Counter
}

