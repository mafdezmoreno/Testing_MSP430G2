/*
 * Based on https://github.com/bafeigum/DHT11-Library-for-MSP430/tree/master from Bryce Feigum
 * Changes:
 * * Improved with encapsulations.
 * * Replaced callbacks to timer 0 wiht timer 1. Included functions and methods to use both.
 */

#ifndef DHT_H_
#define DHT_H_

#include "msp430g2553.h"

#define DHT_PIN BIT3

#define BIT_DHT_VCC BIT2

#define DISPLAY_LED_BIT BIT5

#define TST(x,y)	(x & (y))
#define SET(x,y)	(x|= (y))
#define CLR(x,y)	(x &= ~(y))
#define TOG(x,y)	(x ^= (y))

class dht
{
public:
    dht();
    bool readDht();
    const char * getHumidity();
    const char * getTemperature();

private:
    unsigned char checksum;
    unsigned char data[5];

    unsigned char readData(unsigned char * data);
    void startSignal();
    unsigned char checkResponse();
    unsigned char readByte();
    bool checkChecksum(unsigned char *);
};

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

#endif /* DHT_H_ */
