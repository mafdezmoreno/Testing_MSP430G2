# Testing_MSP430G2. LCD_5110 + DHT

Encapsulated dht version, from Bryce Feigum, with the possibility of choose between timer0 and timer1.

Connections to microcontroller are in the heads of lcd.h and dht.h

Tested with msp430g22553 working at 3.3V and 5V. The lower voltaje (3.3V) can block some times the interruptions. I recomed work with 5volt to avoid problems.