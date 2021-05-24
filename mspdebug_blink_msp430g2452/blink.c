//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.2
//
//  Description; Toggle P1.2 by xor'ing P1.2 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430x5xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.2|-->LED
//
//  Texas Instruments, Inc
//  July 2011
//***************************************************************************************

//#include <msp430.h>
#include <msp430g2452.h>		

int main() {
	
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer
	
	P2DIR |= BIT2;					// Set P1.2 to output direction
	P1DIR |= BIT0;

	for(;;) {
		volatile unsigned long i;	// volatile to prevent optimization

		P2OUT ^= BIT2;				// Toggle P2.2
		P1OUT ^= BIT0;				// Toggle P1.0

		i = 10000;					// SW Delay
		do i--;
		while(i != 0);
	}
	return 0;
}
