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
unsigned char volatile DHT_TIMER=0;

#define Power_DHT BIT2  // Config on pin 2.2


//DHT Functions
void init_dht(void);
void print_humidity(unsigned char decena, unsigned char unidad);
void print_temp(unsigned char decena, unsigned char unidad, unsigned char decimal);
void dht_to_lcd_loop();
void on_dht();
void off_dht();


void init_dht(){

    //Configure power pin of dht as output
    P2DIR |= Power_DHT;
    P2OUT &= ~Power_DHT; //Initialized to LOW

    __delay_cycles(10000);

}

void print_humidity(unsigned char decena, unsigned char unidad){

    static const unsigned char fila = 5;
    unsigned char columna = 5;

    setAddr(columna, fila);
    writeStringToLCD("HR");

    columna =columna+13;
    setAddr(columna, fila);
    writeCharToLCD(decena);

    columna =columna+6;
    setAddr(columna, fila);
    writeCharToLCD(unidad);

    columna =columna+7;
    setAddr(columna, fila);
    writeStringToLCD("%");
}

void print_temp(unsigned char decena, unsigned char unidad, unsigned char decimal){

    static const unsigned char fila = 5;
    unsigned char columna = 50;

    setAddr(columna, 5);
    writeCharToLCD(decena);

    columna = columna +6;
    setAddr(columna, 5);
    writeCharToLCD(unidad);

    columna = columna +5;
    setAddr(columna, 5);
    writeCharToLCD(0x2e);  //dot print

    columna = columna +6;
    setAddr(columna, 5);
    writeCharToLCD(decimal);

    columna = columna + 7;
    setAddr(columna, 5);
    writeStringToLCD("C");

}

void dht_to_lcd_loop(){

    unsigned char Packet[5];

        on_dht();
        __delay_cycles(8000000);

        read_Packet(Packet);  //retrieve the data to Packet

        //RESTORE TIMER (Modified inside read_Packet
        TACCR0 = 50000;     //Initialize interrupt counter
        TA0CCTL0 |= CCIE;            // Enable interrupt
        TA0CTL  |= TASSEL_2 + ID_2 + MC_1 + TAIFG + TACLR;    // SMCLK, div 4, up mode,

        // print humidity
        print_humidity(((Packet[0])/10+0x30), ((Packet[0])%10+0x30));
        // print temperature
        print_temp((Packet[2]/10+0x30), (Packet[2]%10+0x30), (Packet[3]%10+0x30));

        off_dht();

}

void on_dht(){

    P2OUT |= Power_DHT;
}

void off_dht(){

    P2OUT &= ~Power_DHT;
}



#endif /* DHT_TO_LCD_H_ */
