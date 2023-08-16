/**
 * Program to test and look for the appropriate sound (frequency pulses) for a buzzer.
 * With a push button the sound it's activated.
 */

#define TI  // Ti Compiler
//#define GNU // GNU Compiler

#ifdef GNU
#include <msp430.h>
#endif
#ifdef TI
#include "msp430g2553.h"
#endif
#include "buzzer.h"
#include "io.h"

int buzzer_on = 0;

int main(void)
{

    init_button();
    init_clocks();
    initTimer_A();
    init_buzzer();
    _bis_SR_register(GIE); //Enable interrupts

    up_timer_A();
    while (1)
    {
        buzzer_on = check_state();
        if (buzzer_on)
        {
            enable_led();
            init_buzzer();
        }
        else
        {
            disable_led();
            clear_buzzer_pin();
        }
    }
}

#ifdef TI
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
#endif
#ifdef GNU
__attribute__ ( ( interrupt( TIMER0_A0_VECTOR ) ) )
void TIMER0_A0_ISR( void )
#endif
{
    static unsigned int start = 0;
    static unsigned int stop = 40;

    if (buzzer_on)
    {
        if (start < 2 * stop)
        {
            make_sound();
            led_on();
            start++;
        }
        else if (start < 3 * stop)
        {
            led_off();
            start++;
        }
        else if (start < 5 * stop)
        {
            make_sound();
            led_on();
            start++;
        }
        else if (start < 10 * stop)
        {
            led_off();
            start++;
        }
        else
            start = 0;
    }
}
