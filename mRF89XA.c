/*
 * File:   I2C.c
 * Author: Jamie
 *
 * Created on 30 November 2018, 17:24
 */


#include <xc.h>

#include "globals.h"
#include "mRF89XA.h"

//This is a software implementation of SPI (bit-banged) as the 

//Pages 23 in data sheet

void mRF89XA_Setup(void) {
    //Serial clock (SCK) - RA0/AN0 
    //Serial data out (SDO) - RA1/AN1
    //Serial data in (SDI) - RA2/AN2/VREF-/CVREF
    //CSCON - RA3/AN3/VREF+
    //CSDAT - RA4/T0CKI/C1OUT/RCV
         
    //Setting ADCON1 bits to set all of portA to be digital
    ADCON1bits.PCFG0 = 1;
    ADCON1bits.PCFG1 = 1;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG3 = 1;
    
    //Setting a TRISA bit (=1) will make the corresponding PORTA pin an input
    TRISAbits.RA0 = 0;
    TRISAbits.RA1 = 0;
    TRISAbits.RA2 = 1;
    TRISAbits.RA3 = 0;
    TRISAbits.RA4 = 0;
    
    CSCON = 1;
    CSDAT = 1;
    
    mRF89XA_Configuration();
}

void mRF89XA_Configuration(void) {
    mRF89XA_ConfigWrite(0x00, 0x68); //GCONREG: GENERAL CONFIGURATION REGISTER
    
    mRF89XA_ConfigWrite(0x01, 0xA8); //DMODREG: DATA AND MODULATION CONFIGURATION REGISTER
    mRF89XA_ConfigWrite(0x05, 0x0F); //FIFOCREG: FIFO CONFIGURATION REGISTER
    mRF89XA_ConfigWrite(0x0D, 0x00); //FTXRXIREG: FIFO TRANSMIT AND RECEIVE INTERRUPT REQUEST CONFIGURATION REGISTER
    mRF89XA_ConfigWrite(0x0E, 0x11); //FTPRIREG: FIFO TRANSMIT PLL AND RSSI INTERRUPT REQUEST CONFIGURATION REGISTER
    
    mRF89XA_ConfigWrite(0x12, 0x3E); //SYNCREG: SYNC CONTROL REGISTER
    mRF89XA_ConfigWrite(0x16, 0x69); //SYNCV31REG:SYNCVALUEFIRSTBYTECONFIGURATIONREGISTER
    mRF89XA_ConfigWrite(0x17, 0x69); //SYNCV23REG:SYNCVALUEFIRSTBYTECONFIGURATIONREGISTER
    mRF89XA_ConfigWrite(0x18, 0x69); //SYNCV15REG:SYNCVALUEFIRSTBYTECONFIGURATIONREGISTER
    mRF89XA_ConfigWrite(0x19, 0x69); //SYNCV07REG:SYNCVALUEFIRSTBYTECONFIGURATIONREGISTER
}

void mRF89XA_Wait(void) {
    for (uint8_t i = 0; i < 2; i++);
}

uint8_t mRF89XA_DuplexTransmit(uint8_t byteOut) {
    uint8_t byteIn = 0x00;

    for (uint8_t bit = 0x80; bit; bit >>= 1) { //Start bit as MSB only and shift right to test MSB to LSB (when 0x00 is reached the condition is false)
        
        SDO = ((byteOut & bit) == bit); //Bitwise AND operator (sets output - MOSI)

        mRF89XA_Wait();

        SCK = 1;

        if (SDI == 1) {
            byteIn |= bit; //Bitwise OR operator (appends current bit if input is high - MISO)
        }

        mRF89XA_Wait();

        SCK = 0;
    }

    return byteIn;
}

void mRF89XA_ConfigWrite(uint8_t registerAddress, uint8_t byte) {
    uint8_t addressByte = 0x00;
    
    CSCON = 0;
    
    // Bits 7, 6 and 0 represent start, read/write and stop respectively
    addressByte |= (registerAddress << 1); // Append register address
    
    mRF89XA_DuplexTransmit(addressByte);
    mRF89XA_DuplexTransmit(byte);
    
    CSCON = 1;
}

uint8_t mRF89XA_ConfigRead(uint8_t registerAddress) {
    uint8_t addressByte = 0x00;
    uint8_t value;
    
    CSCON = 0;
    
    // Bits 7, 6 and 0 represent start, read/write and stop respectively
    addressByte |= 0x40; // Append read/write bit to 1 (indicate read)
    addressByte |= (registerAddress << 1); // Append register address
    
    mRF89XA_DuplexTransmit(addressByte);
    value = mRF89XA_DuplexTransmit(0x00); // The data byte sent is irrelevant (set to 0x00)
    
    CSCON = 1;
    
    return value;
}

void mRF89XA_DataSend(uint8_t data[], uint8_t numberOfBytes) {
    
    uint8_t startValue = mRF89XA_ConfigRead(0x00);
    
    bool modeChange = ((startValue & 0x80) != 0x80);
    
    if (modeChange) {
        mRF89XA_ConfigWrite(0x00, (startValue & 0x1F) | 0x80); //Bit mask first 3 bits to 0s, then OR operator to change to '100' (transmit mode)
    }
    
    for (uint8_t i = 0; i < numberOfBytes; i++) { 
        mRF89XA_Wait();
        
        CSDAT = 0;

        mRF89XA_DuplexTransmit(data[i]);

        CSDAT = 1;

        mRF89XA_Wait();
    }
    
    while ((mRF89XA_ConfigRead(0x0E) & 0x20) != 0x20);

    mRF89XA_Wait();
    
    if (modeChange) {
        mRF89XA_ConfigWrite(0x00, startValue); 
    }
}

void mRF89XA_DataFIFORead(uint8_t *recievedBytesPointer, uint8_t numberOfBytes) {
    
    uint8_t startValue = mRF89XA_ConfigRead(0x00);
    
    bool modeChange = ((startValue & 0x60) != 0x60);
    
    if (modeChange) {
        mRF89XA_ConfigWrite(0x00, (startValue & 0x1F) | 0x60); //Bit mask first 3 bits to 0s, then OR operator to change to '011' (reception mode)
    }
    
    for (uint8_t i = 0; i < numberOfBytes; i++) {
        CSDAT = 0;

        recievedBytesPointer[i] = mRF89XA_DuplexTransmit(0x00);

        CSDAT = 1;
        
        mRF89XA_Wait();
    }
    
    if (modeChange) {
        mRF89XA_ConfigWrite(0x00, startValue); 
    }
}