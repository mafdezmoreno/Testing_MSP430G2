/*
 * hal_io.h
 *
 *  Created on: May 23, 2021
 *      Author: mafm
 */

#ifndef HAL_IO_H_
#define HAL_IO_H_


void set_output(unsigned char pin, unsigned char port);
void toggle_pin(unsigned char pin, unsigned char port);

void set_output(unsigned char pin, unsigned char port){

    if (port == BIT1)
        P1DIR |= pin;

    else if (port == BIT2)
        P2DIR |= pin;

}

void set_high_pin(unsigned char pin, unsigned char port){

    if (port == BIT1)
        P1DIR |= pin;

    else if (port == BIT2)
        P2DIR |= pin;
}

void set_low_pin(unsigned char pin, unsigned char port){

    if (port == BIT1)
        P1OUT &= ~ pin;

    else if (port == BIT2)
        P2OUT &= ~ pin;
}


void toggle_pin(unsigned char pin, unsigned char port){

    if (port == BIT1)
        P1OUT ^= pin;

    else if (port == BIT2)
        P2OUT ^= pin;
}

#endif /* HAL_IO_H_ */
