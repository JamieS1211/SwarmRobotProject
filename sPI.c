/*
 * File:   I2C.c
 * Author: Jamie
 *
 * Created on 30 November 2018, 17:24
 */


#include <xc.h>

#include "globals.h"
#include "sPI.h"

//This is a software implementation of SPI (bit-banged) as the 

//Pages 23 in data sheet

void sPI_Setup(void) {
    //Serial clock (SCK) - RA0/AN0 
    //Serial data in (SDI) - RA1/AN1
    //Serial data out (SDO) - RA2/AN2/VREF-/CVREF
    //CSCON - RA3/AN3/VREF+
    //CSDAT - RA4/T0CKI/C1OUT/RCV
         
    //Setting a TRISA bit (=1) will make the corresponding PORTA pin an input
    TRISAbits.RA0 = 0;
    TRISAbits.RA1 = 1;
    TRISAbits.RA2 = 0;
    TRISAbits.RA3 = 0;
    TRISAbits.RA4 = 0;
}

void sPI_ToggleClock(void) {
    SCK = !SCK;
}

void sIP_ConfigWrite(uint8_t registerAddress, uint8_t byte) {
    // Start
    SCK = 0;
    CSCON = 0;
    SDO = 0;
    sPI_ToggleClock();
    
    // Indicate write mode
    sPI_ToggleClock();
    SDO = 0;
    sPI_ToggleClock();
    
        
    // Write address
    for (uint8_t i = 0; i < 5; i++) {
        sPI_ToggleClock();
        SDO = (registerAddress & 0x10) == 0x10; // Binary and operator checking against binary value 0001 0000 after required shift
        sPI_ToggleClock();
        
        registerAddress <<= 1;
    }
    
    
    // Stop
    SDO = 0;
    sPI_ToggleClock();
    sPI_ToggleClock();
        
    // Write byte
    for (uint8_t i = 0; i < 8; i++) {
        sPI_ToggleClock();
        SDO = (byte & 0x80) == 0x80; // Binary and operator checking against binary value 1000 0000 after required shift
        sPI_ToggleClock();
        
        byte <<= 1;
    }
    
    
    // End transmission
    sPI_ToggleClock();
    CSCON = 1;
    sPI_ToggleClock();
}

