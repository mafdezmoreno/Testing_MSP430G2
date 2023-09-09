#include <msp430.h> 
#include "lcd.h"

/**
 * main.c
 */


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
    initLcd();

    setAddr(0, 0);
    writeStringToLcd("Hello", 5);

	return 0;
}
