/*
 * buzzer.h
 *
 *  Created on: May 27, 2021
 *      Author: mafm
 */

#include "msp430g2452.h"

#ifndef BUZZER_H_
#define BUZZER_H_

void init_clocks(){

    WDTCTL = WDTPW + WDTHOLD; //Stop watchdog timer

    //MCLK=SMCLK=1Mhz
        //DCOCTL, DCO Control Register
        DCOCTL = 0;            //clears DCOCTL to set the DCOCLK to the lowest setting.
        DCOCTL = CALDCO_1MHZ;  //Copy the calibration data


        //BCSCTL1, Basic Clock System Control Register 1
        BCSCTL1 = CALBC1_1MHZ; //Copy the calibration data


    //BCSCTL2, Basic Clock System Control Register 2
        //Allow set divider for clocks
        BCSCTL2 |= DIVM_3 + DIVS_3; //MCLK snd SMCLK /8

        // Hearing range: 
            // 20 Hz – 20.000 Hz
        
        //1Mhz/8 = 125 kHz

}

void initTimer_A(void)
{
    // TACCTL0, Capture/Compare Control Register
    //TACCTL0 |= CCIE; //Enable Interrupts on Timer

    //TACCR0, Timer_A Capture/Compare Register 0
    //TACCR0 = 210; //Number of cycles in the timer
            //125 / 25 = 5 kHz      //high-pitched sound
            //125 / 125 = 1 kHz  
            //125 / 250 = 0.5 kHz   
            //125 / 500 = 0.25 kHz  //bass sound

    // Timer_A Control Register
    TACTL    |= TASSEL_2 + ID_1; //+ MC_1;
                                //MC_1; //Use UP mode timer
                          //ID_1; //SMCLK/1
                //TASSEL_2; //Use SMCLK as source for timer
}

void stop_timer_A(){
    // Timer_A Control Register
    TACTL    |= MC_0; // Stop mode
    TACCTL0  &=~ CCIE; // Disable Interrupts on Timer
}

void up_timer_A(){
    TACCR0    = 210;
    TACCTL0  |= CCIE;
    TACTL    |= MC_1;
}

void init_buzzer(){

    switch(BUZZER_PORT) {
       case 1 :
          P1DIR |= BUZZER_PIN; //output
          P1OUT &=~ BUZZER_PIN; //LOW state
          break;

       case 2  :
          P2DIR |= BUZZER_PIN;
          P2OUT &=~ BUZZER_PIN;
          break;
    }
}

void clear_buzzer_pin(){
    switch(BUZZER_PORT) {
       case 1 :
          P1DIR &=~ BUZZER_PIN;
          break;

       case 2  :
          P2DIR &=~ BUZZER_PIN;
          break;
    }
}

void make_sound(){

    switch(BUZZER_PORT) {
           case 1 :
              P1OUT ^= BUZZER_PIN;
              break;

           case 2  :
              P2OUT ^= BUZZER_PIN;
              break;
        }
}

#endif /* BUZZER_H_ */
