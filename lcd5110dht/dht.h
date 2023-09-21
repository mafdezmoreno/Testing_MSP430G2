/*
 * Based on https://github.com/bafeigum/DHT11-Library-for-MSP430/tree/master from Bryce Feigum
 * Changes:
 * * Improved with encapsulations.
 * * Replaced callbacks to timer 0 with timer 1. Included functions and methods to use both.
 */

#ifndef LCD5110DHT_DHT_H
#define LCD5110DHT_DHT_H

#include "msp430g2553.h"
#include "timer.h"

#define DHT_PIN BIT3
#define BIT_DHT_VCC BIT2
#define DISPLAY_LED_BIT BIT5

#define TST(x,y)	(x & (y))
#define SET(x,y)	(x|= (y))
#define CLR(x,y)	(x &= ~(y))

#define DHT_TIMER1

class dht
{
public:
    dht();
    ~dht();
    bool readDht();
    const char * getHumidity();
    const char * getTemperature();

private:
    unsigned char tempDigits[3];
    unsigned char humiDigits[2];

#ifdef DHT_TIMER1
    timer1 *pT;
#else
    timer0 *pT;
#endif

    void readData(unsigned char * data);
    void startSignal();
    bool checkResponse();
    bool checkChecksum(unsigned char * tmp);
    bool checkDigits(const unsigned char * digits);
    void saveDigits(const unsigned char * digits);
    unsigned char * getDigits(unsigned char * tmpData);
    bool analyzeResponse(unsigned char * tmpData);
};

#endif /* LCD5110DHT_DHT_H */
