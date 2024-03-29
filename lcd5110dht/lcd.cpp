#include "lcd.h"
#include "timer.h"

lcd::lcd()
{
    initLcd();
    lcdOn();
    pD = new dht;
    dynamicAlloc = true;
}

lcd::lcd(dht * d)
{
    initLcd();
    lcdOn();
    pD = d;
    dynamicAlloc = false;
}

lcd::~lcd()
{
    if (dynamicAlloc)
    {
        delete pD;
    }
    delete pT;
}

void lcd::lcdOn()
{
#ifdef LCD_TIMER0
    pT = new timer0;
#else
    pT = new timer1;
#endif
    P2OUT &= ~DISPLAY_LED_BIT;  // Drain current for display light
    P2DIR |= DISPLAY_LED_BIT;
    strcpy (lastTemperature, "00.0C");
    strcpy (lastHumidity, "HR00%");
    refresh();
}

void lcd::printDht()
{
    static char counter = '0';

    pT->msWait(&waitOneSec);

    if (!pD->readDht())
    {
        return;
    }

    setAddr(0, 0);
    writeStringToLcd(&counter, 1);
    counter++;
    if (counter > '9')
    {
        counter = '0';
    }

    updateMeasures();
    refresh();
}

void lcd::refresh()
{
    setAddr(0, 1);
    writeStringToLcd(lastHumidity, 5);

    setAddr(0, 2);
    writeStringToLcd(lastTemperature, 5);
}

void lcd::writeToLcd(unsigned char dataCommand, unsigned char data)
{
    LCD5110_SELECT;

    if (dataCommand)
    {
        LCD5110_SET_DATA;
    } else
    {
        LCD5110_SET_COMMAND;
    }

    UCB0TXBUF = data;
    while (!(IFG2 & UCB0TXIFG));

    LCD5110_DESELECT;
}

void lcd::initLcd()
{

    P1OUT |= LCD5110_SCE_PIN | LCD5110_DC_PIN;  // Disable LCD, set Data mode
    P1DIR |= LCD5110_SCE_PIN | LCD5110_DC_PIN;  // Set pins to output direction

    // Setup USIB
    P1SEL |= LCD5110_SCLK_PIN | LCD5110_DN_PIN;
    P1SEL2 |= LCD5110_SCLK_PIN | LCD5110_DN_PIN;

    UCB0CTL0 |= UCCKPH | UCMSB | UCMST | UCSYNC; // 3-pin, 8-bit SPI master
    UCB0CTL1 |= UCSSEL_2;               // SMCLK
    UCB0BR0 |= 0x01;                    // 1:1
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST;               // clear SW

    pT->msWait(&waitOneSec);

    writeToLcd(LCD5110_COMMAND, PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
    writeToLcd(LCD5110_COMMAND, PCD8544_SETVOP | 0x3F);
    writeToLcd(LCD5110_COMMAND, PCD8544_SETTEMP | 0x02);
    writeToLcd(LCD5110_COMMAND, PCD8544_SETBIAS | 0x03);
    writeToLcd(LCD5110_COMMAND, PCD8544_FUNCTIONSET);
    writeToLcd(LCD5110_COMMAND, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

    pT->msWait(&waitOneSec);
    clearLcd();
}

void lcd::writeCharToLcd(unsigned int c)
{
    unsigned int i;
    for (i = 5; i > 0; --i)
    {
        //writeToLcd(LCD5110_DATA, font[c - 0x2D][i]);
        writeToLcd(LCD5110_DATA, font[c - 0x20][5-i]);
    }
    writeToLcd(LCD5110_DATA, 0);
}

void lcd::writeStringToLcd(const char *string, int numberCharacters)
{
    int i = numberCharacters;
    while (*string)
    {
        i--;
        writeCharToLcd(*string);
        if (i == 0)
            return;
        *string++;
    }
}

void lcd::setAddr(unsigned char xAddr, unsigned char yAddr)
{
    writeToLcd(LCD5110_COMMAND, PCD8544_SETXADDR | xAddr);
    writeToLcd(LCD5110_COMMAND, PCD8544_SETYADDR | yAddr);
}

void lcd::clearBank(unsigned char bank)
{
    setAddr(0, bank);
    int i = 0;
    while (i < PCD8544_HPIXELS)
    {
        writeToLcd(LCD5110_DATA, 0);
        i++;
    }
    setAddr(0, bank);
}

void lcd::clearLcd()
{
    setAddr(0, 0);
    int i = 0;
    while (i < PCD8544_MAXBYTES)
    {
        writeToLcd(LCD5110_DATA, 0);
        i++;
    }
    setAddr(0, 0);
}
void lcd::updateMeasures()
{
    const char * str;

    str = pD->getHumidity();
    strcpy(lastHumidity, str);
    delete[] str;

    str = pD->getTemperature();
    strcpy(lastTemperature, str);
    delete[] str;
}
