#include <msp430.h>				


/**
 * blink.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	P2DIR |= BIT2;					// configure P2.2 as output

	volatile unsigned int i;		// volatile to prevent optimization

	while(1)
	{
		P2OUT ^= BIT2;				// toggle P2.2
		for(i=10000; i>0; i--);     // delay
	}
}
