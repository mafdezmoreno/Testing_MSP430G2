/*References:
 * 5110 LCD:
 *      https://forum.43oh.com/topic/1312-nokia-5110-display
 *      https://gist.github.com/DmitryMyadzelets/83b6e1384a1ee35a7f5f
 * DHT:
 *      https://github.com/bafeigum/DHT11-Library-for-MSP430
 */


//**********************************************//
// MSP430G2553 + PCD8544LCD(Nokia 5110) + DHT22 //
//**********************************************//

/*
 * TODO:
 *      Use a output pin to fed dht only the required time to make it works (once every minute).
 *      Configure a pin to read the level (voltage) of the battery.
 *      Print the level of the battery in the top-right corner of the lcd.
 *      Configure a second timer to refresh the display every 2-3 seconds.
 */

#include <stdbool.h>
#include <msp430g2553.h>
#include "PCD8544.h"
#include "DHT11_LIB.h"
#include "lcd_5110.h"
#include "dht_to_lcd.h"

unsigned char volatile debug_var_timer_1=0;
unsigned char volatile debug_var_timer_0=0;
bool volatile timer_1_loop = true; // To realize a print just after starting
bool volatile timer_0_dht = true;  // To get a DHT read just after starting

void init();
void print_debug();
void init_timers();

//Time functions
void print_hora();
void print_fecha();
void print_alarma();

//Control functions
void print_level_battery();

void main(void) {

    init();
    init_timers(); //to control principal loop and dht
    clearLCD();

    while(1) {

        if (timer_1_loop){
            print_debug();
            print_fecha();
            print_hora();
            print_level_battery();
            timer_1_loop = false;
            print_alarm();
        }
        if (timer_0_dht){
            dht_to_lcd_loop(); //DHT request data and print
            timer_0_dht = false;
        }

    }
}



void init(){
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    BCSCTL1 = CALBC1_1MHZ;      // Set oscillator to 1MHz
    DCOCTL = CALDCO_1MHZ;       // Set oscillator to 1MHz

    initLCD();
    clearLCD();
    setAddr(0, 0);
    writeStringToLCD("LCD Iniciada");
    init_dht();
    setAddr(0, 1);
    writeStringToLCD("DHT Iniciado");
    __delay_cycles(4000000);

}


void print_debug(){

    //static unsigned int i = 0;

    //print debug_var_timer_1
    setAddr(0, 4);
    writeStringToLCD("d-d");
    setAddr(25, 4);
    writeCharToLCD(debug_var_timer_0+0x30);

    //print i
    //setAddr(30, 0);
    //writeStringToLCD("i_vr");
    //setAddr(30, 1);
    //writeCharToLCD(i+0x30);

    //print debug_var_timer_0
    setAddr(45, 4);
    writeStringToLCD("d-l");
    setAddr(70, 4);
    writeCharToLCD(debug_var_timer_1+0x30);

    //i++;
    //if (i>=10){
    //    i = 0;
    //}
}


void init_timers(){

    clearLCD();
    setAddr(0, 0);
    writeStringToLCD("Init Timer");
    __delay_cycles(100000);

    //Timer 0 configuration to use with dht
    //The interruption is called every 0.2 seg
    TA0CCR0   = 50000;           // Initialize the timer to count at 5Hz
    TA0CCTL0 |= CCIE;            // Enable interrupt
    TA0CTL  |= TASSEL_2 + ID_2 + MC_1 + TAIFG + TACLR;    // SMCLK, div 4, up mode,
                                 //UP          // clear timer


    //Timer 1 configuration (loop) // This timer in used to control the principal code execution
    //The interruption is called every 0.5seg

    TA1CCR0  = 62500;  //Initialize the timer
    TA1CCTL0 |= CCIE;  //Enable interrupt
    TA1CTL |= TASSEL_2 + ID_3 +  MC_1 + TAIFG + TACLR;
             //SMCLK     //DIV 8 //UP           // clear timer


    _enable_interrupt();                        //Enable global interrupt

    setAddr(0, 1);
    writeStringToLCD("Timer Iniciado");
    __delay_cycles(2000000);

}

//this timer is used by the dht
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer__dht(void){


    static unsigned int sub_divider_timer_0 = 0;

    if (sub_divider_timer_0 >= 600){ ////600 = after 10min
        timer_0_dht = true;

        sub_divider_timer_0 = 0;
        TOUT=1; //Requiered by dht lib timer

        debug_var_timer_0++;
        if (debug_var_timer_0 >= 10){
            debug_var_timer_0 =0;
        }

    }
    sub_divider_timer_0++;


}


// The timer in used to control the principal code execution
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer__loop(void){

    static unsigned int sub_divider_timer_1 = 0;

    if (sub_divider_timer_1 >= 100){ //120 = after 1 min
        timer_1_loop = true;
        sub_divider_timer_1 = 0;

        debug_var_timer_1++;
        if (debug_var_timer_1 >= 10){
            debug_var_timer_1 =0;
        }
    }
    sub_divider_timer_1++;
}


void print_hora(){
    setAddr(25, 1);
    writeStringToLCD("19:28");
}
void print_fecha(){
    setAddr(10, 2);
    writeStringToLCD("22/05/2021");

    setAddr(0, 0);
    writeStringToLCD("W:");

    setAddr(10, 0);
    writeCharToLCD(0+0x30);

    setAddr(30, 0);
    writeStringToLCD("Sat");
}

void print_alarm(){

    setAddr(0, 3);
        writeStringToLCD("Alarm1 On 7:00");

}

void print_level_battery(){

    setAddr(60, 0);
    writeStringToLCD("B:");
    setAddr(75, 0);
    writeCharToLCD(0+0x30);

}
