//#include <msp430.h>
#include "msp430g2553.h"
#include "time_clock.h"
#include "lcd5110.h"
#include "timerACLK.h"


/**
 * main.c
 */

void clean_lcd();
void time_to_lcd();


Time_Clock my_time;

int main(void)
{
    init_clocks();
    initTimer0();
    up_timer0();
    _bis_SR_register(GIE); //Enable interrupts

    initLCD();
    clearLCD();
    clean_lcd();
    for(unsigned long i = 2000; i>0; i--);     // delay

    while(1){

        time_to_lcd();
        for(unsigned long i = 200; i>0; i--);     // delay
    }
    return 0;
}

//Timer ISR
#pragma vector = TIMER0_A0_VECTOR		//
__interrupt void Timer_A0(void)		//for TI compiler
{
    my_time.increment_second();
}

void clean_lcd(){
    setAddr(46, 1);
    writeStringToLCD(my_time.get_second(), 2);
    setAddr(28, 1);
    writeStringToLCD(my_time.get_minute(),2 );
    writeCharToLCD(0x3a); //":"
    setAddr(10, 1);
    writeStringToLCD(my_time.get_hour(), 2);
    writeCharToLCD(0x3a); //":"
}

void time_to_lcd(){

    setAddr(46, 1);
    writeStringToLCD(my_time.get_second(),2);
     
    if(my_time.get_update_minute()){
        setAddr(28, 1);
        writeStringToLCD(my_time.get_minute(),2);

        if(my_time.get_update_hour()){
            setAddr(10, 1);
            writeStringToLCD(my_time.get_hour(),2);

            if(my_time.get_update_day()){
                setAddr(10, 2);
                writeStringToLCD(my_time.get_nday(),2);
                writeCharToLCD(0x2f); //"/"
                setAddr(10, 3);
                writeCharToLCD(0x57); //"W"
                writeStringToLCD(my_time.get_week(),2);
                
                
                if(my_time.get_update_week()){
                    setAddr(35, 3);
                    writeStringToLCD(my_time.get_wday(),9);
                }
                if(my_time.get_update_month_year()){
 
                    setAddr(28, 2);
                    writeStringToLCD(my_time.get_month(),2);
                    writeCharToLCD(0x2f); //"/"
                    writeStringToLCD(my_time.get_year(),2);

                }
            }
        }
    }
}
