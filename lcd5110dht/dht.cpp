/*
 * Based on https://github.com/bafeigum/DHT11-Library-for-MSP430/tree/master from Bryce Feigum
 * Changes:
 * * Improved with encapsulations.
 * * Moved timer management into a class
 */

#include "dht.h"
#include "timer.h"

dht::dht()
{
    SET (P2OUT, BIT_DHT_VCC);       // VCC to dht (pin output)
    humiDigits[0] = '0';
    humiDigits[1] = '0';
    tempDigits[0] = '0';
    tempDigits[1] = '0';
    tempDigits[2] = '0';

#ifdef DHT_TIMER1
    pT = new timer1;
#else
    pT = new timer0;
#endif
}

dht::~dht()
{
    delete pT;
}

bool dht::readDht()
{
    static unsigned counter = 0;
    unsigned char tmpData[5];

    counter++;
    switch (counter)
    {
        case 1:
            powerOn();
            break;
        case 3:
            readData(tmpData);
            powerOff();
            return analyzeResponse(tmpData);
        case 10:
            counter = 0;
    }
    return false;
}

bool dht::analyzeResponse(unsigned char * tmpData)
{
    if (checkChecksum(tmpData))
    {
        unsigned char * digits;
        digits = getDigits(tmpData);
        if (!checkDigits(digits))
        {
            delete digits;
            return false;
        }
        saveDigits(digits);
        delete digits;
        return true;
    }
    return false;
}

unsigned char * dht::getDigits(unsigned char * tmpData)
{
    unsigned char * digits;
    digits = new unsigned char [5];

    digits[0] = tmpData[0] / 10 + '0';
    digits[1] = tmpData[0] % 10 + '0';
    digits[2] = tmpData[2] / 10 + '0';
    digits[3] = tmpData[2] % 10 + '0';
    digits[4] = tmpData[3] % 10 + '0';

    return digits;
}

bool dht::checkDigits(const unsigned char * digits)
{
    for (int i=0; i<5; i++)
    {
        if ( digits[0] >= '0' && digits[0] <= '9')
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}

void dht::saveDigits(const unsigned char * digits)
{
    humiDigits[0] = digits[0];
    humiDigits[1] = digits[1];
    tempDigits[0] = digits[2];
    tempDigits[1] = digits[3];
    tempDigits[2] = digits[4];
}

const char *dht::getTemperature()
{
    char *str;
    str = new char[6]{"00.0C"};
    str[0] = tempDigits[0];
    str[1] = tempDigits[1];
    str[3] = tempDigits[2];
    return str;
}

const char *dht::getHumidity()
{
    char *str;
    str = new char[6]{"HR00%"};
    str[2] = humiDigits[0];
    str[3] = humiDigits[1];
    return str;
}

void dht::readData(unsigned char *data)
{
    startSignal();
    if (checkResponse())
    {
        unsigned char i, j, num;
        unsigned interruptIn = 90;

        for (j = 0; j<5; j++)
        {
            num = 0;
            for (i = 8; i > 0; i--)
            {
                while (!(TST(P2IN, DHT_PIN)));//Wait for signal to go high
                pT->usInitTimer(&interruptIn);
                while (TST(P2IN, DHT_PIN)) //Wait for signal to go low
                {
                    if (pT->timeOut()) break;
                }
#ifdef DHT_TIMER1
                CLR(TA1CTL, 0x30); //Halt Timer
                if (TA1R > 11)     //40 @ 1x divider
                {
                    num |= 1 << (i - 1);
                }
#else
                CLR(TA0CTL, 0x30); //Halt Timer
                if (TA0R > 11)     //40 @ 1x divider
                {
                    num |= 1 << (i - 1);
                }
#endif
            }
            data[j] = num;
        }
    }
}

void dht::startSignal()
{
    unsigned wait1 = 20;
    unsigned wait2 = 30;
    SET(P2DIR, DHT_PIN);        // Set Data pin to output direction
    CLR(P2OUT, DHT_PIN);        // Low for at least 18ms
    pT->msWait(&wait1);
    SET(P2OUT, DHT_PIN);        // High for at 20us-40us
    pT->usWait(&wait2);
    CLR(P2DIR, DHT_PIN);        // Set data pin to input direction
}

bool dht::checkResponse()
{
    unsigned wait = 95;
    pT->usInitTimer(&wait);
    while (!(TST(P2IN, DHT_PIN)));
    {
        if (pT->timeOut()) return false;
    }
    pT->usInitTimer(&wait);
    while (TST(P2IN, DHT_PIN))
    {
        if (pT->timeOut()) return false;
    }
        //stopTimer(); // It's produces a delay that affects read (not use)
    return true;
}

bool dht::checkChecksum(unsigned char * tmp)
{
    if (tmp[4] != (tmp[0] + tmp[1] + tmp[2] + tmp[3]))
    {
        return false;
    }
    return true;
}

void dht::powerOn()
{
    SET (P2DIR, BIT_DHT_VCC);
}

void dht::powerOff()
{
    CLR(P2DIR, BIT_DHT_VCC);
}