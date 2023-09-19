
#include "timer.h"

bool timer::timeOut()
{
    return (TA0CTL & MC_1) == 0x00;
}

/// Not used
///! Warning, this timer works with less voltage than
///! the timer 1 (at least in my hardware...)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void ccr0Isr(void)
{
    TA0CTL &= ~MC_3;
    CLR (TA0CCTL0, CCIE);
}

/// To use in delays. Auto stopped after call
#pragma vector = TIMER1_A0_VECTOR
__interrupt void dhtTimer(void)
{
    TA1CTL &= ~MC_3;
    CLR (TA1CCTL0, CCIE);
}

void timer::usInitTimer(const unsigned *us)
{
    TA0CCR0 = (unsigned) ((*us) / 4);          // Count up to limit
    //TA0CTL = TASSEL_2 | MC_1 | TACLR;
    TA0CTL = TASSEL_2 + ID_2 + MC_1 + TACLR;
    // SMCLK, div 4, up mode, restart
    TA0CCTL0 |= CCIE;          // Enable interrupt
}

void timer::msInitTimer(const unsigned *ms)
{
    // TA0CTL |= TASSEL_1;     // ACLK as source (Current 32768Hz)
    // TA0CTL |= ID_3;         // DIV by 8 the prev source (Current 4096 Hz)
    // TA0CTL |= MC_1;         // Count Up mode
    // TA0CTL |= TACLR;        // Reset timer
    TA0CCTL0 |= CCIE;          // Enable interrupt
    TA0CCR0 = (*ms) * 5;          // Count up to limit
    TA0CTL = TASSEL_1 | ID_3 | MC_1 | TACLR;
}

void timer::msWait(const unsigned *msDelay)
{
    msInitTimer(msDelay);
    while (true)
    {
        if ((TA0CTL & MC_1) == 0x00)
        {
            break;
        }
    }
}

void timer::usWait(const unsigned *usDelay)
{
    usInitTimer(usDelay);
    while (true)
    {
        if (timeOut())
        {
            break;
        }
    }
}

void timer::stopTimer()
{
    TA0CTL &= ~MC_3;
    CLR (TA0CCTL0, CCIE);
}
