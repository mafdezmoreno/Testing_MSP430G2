#include "msp430g2553.h"
#include "lcd.h"

void initMcu()
{
    WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer

    BCSCTL1= CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    __bis_SR_register(GIE); // Enable interrupts
}

int main()
{
    initMcu();
    lcd lcd;

    while(1)
    {
        lcd.printDht();
    }
}
