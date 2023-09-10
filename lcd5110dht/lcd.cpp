#include "lcd.h"


void writeToLcd(unsigned char dataCommand, unsigned char data)
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

void initLcd()
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

    __delay_cycles(100000);

    writeToLcd(LCD5110_COMMAND, PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
    writeToLcd(LCD5110_COMMAND, PCD8544_SETVOP | 0x3F);
    writeToLcd(LCD5110_COMMAND, PCD8544_SETTEMP | 0x02);
    writeToLcd(LCD5110_COMMAND, PCD8544_SETBIAS | 0x03);
    writeToLcd(LCD5110_COMMAND, PCD8544_FUNCTIONSET);
    writeToLcd(LCD5110_COMMAND, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

    __delay_cycles(100000);
    clearLcd();
}

void writeCharToLcd(char c)
{
    unsigned char i;
    for (i = 0; i < 5; i++)
    {
        //writeToLcd(LCD5110_DATA, font[c - 0x2D][i]);
        writeToLcd(LCD5110_DATA, font[c - 0x20][i]);
    }
    writeToLcd(LCD5110_DATA, 0);
}

void writeStringToLcd(const char *string, int numberCharacters)
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

void setAddr(unsigned char xAddr, unsigned char yAddr)
{
    writeToLcd(LCD5110_COMMAND, PCD8544_SETXADDR | xAddr);
    writeToLcd(LCD5110_COMMAND, PCD8544_SETYADDR | yAddr);
}

void clearBank(unsigned char bank)
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

void clearLcd()
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
