/**
 * Program to make the appropriate sound, with timer interruptions, to 
 * feed a buzzer to be integrated in an alarm clock.
 * 
 * A push button activates/desactivates the alarm
 * 
 */

//#include <msp430.h> 
#include "msp430g2452.h"
#include "buzzer.h"
#include "button.h"


bool buzzer_on = false;

int main(void)
{
	init_clocks();
	initTimer_A();
	init_buzzer(2, BIT2);  // (port, pin) PIN2.2
	_bis_SR_register(GIE); //Enable interrupts

	init_button(2, BIT1);  // (port, pin) PIN2.2

	while(true){
		
		if(check_state(2, BIT1)){
			if(!buzzer_on)
				buzzer_on = true;
			else if (buzzer_on)
				buzzer_on = false;
			for(unsigned long int i=500; i>0; i--);     // delay
		}
	}
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