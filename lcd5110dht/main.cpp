#include "msp430g2553.h"
#include "lcd.h"
#include "dht.h"

const unsigned msPause = 500;

void initMcu()
{
    WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer

    BCSCTL1= CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    __bis_SR_register(GIE); // Enable interrupts
}

void initIo()
{
    P2OUT |= BIT_DHT_VCC;       // VCC to dht
    P2DIR |= BIT_DHT_VCC;

    P2OUT &= ~DISPLAY_LED_BIT;  // Drain current for display light
    P2DIR |= DISPLAY_LED_BIT;
}

void init(){

    initMcu();
    initIo();
    initLcd();
}

int main()
{

    init();

    setAddr(0, 0);
    writeStringToLcd("Hola", 5);

    char counter = '0';
    const char * str;

    while(1)
    {
        if (!readDht())
        {
            msWait(&msPause);
            continue;
        }

        str = getHumidity();
        setAddr(0, 1);
        writeStringToLcd(str, 4);
        delete[] str;

        str = getTemperature();
        setAddr(0, 2);
        writeStringToLcd(str, 5);
        delete[] str;

        setAddr(40, 0);
        writeStringToLcd(&counter, 1);
        counter++;
        if (counter > '9')
        {
            counter = '0';
        }

        msWait(&msPause);
    }
}
