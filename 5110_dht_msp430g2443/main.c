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

#include <msp430g2553.h>
#include "PCD8544.h"
#include "DHT11_LIB.h"
#include "lcd_5110.h"
#include "dht_to_lcd.h"

void init();
void print_debug();
void init_loop_timer();

void main(void) {

    init();
    while(1) {

        print_debug();
        dht_to_lcd_loop(); //DHT request data and print
        __delay_cycles(2000000);

    } // eof while()
}



void init(){
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
    initLCD();
    init_dht();
}

void print_debug(){

    static unsigned int i = 0;
    //clearLCD();
    setAddr(0, 0);
    writeStringToLCD("Testing");
    setAddr(1, 1);
    writeCharToLCD(i+0x30);
    i++;
    if (i>=10){
        i = 0;
    }
}

void init_loop_timer(){

}


