/**
 * Program to make the appropriate sound, with timer interruptions, to 
 * feed a buzzer to be integrated in an alarm clock.
 * 
 * A push button activates/desactivates the alarm
 * 
 */

//#include <msp430.h> 

#define BUTTON_PORT	2
#define BUTTON_PIN	BIT1
#define BUZZER_PORT 2
#define BUZZER_PIN BIT2

#include "msp430g2452.h"
#include "buzzer.h"
#include "button.h"


bool buzzer_on = false;

int main(void)
{
	init_clocks();
	initTimer_A();
	init_buzzer();  // (port, pin) PIN2.2
	_bis_SR_register(GIE); //Enable interrupts

	init_button(BUTTON_PORT, BUTTON_PIN);  // (port, pin) PIN2.2

	while(true){
		static bool checked = false;
		static unsigned int delay_check = 10000;
		if (!checked){
			if(check_state(BUTTON_PORT, BUTTON_PIN)){
				//stop_timer_A();
				if(!buzzer_on){
					buzzer_on = true;
					up_timer_A();
				}
				else if (buzzer_on){
					buzzer_on = false;
					stop_timer_A();
				}

				checked = true;
			}
			
		}
		else{
			delay_check--;
			if (delay_check == 0){ 
				delay_check = 10000;
				checked = false;
			}

		}
		//for(unsigned long int i=100; i>0; i--);     // delay
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
	//if (buzzer_on){
    	if (start < stop){
			make_sound(); // (port, pin)
			start++;
		}
		else if (start < 1.7*stop){
			start ++;
		}
		else
			start = 0;
	//}
}