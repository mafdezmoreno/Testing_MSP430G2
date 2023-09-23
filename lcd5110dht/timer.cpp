
#include "timer.h"

#pragma vector = TIMER0_A0_VECTOR
__interrupt void ccr0Isr(void)
{
    TA0CTL &= ~MC_3;
    CLR (TA0CCTL0, CCIE);
#ifdef LOW_POWER_MODE
    __bic_SR_register_on_exit(LPM3_bits);
#endif
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void dhtTimer(void)
{
    TA1CTL &= ~MC_3;
    CLR (TA1CCTL0, CCIE);
}

void timer0::msWait(const unsigned *msDelay)
{
    msInitTimer(msDelay);
    while (true)
    {
        if (timeOut())
        {
            break;
        }
    }
}

void timer0::usWait(const unsigned *usDelay)
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

void timer0::usInitTimer(const unsigned *us)
{
    TA0CCR0 = (unsigned) (*us);          // Count up to limit
    //TA0CTL = TASSEL_2 | MC_1 | TACLR;
    TA0CTL = TASSEL_2 + MC_1 + TACLR;
    // SMCLK, up mode, restart
    TA0CCTL0 |= CCIE;          // Enable interrupt
#ifdef LOW_POWER_MODE
    __bis_SR_register(LPM3_bits);
#endif
}

void timer0::msInitTimer(const unsigned *ms)
{
    // TA0CTL |= TASSEL_1;     // ACLK as source (Current 32768Hz)
    // TA0CTL |= ID_3;         // DIV by 8 the prev source (Current 4096 Hz)
    // TA0CTL |= MC_1;         // Count Up mode
    // TA0CTL |= TACLR;        // Reset timer
    TA0CCTL0 |= CCIE;          // Enable interrupt
    TA0CCR0 = (*ms) * 5;       // Count up to limit
    TA0CTL = TASSEL_1 | ID_3 | MC_1 | TACLR;
#ifdef LOW_POWER_MODE
    __bis_SR_register(LPM3_bits);
#endif
}


bool timer0::timeOut()
{
    return (TA0CTL & MC_1) == 0x00;
}

void timer0::stopTimer()
{
    TA0CTL &= ~MC_3;
    CLR (TA0CCTL0, CCIE);
}

void timer1::msWait(const unsigned *msDelay)
{
    msInitTimer(msDelay);
    while (true)
    {
        if (timeOut())
        {
            break;
        }
    }
}

void timer1::usWait(const unsigned *usDelay)
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

void timer1::usInitTimer(const unsigned *us)
{
    TA1CCR0 = (unsigned) (*us);
    TA1CTL = TASSEL_2 + MC_1 + TACLR;
    TA1CCTL0 |= CCIE;
}

void timer1::msInitTimer(const unsigned *ms)
{
    TA1CCTL0 |= CCIE;
    TA1CCR0 = (*ms) * 5;
    TA1CTL = TASSEL_1 | ID_3 | MC_1 | TACLR;
}


bool timer1::timeOut()
{
    return (TA1CTL & MC_1) == 0x00;
}

void timer1::stopTimer()
{
    TA1CTL &= ~MC_3;
    CLR (TA1CCTL0, CCIE);
}
