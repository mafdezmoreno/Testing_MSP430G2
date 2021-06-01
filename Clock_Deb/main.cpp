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
    initTimer0();
    up_timer0();
    _bis_SR_register(GIE); //Enable interrupts


    initLCD();

    clearLCD();
    setAddr(10, 0);
    writeStringToLCD("00:00:00");
    setAddr(10, 1);
    writeStringToLCD("00:00:00");
    for(unsigned long i = 200000; i>0; i--);     // delay

    while(1){

        setAddr(46, 1);
        writeStringToLCD(my_time.get_second());
        
        if(my_time.get_update_minute()){
            setAddr(28, 1);
            writeStringToLCD(my_time.get_minute());

            if(my_time.get_update_hour()){
                setAddr(10, 1);
                writeStringToLCD(my_time.get_hour());
            }
        }
        
        for(unsigned long i = 20000; i>0; i--);     // delay
    }
    return 0;
}


//Timer ISR
#pragma vector = TIMER0_A0_VECTOR		//
__interrupt void Timer_A0(void)		//for TI compiler
{
    my_time.increment_second();
}
