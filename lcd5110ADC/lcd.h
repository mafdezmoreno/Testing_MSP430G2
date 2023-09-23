#ifndef LCD_H_
#define LCD_H_
//based on: https://gist.github.com/DmitryMyadzelets/83b6e1384a1ee35a7f5f

//  LCD Connections:
//
//          5110 LCD                                                 MSP-EXP430G2
//       -------------                                           -------------------
//      |          GND|<-- Ground ------------------------------|J6     GND         |
//      |           BL|<-- Back-light (not connected)
//      |          VCC|<-- Vcc +3..5V --------------------------|J1.1   VCC         |
//      |             |
//      |          CLC|<-- Clock -------------------------------|J1.7   P1.5        |
//      |          DIN|<-- Data Input --------------------------|J2.15  P1.7        |
//      |           DC|<-- Data/Command (high/low) -------------|J1.3   P1.4        |
//      |           CE|<-- Chip Enable (active low) ------------|J1.2   P1.6        |
//      |          RST|<-- Reset -------------------------------|J2.16  RST
//

#include <msp430.h>
#include "PCD8544.h"

#define LCD5110_SCLK_PIN            BIT5
#define LCD5110_DN_PIN              BIT7
#define LCD5110_DC_PIN              BIT4
#define LCD5110_SCE_PIN             BIT6

#define LCD5110_SELECT              P1OUT &= ~LCD5110_SCE_PIN
#define LCD5110_DESELECT            P1OUT |= LCD5110_SCE_PIN
#define LCD5110_SET_COMMAND         P1OUT &= ~LCD5110_DC_PIN
#define LCD5110_SET_DATA            P1OUT |= LCD5110_DC_PIN
#define LCD5110_COMMAND             0
#define LCD5110_DATA                1

// LCD functions declaration
void writeToLcd(unsigned char dataCommand, unsigned char data);
void writeCharToLcd(char c);
void writeStringToLcd(const char *string, int numberCharacters);
void initLcd();

// Sets LCD address (x = 0..83, y = 0..5)
void setAddr(unsigned char xAddr, unsigned char yAddr);
void clearLcd();
void clearBank(unsigned char bank);


#endif /* LCD_H_ */
