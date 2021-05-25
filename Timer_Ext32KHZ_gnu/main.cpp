/* References
 * http://www.msp430launchpad.com/2012/03/using-aclk-and-32khz-crystal.html
 *
 */


/* WARNINGS
 *
 * The circuit must be powered at 5Volt to make the external crystal works properly
 *
 * It's takes a few seconds to stabilize the frequency. At the beginning, just after
 * powering the circuit, it could seem that it does not work, but after a few seconds
 * the LED lights up regularly according to the configured timer.
 */


#include "msp430g2452.h"
#include "hal_io.h"

void init_external_clock();
void init_internal_clock_1MHz();
void init_timer_aclk_div_8();
void init_ports();

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
    init_ports();

	init_external_clock();
	//init_internal_clock_1MHz();
	init_timer_aclk_div_8();


    //_BIS_SR(LPM3_bits + GIE);           // Enter LPM3 (low power mode 3) w/ interrupt
    _BIS_SR(GIE); //

    while(1){

	}

	return 0;

}

// Timer A0 interrupt service routine
/* For TI Compiler:
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
*/
///*For GNU Compiler
void __attribute__((interrupt(TIMER0_A0_VECTOR))) TimerA_ISR(void)
//*/
{
    toggle_pin(BIT2, BIT2);
}

void init_ports(){

     set_output(BIT2, BIT2);
     set_low_pin(BIT2, BIT2);
}

void init_external_clock(){

    // Setup:
        // External crystal: 32768Hz
        // ACLK/8: 4096 Hz

    BCSCTL1 |= DIVA_3;              // ACLK/8
    BCSCTL3 |= XT2S_0 + XCAP_3;
             // XT2S_0: Range for crystal
             // XCAP_3: 12pF

}

void init_timer_aclk_div_8(){

    CCTL0 = CCIE;                    // CCR0 interrupt enabled

    TACTL = TASSEL_1 + MC_1 +ID_3+ TAIFG + TACLR;;  // ACLK, /8, upmode
    //ID_3: div_8 = 512 //hz

    CCR0 = 63;  // div 64 = 0.125 seg
    //CCR0 = 511; // div 512= 1 seg

}

void init_internal_clock_1MHz(){
    //MCLK=SMCLK=1Mhz
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
}

