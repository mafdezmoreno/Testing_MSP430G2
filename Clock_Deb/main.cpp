#include "msp430g2553.h"
#include "time_clock.h"
#include "lcd5110.h"
#include "timerACLK.h"


void clean_lcd();
void time_to_lcd();
void change_time();
void init_buttons();
void init_button_change_time();
void init_button_move_pos();
void init_button_increment_value();
void init_button_decrement_value();
bool check_state_button_move_pos_r();
bool check_state_button_move_pos_l();
bool check_state_button_time();
bool check_state_button_increment_value();
bool check_state_button_decrement_value();
void toggle_hour();
void toggle_nday();
void toggle_minute();

bool move_pos();

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
    init_buttons();
    for(unsigned long i = 2000; i>0; i--);     // delay

    while(1){

        time_to_lcd();
        change_time();
        for(unsigned long i = 200; i>0; i--);     // delay
    }
    return 0;
}

//Timer ISR
#pragma vector = TIMER0_A0_VECTOR	//
__interrupt void Timer_A0(void)		//for TI compiler
{
    my_time.increment_second();
}

void change_time(){
    
    static unsigned int position = 3;
    while(check_state_button_time()){

        switch(position) {
            case 1 :
                toggle_hour();
                break;
            case 2 :
                toggle_minute();
                break;
            case 3 :
                toggle_nday();
                break;
        }

        if (move_pos()){
            position++;
            clean_lcd();

            if (position >3)
                position = 1;
            else if (position <1)
                position = 3; 
        }

        clean_lcd();
        for(unsigned long i=5000; i>0; i--);     // delay
    }
}

void toggle_hour(){
    
    if (check_state_button_increment_value())
        my_time.increment_hour();
    else if (check_state_button_decrement_value())
        my_time.decrement_hour();

    //static bool toggle = true;
    setAddr(10, 1);
    //if (toggle){
        writeStringToLCD(my_time.get_hour(), 2);
    //    toggle = false;
    //}
    for(unsigned long i=80000; i>0; i--);     // delay
    //else {
    setAddr(10, 1);
        writeCharToLCD(0x5b); //"_"
        writeCharToLCD(0x5b); //"_"
    //    toggle = true;
    //}

    for(unsigned long i=20000; i>0; i--);     // delay
}

void toggle_minute(){
    
    static bool toggle = true;
    setAddr(28, 1);
    if (toggle){
        writeStringToLCD(my_time.get_minute(), 2);
        toggle = false;
    }
    else {
        writeCharToLCD(0x5b); //"_"
        writeCharToLCD(0x5b); //"_"
        toggle = true;
    }
    for(unsigned long i=50000; i>0; i--);     // delay
}

void toggle_nday(){
    
    static bool toggle = true;
    setAddr(10, 2);
    if (toggle){
        writeStringToLCD(my_time.get_nday(), 2);
        toggle = false;
    }
    else {
        writeCharToLCD(0x5b); //"_"
        writeCharToLCD(0x5b); //"_"
        toggle = true;
    }
    for(unsigned long i=50000; i>0; i--);     // delay
}

void init_buttons(){
    init_button_change_time();
    init_button_move_pos();
    init_button_increment_value();
    init_button_decrement_value();

}

void init_button_decrement_value(){
    //1.2
    P1DIR &=~ BIT2; //set pin as input (by default it's setted as input)
    P1REN |=  BIT2; //resistor enable
    P1OUT |=  BIT2; //pull up resistor
}

void init_button_increment_value(){
    //1.3
    P1DIR &=~ BIT3; //set pin as input (by default it's setted as input)
    P1REN |=  BIT3; //resistor enable
    P1OUT |=  BIT3; //pull up resistor
}

void init_button_change_time(){
    //1.0
    P1DIR &=~ BIT0; //set pin as input (by default it's setted as input)
    P1REN |=  BIT0; //resistor enable
    P1OUT |=  BIT0; //pull up resistor
}

void init_button_move_pos(){
    //2.0
    P2DIR &=~ BIT0; //set pin as input (by default it's setted as input)
    P2REN |=  BIT0; //resistor enable
    P2OUT |=  BIT0; //pull up resistor
}


bool check_state_button_increment_value(){
    //1.3
    if ((P1IN & BIT3) != BIT3){
        for(unsigned int i=50; i>0; i--);     // delay
            if ((P1IN & BIT3) != BIT3){
            return true;
            }
    }
    return false;
}

bool check_state_button_decrement_value(){
    //1.2
    if ((P1IN & BIT2) != BIT2){
        for(unsigned int i=50; i>0; i--);     // delay
            if ((P1IN & BIT2) != BIT2){
            return true;
            }
    }
    return false;
}


bool check_state_button_move_pos(){
    //2.0
    if ((P2IN & BIT0) != BIT0){
        for(unsigned int i=50; i>0; i--);     // delay
            if ((P2IN & BIT0) != BIT0){
            return true;
            }
    }
    return false;
}

bool check_state_button_time(){
    //1.0
    if ((P1IN & BIT0) != BIT0){
        for(unsigned int i=50; i>0; i--);     // delay
            if ((P1IN & BIT0) != BIT0){
            return true;
            }
    }
    return false;
}

bool move_pos(){
    if (check_state_button_move_pos())
        return true;
    return false;
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

    setAddr(10, 2);
    writeStringToLCD(my_time.get_nday(), 2);

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
