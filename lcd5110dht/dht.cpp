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
    P2OUT |= BIT_DHT_VCC;       // VCC to dht
    P2DIR |= BIT_DHT_VCC;
    humiDigits[0] = '0';
    humiDigits[1] = '0';
    tempDigits[0] = '0';
    tempDigits[1] = '0';
    tempDigits[2] = '0';
}

bool dht::readDht()
{

    static unsigned oneSec = 3000;
    unsigned char tmpData[5];
    unsigned char digits[5];

    msWait(&oneSec);
    readData(tmpData);

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

unsigned char dht::readData(unsigned char *data)
{
    startSignal();
    if (checkResponse())
    {
        //Cannot be done with a for loop!
        data[0] = readByte();
        data[1] = readByte();
        data[2] = readByte();
        data[3] = readByte();
        data[4] = readByte();
        return 1;
    } else return 0;
}

void dht::startSignal()
{
    unsigned wait1 = 20;
    unsigned wait2 = 30;
    SET(P2DIR, DHT_PIN);        // Set Data pin to output direction
    CLR(P2OUT, DHT_PIN);        // Low for at least 18ms
    msWait(&wait1);
    SET(P2OUT, DHT_PIN);        // High for at 20us-40us
    usWait(&wait2);
    CLR(P2DIR, DHT_PIN);        // Set data pin to input direction
}

unsigned char dht::checkResponse()
{
    unsigned wait = 100;
    usInitTimer(&wait);
    while (!(TST(P2IN, DHT_PIN)) && !(timeOut()));
    if ((timeOut()))
    {
        return 0;
    } else
    {
        usInitTimer(&wait);
        while ((TST(P2IN, DHT_PIN)) && !(timeOut()));
        if ((timeOut()))
        {
            return 0;
        } else
        {
            stopTimer();
            return 1;
        }
    }
}

unsigned char dht::readByte()
{
    unsigned char num = 0;
    unsigned char i;
    unsigned interruptIn = 90;

    for (i = 8; i > 0; i--)
    {
        while (!(TST(P2IN, DHT_PIN)));//Wait for signal to go high
        usInitTimer(&interruptIn);
        while (TST(P2IN, DHT_PIN)) //Wait for signal to go low
        {
            if(timeOut()) break;
        }
        CLR(TA0CTL, 0x30); //Halt Timer
        if (TA0R > 11)     //40 @ 1x divider
        {
            num |= 1 << (i - 1);
        }
    }
    return num;
}

bool dht::checkChecksum(unsigned char * tmp)
{
    if (tmp[4] != (tmp[0] + tmp[1] + tmp[2] + tmp[3]))
    {
        return false;
    }
    return true;
}
