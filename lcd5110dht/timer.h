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
extern bool volatile timeOut0;
extern bool volatile timeOut1;

void usWait(const unsigned * usDelay);
void msWait(const unsigned * msDelay);
void usInitTimer1(const unsigned * us);
void msInitTimer1(const unsigned * ms);
void usInitTimer0(const unsigned * us);
void secInitTimer0(const unsigned * sec);

void upModeTimer0();
void upModeTimer1();
void stopTimer0();
void stopTimer1();

#endif /* TIMER_H_ */
