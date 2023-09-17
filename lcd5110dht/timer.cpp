
#include "timer.h"

void stopTimer0()
{
    TA0CTL &= ~MC_3;
    CLR (TA0CCTL0, CCIE);

bool timeOut()
{
    return (TA1CTL & MC_1) == 0x00;
}

/// Not used
///! Warning, this timer works with less voltage than
///! the timer 1 (at least in my hardware...)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void ccr0Isr(void)
{
    stopTimer0();
}

/// To use in delays. Auto stopped after call
#pragma vector = TIMER1_A0_VECTOR
__interrupt void dhtTimer(void)
{
    TA1CTL &= ~MC_3;
    CLR (TA1CCTL0, CCIE);
}


void usInitTimer0(const unsigned *us)
{
    TA0CCR0 = (unsigned) ((*us) / 4);          // Count up to limit
    TA0CTL |= TASSEL_2 + ID_2 + MC_1 + TACLR;
    // SMCLK, div 4, up mode, restart, enable interrup
    TA0CCTL0 |= CCIE;
}

void usInitTimer1(const unsigned *us)
{
    TA1CCR0 = (unsigned) ((*us) / 4);          // Count up to limit
    //TA1CTL = TASSEL_2 | MC_1 | TACLR;
    TA1CTL = TASSEL_2 + ID_2 + MC_1 + TACLR;
    // SMCLK, div 4, up mode, restart
    TA1CCTL0 |= CCIE;          // Enable interrupt
}


//void secInitTimer0(const unsigned * sec)
//{
//    TA0CCR0 = (unsigned long) (50000 * (*sec));
//    TA0CCTL0 = CCIE;				            // Enable interrupt
//    TA0CTL = TASSEL_2 + ID_2 + MC_1 + TACLR;	// SMCLK, div 4, up mode, restart
//}

void msInitTimer1(const unsigned *ms)
{
    // TA1CTL |= TASSEL_1;     // ACLK as source (Current 32768Hz)
    // TA1CTL |= ID_3;         // DIV by 8 the prev source (Current 4096 Hz)
    // TA1CTL |= MC_1;         // Count Up mode
    // TA1CTL |= TACLR;        // Reset timer
    TA1CCTL0 |= CCIE;          // Enable interrupt
    TA1CCR0 = (*ms) * 5;          // Count up to limit
    TA1CTL = TASSEL_1 | ID_3 | MC_1 | TACLR;
}

void msWait(const unsigned *msDelay)
{
    msInitTimer1(msDelay);
    while (true)
    {
        if ((TA1CTL & MC_1) == 0x00)
        {
            break;
        }
    }
}

void usWait(const unsigned *usDelay)
{
    usInitTimer1(usDelay);
    while (true)
    {
        if (timeOut())
        {
            break;
        }
    }
}


//void timer1::upModeTimer0()
//{
//    TA0CTL |= MC_1;
//    TA0CCTL0 |= CCIFG;   // To enable interrupts
//}

void upModeTimer1()
{
    TA1CTL |= MC_1;
    TA1CCTL0 |= CCIFG;   // To enable interrupts
}

void stopTimer1()
{
    TA1CTL &= ~MC_3;         // Stop mode to conserve power
    CLR (TA1CCTL0, CCIFG);   // To allow other interrupts
}
