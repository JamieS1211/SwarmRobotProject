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

void sPI_Wait(void) {
    for (uint8_t i = 0; i < 200; i++);
}

uint8_t sPI_DuplexTransmit(uint8_t byteOut) {
    uint8_t byteIn = 0x00;

    for (uint8_t bit = 0x80; bit; bit >>= 1) { //Start bit as MSB only and shift right to test MSB to LSB
        
        SDO = ((byteOut & bit) == bit); //Bitwise AND operator (sets output - MOSI)

        sPI_Wait();

        SCK = 1;

        if (SDI == 1) {
            byteIn |= bit; //Bitwise OR operator (appends current bit if input is high - MISO)
        }

        sPI_Wait();

        SCK = 0;
    }

    return byteIn;
}

void sIP_ConfigWrite(uint8_t registerAddress, uint8_t byte) {
    uint8_t addressByte = 0x00;
    
    CSCON = 0;
    
    // Bits 7, 6 and 0 represent start, read/write and stop respectively
    addressByte |= (registerAddress << 1); // Append register address
    
    sPI_DuplexTransmit(addressByte);
    sPI_DuplexTransmit(byte);
    
    CSCON = 1;
}

uint8_t sIP_ConfigRead(uint8_t registerAddress) {
    uint8_t addressByte = 0x00;
    uint8_t value;
    
    CSCON = 0;
    
    // Bits 7, 6 and 0 represent start, read/write and stop respectively
    addressByte |= 0x40; // Append read/write bit to 1 (indicate read)
    addressByte |= (registerAddress << 1); // Append register address
    
    sPI_DuplexTransmit(addressByte);
    value = sPI_DuplexTransmit(0x00); // The data byte sent is irrelevant (set to 0x00)
    
    CSCON = 1;
    
    return value;
}