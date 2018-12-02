/*
 * File:   main.c
 * Author: Jamie
 *
 * Created on 29 November 2018, 14:50
 * 
 * Created for PIC18F2550
 * 
 * Data sheet PIC18F2455/2550/4455/4550
 * 2006 Microchip Technology Inc.
 */


#include <xc.h>

#include "interrupts.h"
#include "i2C.h"
#include "vL53L0X.h"

#pragma config WDT = OFF
#pragma config FOSC = INTOSC_EC // 8 MHz clock (±2% accuracy) 
#pragma config MCLRE = OFF

// Pins and how this program uses them
/*
 * 
 * 1  - MCLR/VPP/RE3                - [NONE]
 * 2  - RA0/AN0                     - [NONE]
 * 3  - RA1/AN1                     - [NONE]
 * 4  - RA2/AN2/VREF-/CVREF         - [NONE]
 * 5  - RA3/AN3/VREF+               - [NONE]
 * 6  - RA4/T0CKI/C1OUT/RCV         - [NONE]
 * 7  - RA5/AN4/SS/HLVDIN/C2OUT     - [NONE]
 * 8  - VSS                         - [+0V]
 * 9  - OSC1/CLKI                   - [NONE]
 * 10 - OSC2/CLKO/RA6               - [NONE]
 * 11 - RC0/T1OSO/T13CKI            - [NONE]
 * 12 - RC1/T1OSI/CCP2(1)/UOE       - [NONE]
 * 13 - RC2/CCP1                    - [NONE]
 * 14 - VUSB                        - [NONE]
 * 15 - RC4/D-/VM                   - [NONE]
 * 16 - RC5/D+/VP                   - [NONE]
 * 17 - RC6/TX/CK                   - [NONE]
 * 18 - RC7/RX/DT/SDO               - [NONE]
 * 19 - VSS                         - [+0V]
 * 20 - VDD                         - [+5V]
 * 21 - RB0/AN12/INT0/FLT0/SDI/SDA  - [SDA - I2C data pin]
 * 22 - RB1/AN10/INT1/SCK/SCL       - [SCL - I2C clock pin]
 * 23 - RB2/AN8/INT2/VMO            - [NONE]
 * 24 - RB3/AN9/CCP2(1)/VPO         - [NONE]
 * 25 - RB4/AN11/KBI0               - [NONE]
 * 26 - RB5/KBI1/PGM                - [NONE]
 * 27 - RB6/KBI2/PGC                - [NONE]
 * 28 - RB7/KBI3/PGD                - [TEST (output result of TOF sensor)]
 * 
 */

void main(void) {   
    interrupts_Setup();
    i2C_Setup();
    vl5310x_Setup(); //TODO - This function is incomplete! 
    
    //Setup timer 0 (linked to interrupt)
    //Settings trigger interrupt approx. every 4 seconds
    T0CONbits.TMR0ON = 1;
    T0CONbits.T08BIT = 0;
    T0CONbits.T0CS = 0;
    T0CONbits.T0SE = 1;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS2 = 1;
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS0 = 1;
    
    //<TEST CODE>
    //Set TRISB 7 to outputs (output result of TOF sensor)
    TRISBbits.TRISB7 = 0;
    //</TEST CODE>
    
    while(1);
    
    return;
}