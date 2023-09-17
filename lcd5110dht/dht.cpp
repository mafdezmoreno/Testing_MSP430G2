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
}

bool dht::readDht()
{

    static unsigned oneSec = 1000;
    msWait(&oneSec);
    readData(data);
//    if (checkChecksum(data))
//    {
//        return true;
//    }
//    return false;
    return true;
}

const char *dht::getTemperature()
{
    char *str;
    str = new char[6]{"00.0C"};
    str[0] = data[2] / 10 + '0';
    str[1] = data[2] % 10 + '0';
    str[3] = data[3] % 10 + '0';
    return str;
}

const char *dht::getHumidity()
{
    char *str;
    str = new char[6]{"HR00%"};
    str[2] = data[0] / 10 + '0';
    str[3] = data[0] % 10 + '0';
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
    unsigned interruptIn = 100;
    for (i = 8; i > 0; i--)
    {
        while (!(TST(P2IN, DHT_PIN))); //Wait for signal to go high
        //usInitTimer0(&interruptIn);
        usInitTimer(&interruptIn);
        while (TST(P2IN, DHT_PIN)); //Wait for signal to go low
        //CLR(TA0CTL,0x30); //Halt Timer
        CLR(TA1CTL, 0x30); //Halt Timer
        //if (TAR > 13)	 //40 @ 1x divider
        if (TA1R > 13)     //40 @ 1x divider
        CLR(TA0CTL, 0x30); //Halt Timer
        if (TA0R > 11)     //40 @ 1x divider
        {
            num |= 1 << (i - 1);
        }
    }
    return num;
}

//bool dht::checkChecksum(unsigned char *data)
//{
//// Not working
////    if (data[4] != (data[0] + data[1] + data[2] + data[3]))
////    {
////        return false;
////    }
//    return true;
//}
