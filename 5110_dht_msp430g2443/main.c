/*References:
 * 5110 LCD:
 *      https://forum.43oh.com/topic/1312-nokia-5110-display
 *      https://gist.github.com/DmitryMyadzelets/83b6e1384a1ee35a7f5f
 * DHT:
 *      https://github.com/bafeigum/DHT11-Library-for-MSP430
 */


//**********************************************//
// MSP430G2553 + PCD8544LCD(Nokia 5110) + DHT22 //
//**********************************************//

#include <msp430g2553.h>
#include "PCD8544.h"
#include "DHT11_LIB.h"
#include "lcd_5110.h"

//DHT variables
unsigned char RH_byte1;
unsigned char RH_byte2;
unsigned char T_byte1;
unsigned char T_byte2;
unsigned char checksum;

unsigned char Packet[5];

unsigned char volatile TOUT;            //REQUIRED for library
unsigned char volatile SECOND_TIMER=0;

const unsigned ancho_digito = 6;

//DHT Functions
void init_dht(void);


void main(void) {

    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    /*  Default settings after reset:
    *
    *   Source of the Main system clock (MCLK) and sub-main system clock (SMCLK) is
    *   Internal Digitally Controlled Oscillator Clock (DCO).
    *   MCLK is about 1 MHz.
    *   VLO is about 12 kHz.
    */

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

    __delay_cycles(500000);
    initLCD();
    __delay_cycles(100000);
    init_dht();
    __delay_cycles(100000);
    unsigned char i = 0;
    unsigned int pos_impresion = 0;
    while(1) {

        //LCD
            clearLCD();

            setAddr(0, 0);
            writeStringToLCD("Testing");
            setAddr(1, 1);
            writeCharToLCD(i+0x30);



        //DHT

            //Must wait 1 second initially and between all reads
            if(SECOND_TIMER >= 5){      //5 @ CCR0 = 50000 & div 4

                //  Simple way to gather all data with one instruction
                read_Packet(Packet);
                RH_byte1 =  Packet[0];
                RH_byte2 =  Packet[1];
                T_byte1 =   Packet[2];
                T_byte2 =   Packet[3];
                checksum =  Packet[4];

                //if (check_Checksum(Packet))
                //    SET (P1OUT, 0x40);

                SET (TACTL, TACLR);
                SET (TA0CTL, 0x10);
                TACCR0 = 50000;     //Initialize the timer to count at 5Hz
                SECOND_TIMER = 0;   //Clear counter
            }

        pos_impresion = 10;
        setAddr(pos_impresion, 3);
        writeCharToLCD(RH_byte1/10+0x30);

        pos_impresion = ancho_digito + pos_impresion;
        setAddr(pos_impresion, 3);
        writeCharToLCD(RH_byte1%10+0x30);

        pos_impresion = ancho_digito + pos_impresion;
        setAddr(pos_impresion, 3);
        writeCharToLCD(0x2e);

        pos_impresion = ancho_digito + pos_impresion;
        setAddr(pos_impresion, 3);
        writeCharToLCD(RH_byte2/10+0x30);

        pos_impresion = ancho_digito + pos_impresion;
        setAddr(pos_impresion, 3);
        writeCharToLCD(RH_byte2%10+0x30);

        //Printing Temperature
        pos_impresion = 10;
        setAddr(pos_impresion, 4);
        writeCharToLCD(T_byte1/10+0x30);

        pos_impresion = ancho_digito + pos_impresion;
        setAddr(pos_impresion, 4);
        writeCharToLCD(T_byte1%10+0x30);

        pos_impresion = ancho_digito + pos_impresion;
        setAddr(pos_impresion, 4);
        writeCharToLCD(0x2e);

        pos_impresion = ancho_digito + pos_impresion;
        setAddr(pos_impresion, 4);
        writeCharToLCD(T_byte2%10+0x30);

        pos_impresion = ancho_digito + pos_impresion;
        setAddr(pos_impresion, 4);
        writeCharToLCD(T_byte2/10+0x30);


        // For debugging purposes
        i++;
        if (i>10){
            i = 0;
        }
        __delay_cycles(2000000);

    } // eof while()
} // eof main


void init_dht(){

    //P1OUT  = 0x00;              //Start with nothing being output
    //P1DIR  = 0x41;              // Set LED to output direction

    BCSCTL1 = CALBC1_1MHZ;      // Set oscillator to 1MHz
    DCOCTL = CALDCO_1MHZ;       // Set oscillator to 1MHz

    TACCR0 = 50000;             // Initialize the timer to count at 5Hz
    TACCTL0 = CCIE;             // Enable interrupt
    TA0CTL = TASSEL_2 + ID_2 + MC_1 + TACLR;    // SMCLK, div 4, up mode,
                                                // clear timer
    _enable_interrupt();                        //Enable global interrupt
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void CCR0_ISR(void){
        SECOND_TIMER++;
        TOUT=1;
        //TOG (P1OUT,0x01);
        CLR (TACCTL0, CCIFG);
}




