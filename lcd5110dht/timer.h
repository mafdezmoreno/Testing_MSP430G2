/*
 * timer.h
 *
 *  Created on: Sep 17, 2023
 *      Author: mafm
 */

#ifndef TIMER_H_
#define TIMER_H_

#define TST(x,y)	(x & (y))
#define SET(x,y)	(x|= (y))
#define CLR(x,y)	(x &= ~(y))

#include "msp430g2553.h"

void usWait(const unsigned * usDelay);
void msWait(const unsigned * msDelay);
void usInitTimer(const unsigned * us);
void msInitTimer(const unsigned * ms);
bool timeOut();
void stopTimer();

#endif /* TIMER_H_ */
