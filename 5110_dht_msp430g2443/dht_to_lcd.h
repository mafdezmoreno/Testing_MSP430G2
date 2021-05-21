/*
 * dht_to_lcd.h
 *
 *  Created on: May 21, 2021
 *      Author: mafm
 */

#ifndef DHT_TO_LCD_H_
#define DHT_TO_LCD_H_

//DHT variables
//
unsigned char volatile TOUT;            //REQUIRED for library
unsigned char volatile SECOND_TIMER=0;

//DHT Functions
void init_dht(void);
void print_humidity(unsigned char decena, unsigned char unidad);
void print_temp(unsigned char decena, unsigned char unidad, unsigned char decimal);
void dht_to_lcd_loop();


void init_dht(){

    //P1OUT  = 0x00;              //Start with nothing being output

    BCSCTL1 = CALBC1_1MHZ;      // Set oscillator to 1MHz
    DCOCTL = CALDCO_1MHZ;       // Set oscillator to 1MHz

    //Timer 0 configuration
    TACCR0 = 50000;             // Initialize the timer to count at 5Hz
    TACCTL0 = CCIE;             // Enable interrupt
    TA0CTL = TASSEL_2 + ID_2 + MC_1 + TACLR;    // SMCLK, div 4, up mode,
                                                // clear timer
    _enable_interrupt();                        //Enable global interrupt

    __delay_cycles(20000);

}

void print_humidity(unsigned char decena, unsigned char unidad){

    setAddr(10, 3);
    writeCharToLCD(decena);

    setAddr(16, 3);
    writeCharToLCD(unidad);
}

void print_temp(unsigned char decena, unsigned char unidad, unsigned char decimal){

    setAddr(10, 4);
    writeCharToLCD(decena);

    setAddr(16, 4);
    writeCharToLCD(unidad);

    setAddr(22, 4);
    writeCharToLCD(0x2e);  //dot print

    setAddr(28, 4);
    writeCharToLCD(decimal);
}

void dht_to_lcd_loop(){

    unsigned char Packet[5];

    //Must wait 1 second initially and between all reads
    if(SECOND_TIMER >= 5){      //5 @ CCR0 = 50000 & div 4

        read_Packet(Packet);  //retrieve the data to Packet

        SET (TACTL, TACLR);
        SET (TA0CTL, 0x10);
        TACCR0 = 50000;     //Initialize the timer to count at 5Hz
        SECOND_TIMER = 0;   //Clear counter
    }

    // print humidity
    print_humidity(((Packet[0])/10+0x30), ((Packet[0])%10+0x30));
    // print temperature
    print_temp((Packet[2]/10+0x30), (Packet[2]%10+0x30), (Packet[3]%10+0x30));
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void CCR0_ISR(void){
        SECOND_TIMER++;
        TOUT=1;
        //TOG (P1OUT,0x01);
        CLR (TACCTL0, CCIFG);
}

#endif /* DHT_TO_LCD_H_ */
