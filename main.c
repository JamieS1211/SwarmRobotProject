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

#pragma config PLLDIV = 1           //PLL Prescaler Selection bits
#pragma config CPUDIV = OSC1_PLL2   //System Clock Postscaler Selection bits
#pragma config USBDIV = 1           //USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN =  1)
#pragma config FOSC = INTOSC_EC     //Oscillator Selection bits
#pragma config FCMEN = OFF          //Fail-Safe Clock Monitor Enable bit
#pragma config IESO = OFF           //Internal/External Oscillator Switchover bit
#pragma config PWRT = OFF           //Power-up Timer Enable bit
#pragma config BOR = ON             //Brown-out Reset Enable bits
#pragma config BORV = 3             //Brown-out Reset Voltage bits
#pragma config VREGEN = OFF         //USB Voltage Regulator Enable bit
#pragma config WDT = OFF            //Watchdog Timer Enable bit
#pragma config WDTPS = 32768        //Watchdog Timer Postscale Select bits
#pragma config CCP2MX = ON          //CCP2 MUX bit
#pragma config PBADEN = OFF         //PORTB A/D Enable bit
#pragma config LPT1OSC = OFF        //Low-Power Timer 1 Oscillator Enable bit
#pragma config MCLRE = OFF          //MCLR Pin Enable bit
#pragma config STVREN = ON          //Stack Full/Underflow Reset Enable bit
#pragma config LVP = ON             //Single-Supply ICSP Enable bit
#pragma config XINST = OFF          //Extended Instruction Set Enable bit
#pragma config CP0 = OFF            //Code Protection bit
#pragma config CP1 = OFF            //Code Protection bit
#pragma config CP2 = OFF            //Code Protection bit
#pragma config CP3 = OFF            //Code Protection bit
#pragma config CPB = OFF            //Boot Block Code Protection bit
#pragma config CPD = OFF            //Data EEPROM Code Protection bit
#pragma config WRT0 = OFF           //Write Protection bit
#pragma config WRT1 = OFF           //Write Protection bit
#pragma config WRT2 = OFF           //Write Protection bit
#pragma config WRT3 = OFF           //Write Protection bit
#pragma config WRTC = OFF           //Configuration Register Write Protection bit
#pragma config WRTB = OFF           //Boot Block Write Protection bit
#pragma config WRTD = OFF           //Data EEPROM Write Protection bit
#pragma config EBTR0 = OFF          //Table Read Protection bit
#pragma config EBTR1 = 1            //Table Read Protection bit
#pragma config EBTR2 = OFF          //Table Read Protection bit
#pragma config EBTR3 = OFF          //Table Read Protection bit
#pragma config EBTRB = OFF          //Boot Block Table Read Protection bit

// Pins and how this program uses them
/*
 * 
 * 1  - MCLR/VPP/RE3                - [PROGRAMMER VPP]
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
 * 27 - RB6/KBI2/PGC                - [TEST LED / PROGRAMMER PGC]
 * 28 - RB7/KBI3/PGD                - [TEST LED / PROGRAMMER PGD]
 * 
 */

void main(void) {   
    interrupts_Setup();
    i2C_Setup();
    vl5310x_Setup(); //TODO - This function is incomplete! 
    
    //Setup timer 0 (linked to interrupt)
    //Settings trigger interrupt approx. every 0.25 seconds
    T0CONbits.TMR0ON = 1;
    T0CONbits.T08BIT = 1;
    T0CONbits.T0CS = 0;
    T0CONbits.T0SE = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS2 = 1;
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS0 = 1;
    
    //<TEST CODE>
    //Set TRISB 7 to outputs (output result of TOF sensor)
    TRISBbits.TRISB7 = 0;
    TRISBbits.TRISB6 = 0;
    //</TEST CODE>
    
    while(1);
    
    return;
}