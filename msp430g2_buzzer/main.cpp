/**
 * Program to make the appropriate sound, with timer interruptions, to 
 * feed a buzzer to be integrated in an alarm clock.
 * 
 */

//#include <msp430.h> 
#include "msp430g2452.h"
#include "buzzer.h"


bool buzzer_on = true;

int main(void)
{
	
	init_clocks();
	initTimer_A();
	init_buzzer(2, BIT2);  // (port, pin) PIN2.2
	_bis_SR_register(GIE); //Enable interrupts

	
	return 0;
}

//Timer ISR
//#pragma vector = TIMER0_A0_VECTOR		//
//__interrupt void Timer_A0(void)		//for TI compiler
__attribute__ ( ( interrupt( TIMER0_A0_VECTOR ) ) )		//
void TIMER0_A0_ISR( void )								//for GNU compiler
{
	static unsigned int start = 0; 
	static unsigned int stop = 200; //200 interruptions pause
		//example: 
			// 600Hz interruption = 1,7 msec every interruption
			// 200 interuptions at 600Hz = 0.33 sec of pause
	if (buzzer_on){
    	if (start < stop){
			make_sound(2, BIT2); // (port, pin)
			start++;
		}
		else if (start < 1.5*stop){
			start ++;
		}
		else
			start = 0;
	}
}