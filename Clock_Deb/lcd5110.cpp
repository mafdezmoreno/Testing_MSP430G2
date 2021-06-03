/*
 * lcd5110.cpp
 *
 *  Created on: May 31, 2021
 *      Author: mafm
 */

#include "lcd5110.h"
#include "msp430g2553.h"

lcd_5110::lcd_5110()
{
    // TODO Auto-generated constructor stub

}

lcd_5110::~lcd_5110()
{
    // TODO Auto-generated destructor stub
}

// LCD functions implementation



void writeToLCD(unsigned char dataCommand, unsigned char data) {
    LCD5110_SELECT;

    if(dataCommand) {
        LCD5110_SET_DATA;
    } else {
        LCD5110_SET_COMMAND;
    }

    UCB0TXBUF = data;
    while(!(IFG2 & UCB0TXIFG));
    LCD5110_DESELECT;
}

void initLCD() {

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

    __delay_cycles(50000);

    writeToLCD(LCD5110_COMMAND, PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETVOP | 0x3F);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETTEMP | 0x02);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETBIAS | 0x03);
    writeToLCD(LCD5110_COMMAND, PCD8544_FUNCTIONSET);
    writeToLCD(LCD5110_COMMAND, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

    __delay_cycles(50000);
    clearLCD();
    //writeStringToLCD("iniciando...");
   // __delay_cycles(2000000);

}

void writeCharToLCD(char c) {
    unsigned char i;
    for(i = 0; i < 5; i++) {
        writeToLCD(LCD5110_DATA, font[c - 0x2D][i]);
    }
    writeToLCD(LCD5110_DATA, 0);
}

void writeStringToLCD(const char *string, int number_characters)
{
    int i = number_characters;
    while(*string){
        i--;
        writeCharToLCD(*string);
        if (i == 0)
            return;
        *string++;
    }
}


void setAddr(unsigned char xAddr, unsigned char yAddr) {
    writeToLCD(LCD5110_COMMAND, PCD8544_SETXADDR | xAddr);
    writeToLCD(LCD5110_COMMAND, PCD8544_SETYADDR | yAddr);
}

void clearBank(unsigned char bank) {
    setAddr(0, bank);
    int i = 0;
    while(i < PCD8544_HPIXELS) {
        writeToLCD(LCD5110_DATA, 0);
        i++;
    }
    setAddr(0, bank);
}

void clearLCD() {
    setAddr(0, 0);
    int i = 0;
    while(i < PCD8544_MAXBYTES) {
        writeToLCD(LCD5110_DATA, 0);
        i++;
    }
    setAddr(0, 0);
}
