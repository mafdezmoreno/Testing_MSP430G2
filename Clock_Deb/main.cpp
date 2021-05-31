//#include <msp430.h>
#include "msp430g2553.h"
#include "time_clock.h"
#include "lcd5110.h"
#include "timerACLK.h"


/**
 * main.c
 */

Time_Clock my_time;

int main(void)
{
    init_clocks();
    //initTimer0();
    //up_timer0();

    initLCD();

    clearLCD();

    while(1){
        setAddr(10, 1);
        writeStringToLCD(my_time.get_second());
        for(uint8_t i = 2000000; i>0; i--);     // delay
        setAddr(10, 2);
        writeStringToLCD("A");
        for(uint8_t i = 2000000; i>0; i--);     // delay
        
    }
    return 0;
}


//Timer ISR
#pragma vector = TIMER0_A0_VECTOR		//
__interrupt void Timer_A0(void)		//for TI compiler
{
    my_time.increment_second();
}
