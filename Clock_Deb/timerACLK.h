/*
 * timerACLK.h
 *
 *  Created on: May 31, 2021
 *      Author: mafm
 */
#include "msp430g2553.h"

#ifndef TIMERACLK_H_
#define TIMERACLK_H_

void init_clocks(); ////MCLK and SMCLK 1Mhz/8 = 125 kHz
void initTimer0();
void stop_timer0();
void up_timer0();

#endif /* TIMERACLK_H_ */
