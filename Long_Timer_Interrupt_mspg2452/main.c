/* References:
 *      http://saroselectronics.com/msp430-timer-interrupts/
 *      http://www.crash-bang.com/getting-started-msp430-timers-3/
 */

#include "msp430g2452.h"

#define PIN_LED BIT3    //BIT1 default msp430g2 launchpad
#define PORT_LED 2      //1 default msp430g2 launchpad

void toggle_led();
void set_output_dir();
void initTimer_A(void);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; //Stop watchdog timer

    //MCLK=SMCLK=1Mhz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    initTimer_A();

    set_output_dir();

    //while(1){
        _bis_SR_register(GIE); //Enable interrupts
        //_bis_SR_register(LPM3_bits); //Enter Low Power Mode 3. It's need external clock
    //}

    /*
     * Energy consumption:
     *      8,5mW (330ohm, yellow led, 3,3volt)
     */

}

void set_output_dir(){
    switch(PORT_LED) {
       case 1 :
          P1DIR |=  PIN_LED;
          P1OUT &= ~PIN_LED;
          break;

       case 2  :
          P2DIR |=  PIN_LED;
          P2OUT &= ~PIN_LED;
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
    _bic_SR_register_on_exit(LPM3_bits);
}
