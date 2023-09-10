/*
 * Based on https://github.com/bafeigum/DHT11-Library-for-MSP430/tree/master from Bryce Feigum
 * Changes:
 * * Improved with encapsulations.
 * * Replaced callbacks to timer 0 wiht timer 1. Included functions and methods to use both.
 */

#include "dht.h"

unsigned char checksum;
unsigned char data[5];
bool volatile timeOut0;
bool volatile timeOut1;

/// Not used
///! Warning, this timer works with less voltaje than
///! the timer 1 (at least in my hardware...)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void ccr0Isr(void)
{
    timeOut0 = true;
    //CLR (TACCTL0, CCIFG);
    stopTimer0();
}

/// To use in delays. Auto stopped after call
#pragma vector = TIMER1_A0_VECTOR
__interrupt void dhtTimer(void)
{
    timeOut1 = true;
    //CLR (TA1CCTL0, CCIFG);
    stopTimer1();
}

bool readDht(){

    static unsigned oneSec = 1000;
    msWait(&oneSec);
    readData(data);
    if (checkChecksum(data))
    {
        return true;
    }
    return false;
}

unsigned char readData(unsigned char * data)
{
    startSignal();
    if (checkResponse()){
        //Cannot be done with a for loop!
        data[0] = readByte();
        data[1] = readByte();
        data[2] = readByte();
        data[3] = readByte();
        data[4] = readByte();
        return 1;
    }
    else return 0;
}

void startSignal()
{
    unsigned wait1 = 20;
    unsigned wait2 = 30;
    SET(P2DIR, DHT_PIN);		// Set Data pin to output direction
    CLR(P2OUT,DHT_PIN); 		// Low for at least 18ms
    msWait(&wait1);
    SET(P2OUT,DHT_PIN);        // High for at 20us-40us
    usWait(&wait2);
    CLR(P2DIR,DHT_PIN);		// Set data pin to input direction
}

unsigned char checkResponse()
{
    unsigned wait = 100;
    usInitTimer1(&wait);
    while(!(TST(P2IN,DHT_PIN)) && !timeOut1);
    if (timeOut1)
    {
        return 0;
    }
    else
    {
        usInitTimer1(&wait);
        while((TST(P2IN,DHT_PIN)) && !timeOut1);
        if(timeOut1)
        {
            return 0;
        }
        else
        {
            stopTimer1();
            //CLR(TA1CCTL0,CCIE);	// Disable timer interrupt
            return 1;
        }
    }
}

unsigned char readByte()
{
    unsigned char num = 0;
    unsigned char i;
    unsigned interruptIn = 100;
    for (i=8; i>0; i--){
        while(!(TST(P2IN,DHT_PIN))); //Wait for signal to go high
        //usInitTimer0(&interruptIn);
        usInitTimer1(&interruptIn);
        while(TST(P2IN,DHT_PIN)); //Wait for signal to go low
        //CLR(TA0CTL,0x30); //Halt Timer
        CLR(TA1CTL,0x30); //Halt Timer
        //if (TAR > 13)	 //40 @ 1x divider
        if (TA1R > 13)	 //40 @ 1x divider
        {
            num |= 1 << (i-1);
        }
    }
    return num;
}

void usInitTimer0(const unsigned * us)
{
    TA0CCR0  = (unsigned)((*us)/4);          // Count up to limit
    TA0CTL |=  TASSEL_2 + ID_2 + MC_1 + TACLR;
    // SMCLK, div 4, up mode, restart, enable interrup
    TA0CCTL0 |= CCIE;
}

void usInitTimer1(const unsigned * us)
{
    timeOut1 = false;
    TA1CCR0  = (unsigned)((*us)/4);          // Count up to limit
    //TA1CTL = TASSEL_2 | MC_1 | TACLR;
    TA1CTL = TASSEL_2 + ID_2 + MC_1 + TACLR;
             // SMCLK, div 4, up mode, restart
    TA1CCTL0 |= CCIE;          // Enable interrupt
}

bool checkChecksum(unsigned char *data){
    if (data[4] != (data[0] + data[1] + data[2] + data[3])){
        return false;
    }
    return true;
}

const char * getTemperature()
{
    char * str;
    str = new char[6] {"00.0C"};
    str[0] = data[2]/10 + '0';
    str[1] = data[2]%10 + '0';
    str[3] = data[3]%10 + '0';
    return str;
}

const char * getHumidity()
{
    char * str;
    str = new char[5] {"00HR"};
    str[0] = data[0]/10 + '0';
    str[1] = data[0]%10 + '0';
    return str;
}

//void secInitTimer0(const unsigned * sec)
//{
//    TA0CCR0 = (unsigned long) (50000 * (*sec));
//    TA0CCTL0 = CCIE;				            // Enable interrupt
//    TA0CTL = TASSEL_2 + ID_2 + MC_1 + TACLR;	// SMCLK, div 4, up mode, restart
//}

void msInitTimer1(const unsigned * ms)
{
    timeOut1 = false;

    // TA1CTL |= TASSEL_1;     // ACLK as source (Current 32768Hz)
    // TA1CTL |= ID_3;         // DIV by 8 the prev source (Current 4096 Hz)
    // TA1CTL |= MC_1;         // Count Up mode
    // TA1CTL |= TACLR;        // Reset timer
    TA1CCTL0 |= CCIE;          // Enable interrupt
    TA1CCR0  = (*ms)*5;          // Count up to limit
    TA1CTL = TASSEL_1 | ID_3 | MC_1 | TACLR;
}

void msWait(const unsigned * msDelay)
{
    msInitTimer1(msDelay);
    while (true)
    {
        if(timeOut1)
        {
            break;
        }
    }
}

void usWait(const unsigned * usDelay)
{
    usInitTimer1(usDelay);
    while (true)
    {
        if(timeOut1)
        {
            break;
        }
    }
}

void stopTimer0()
{
    TA0CTL &= ~MC_3;         // Stop mode to conserve power
    CLR (TA0CCTL0, CCIFG);   // To allow other interrupts
}

void stopTimer1()
{
    TA1CTL &= ~MC_3;         // Stop mode to conserve power
    CLR (TA1CCTL0, CCIFG);   // To allow other interrupts
}

//void upModeTimer0()
//{
//    TA0CTL |= MC_1;
//    TA0CCTL0 |= CCIFG;   // To enable interrupts
//}

void upModeTimer1()
{
    TA1CTL |= MC_1;
    TA1CCTL0 |= CCIFG;   // To enable interrupts
}