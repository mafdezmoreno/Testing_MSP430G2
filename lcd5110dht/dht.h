/*
 * Based on https://github.com/bafeigum/DHT11-Library-for-MSP430/tree/master from Bryce Feigum
 * Changes:
 * * Improved with encapsulations.
 * * Replaced callbacks to timer 0 with timer 1. Included functions and methods to use both.
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

class dht
{
public:
    dht();
    bool readDht();
    const char * getHumidity();
    const char * getTemperature();

private:
    unsigned char checksum;
    unsigned char tempDigits[3];
    unsigned char humiDigits[2];

    void readData(unsigned char * data);
    void startSignal();
    bool checkResponse();
    bool checkChecksum(unsigned char * tmp);
};

#endif /* DHT_H_ */
