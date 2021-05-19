/* References:
 *      http://saroselectronics.com/msp430-timer-interrupts/
 *      http://www.crash-bang.com/getting-started-msp430-timers-3/
 */

#include "msp430g2452.h"

#define PIN_LED BIT2    //BIT1 default msp430g2 launchpad
#define PORT_LED 2      //1 default msp430g2 launchpad

void toggle_led();
void set_output_dir();
void initTimer_A(void);

void main(void)
{
    //Turning off pins by default (to save energy)
    P1OUT = 0;
    P2OUT = 0;
    //enable pullup by default (to save energy)
    P1REN |= 0b11111111;
    P2REN |= 0b11111011; //except PIN_LED


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


    initTimer_A();

    set_output_dir();

    //while(1){
        _bis_SR_register(GIE); //Enable interrupts
        _bis_SR_register(LPM1_bits); //Enter Low Power Mode
                                     //LPM2 an higher will need ACLK an external crystal
    //}

    /*
     * Energy consumption:
     *      8,5mW (330ohm, yellow led, 3,3volt,ID_3)
     *      7,5mW (330ohm, yellow led, 3,3volt,ID_3, LPM0, DIVM_3 + DIVS_3)
     *      7,5mW (330ohm, yellow led, 3,3volt,ID_3, LPM1, DIVM_3 + DIVS_3)
     *      6,9mW (330ohm, yellow led, 3,3volt,ID_3, LPM1, DIVM_3 + DIVS_3, turning off unused pins + pullups)
     */

}

void set_output_dir(){
    switch(PORT_LED) {
       case 1 :
          P1DIR |=  PIN_LED;
          P1OUT |=  PIN_LED;
          break;

       case 2  :
          P2DIR |=  PIN_LED;
          P2OUT |=  PIN_LED;
          break;
    }
}

void toggle_led(){

    switch(PORT_LED) {
           case 1 :
              P1OUT ^= PIN_LED;
              break;

           case 2  :
              P2OUT ^= PIN_LED;
              break;
        }
}

void initTimer_A(void)
{
    // TACCTL0, Capture/Compare Control Register
    TACCTL0 |= CCIE; //Enable Interrupts on Timer

    //TACCR0, Timer_A Capture/Compare Register 0
    TACCR0 = 30000; //Number of cycles in the timer

    // Timer_A Control Register
    TACTL    |= TASSEL_2 + ID_3 + MC_1;
                                //MC_1; //Use UP mode timer
                          //ID_3; //SMCLK/8
                //TASSEL_2; //Use ACLK as source for timer
}


//Timer ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    toggle_led();
}
