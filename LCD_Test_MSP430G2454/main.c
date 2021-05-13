/*
 * Sources:   https://github.com/suryasiddhart/hd44780-lcd
 *            https://circuitdigest.com/microcontroller-projects/16x2-lcd-display-interfacing-with-msp430-using-code-composer-studio
 * mafdezmoreno (13/05/21):
 *      Minimal changes to make it work with msp430g2452:
 *      #include "msp430g2553.h" replaced by #include "msp430g2452.h (in header os source code and in the main.c file)
 */

#include "msp430g2452.h"
#include "hd44780.h"

uint8_t value=0;
void main( void )
{
    WDTCTL = (WDTPW | WDTHOLD);             // Stop watchdog timer
    BCSCTL1 = CALBC1_1MHZ;                  // Set range to calibrated 1MHz
    DCOCTL  = CALDCO_1MHZ;                  // Set DCO step and modulation for 1MHz
    P1DIR = 0xFF;                           // Set P1.0 (D0) to P1.7 (D7)
    P2DIR = (0x01 | 0x02);                  // Set P2.0 (E) and P2.1 (RS) to output
    TA0CCR1  = 32768;                // Set CCR1 value for 32.678 ms interrupt
    TA0CCTL1 = CCIE;                        // Compare interrupt enable
    TA0CTL   = (TASSEL_2 | MC_2 | TACLR);   //SMCLK, Continuous mode
    __bis_SR_register( GIE );               // Enable global interrupts
    hd44780_clear_screen();                 // Clear display content
    while( 1 )                              // Endless loop - main program
    {
        hd44780_write_string( "Probando_____LCD", 1, 1, NO_CR_LF );
        hd44780_output_unsigned_16bit_value( value++, 2, 2, 1, 0 );
        if(value>9)
            value=0;
    }
}

// Directive for timer interrupt
#pragma vector = TIMER0_A1_VECTOR
__interrupt void timer_0_a1_isr( void )     // Timer 0 A1 interrupt service
{
  switch( TA0IV )  // Determine interrupt source
  {
    case 2:      // CCR1 caused the interrupt
    {
      hd44780_timer_isr();  // Call HD44780 state machine
      break;                // CCR1 interrupt handling done
    }
  }
}

