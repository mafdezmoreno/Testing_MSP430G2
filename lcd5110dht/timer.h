
#ifndef LCD5110DHT_TIMER_H
#define LCD5110DHT_TIMER_H

#define TST(x,y)	(x & (y))
#define SET(x,y)	(x|= (y))
#define CLR(x,y)	(x &= ~(y))

#define LOW_POWER_MODE // Reduces consumption ~30mA
                       // Only for timer used in delays

#include "msp430g2553.h"

class timer0
{
public:
    void usWait(const unsigned * usDelay);
    void msWait(const unsigned * msDelay);
    void usInitTimer(const unsigned * us);
    void msInitTimer(const unsigned * ms);
    bool timeOut();
    void stopTimer();
};

class timer1
{
public:
    void usWait(const unsigned * usDelay);
    void msWait(const unsigned * msDelay);
    void usInitTimer(const unsigned * us);
    void msInitTimer(const unsigned * ms);
    bool timeOut();
    void stopTimer();
};

#endif /* LCD5110DHT_TIMER_H */
