/*
 * button.h
 *
 *  Created on: May 28, 2021
 *      Author: mafm
 */

#include "msp430g2452.h"

#ifndef BUTTON_H_
#define BUTTON_H_

void init_button(unsigned char PORT, unsigned char PIN){

    switch(PORT) {
       case 1 :
          P1DIR &=~ PIN; //set pin as input (by default it's setted as input)
          P1REN |=  PIN; //resistor enable
          P1OUT |=  PIN; //pull up resistor
          break;

       case 2  :
          P2DIR &=~ PIN;
          P2REN |=  PIN;
          P2OUT |=  PIN;
          break;
    }
}

bool check_state(unsigned char PORT, unsigned char PIN){
    //clear_buzzer_pin();
    //stop_timer_A();
    //for(unsigned int i=100; i>0; i--);
    switch(PORT) {
       case 1 :
            if ((P1IN & PIN) != PIN){
                for(unsigned int i=300; i>0; i--);     // delay
                if ((P1IN & PIN) != PIN){
                    //init_buzzer();
                    //for(unsigned int i=20; i>0; i--); 
                    return true;
                }
            }

       case 2  :
            if ((P2IN & PIN) != PIN){
                for(unsigned int i=300; i>0; i--);     // delay
                if ((P2IN & PIN) != PIN){
                    //init_buzzer();
                    //for(unsigned int i=100; i>0; i--); 
                    return true;
                }
            }
    }
    return false;
}
#endif /* BUTTON_H_ */
