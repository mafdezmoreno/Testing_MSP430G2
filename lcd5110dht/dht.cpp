/*
 * Based on https://github.com/bafeigum/DHT11-Library-for-MSP430/tree/master from Bryce Feigum
 * Changes:
 * * Improved with encapsulations.
 * * Replaced callbacks to timer 0 with timer 1. Included functions and methods to use both.
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
    pT = new timer1;
}

dht::~dht()
{
    delete pT;
}

bool dht::readDht()
{

    static unsigned threeSec = 3000;
    static unsigned oneSec = 1000;
    unsigned char tmpData[5];
    unsigned char digits[5];

    SET (P2DIR, BIT_DHT_VCC);
    pT->msWait(&threeSec);
    readData(tmpData);
    CLR(P2DIR, BIT_DHT_VCC);
    pT->msWait(&oneSec);
    if (checkChecksum(tmpData))
    {
        digits[0] = tmpData[0] / 10 + '0';
        digits[1] = tmpData[0] % 10 + '0';
        digits[2] = tmpData[2] / 10 + '0';
        digits[3] = tmpData[2] % 10 + '0';
        digits[4] = tmpData[3] % 10 + '0';

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
        humiDigits[0] = digits[0];
        humiDigits[1] = digits[1];
        tempDigits[0] = digits[2];
        tempDigits[1] = digits[3];
        tempDigits[2] = digits[4];
        return true;
    }
    return false;
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
                CLR(TA1CTL, 0x30); //Halt Timer
                if (TA1R > 11)     //40 @ 1x divider
                {
                    num |= 1 << (i - 1);
                }
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
