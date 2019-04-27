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
    TRISAbits.RA5 = 1;
    
    CSCON = 1;
    CSDAT = 1;
    
    mRF89XA_Configuration();
}

void mRF89XA_Configuration(void) {//General Configuration Registers
    mRF89XA_ConfigWrite(0x01, 0x8C); //DMODREG: DATA AND MODULATION CONFIGURATION REGISTER p31
    mRF89XA_ConfigWrite(0x02, 0x0A); //FDEVREG: FREQUENCY DEVIATION CONTROL REGISTER p32
    mRF89XA_ConfigWrite(0x03, 0x07); //BRSREG: BIT RATE SET REGISTER p32
    mRF89XA_ConfigWrite(0x04, 0x0C); //FLTHREG: FLOOR THRESHOLD CONTROL REGISTER p33
    mRF89XA_ConfigWrite(0x05, 0x01); //FIFOCREG: FIFO CONFIGURATION REGISTER p33
    mRF89XA_ConfigWrite(0x06, 0x7D); //R1CREG: R1 COUNTER SET REGISTER p34
    mRF89XA_ConfigWrite(0x07, 0x64); //P1CREG: P1 COUNTER SET REGISTER p34
    mRF89XA_ConfigWrite(0x08, 0x14); //S1CREG: S1 COUNTER SET REGISTER p35
    mRF89XA_ConfigWrite(0x09, 0x74); //R2CREG: R2 COUNTER SET REGISTER p35
    //mRF89XA_ConfigWrite(0x0A, 0x62); //P2CREG: P2 COUNTER SET REGISTER p36 - unused as RPS = 0 in GCONREG
    //mRF89XA_ConfigWrite(0x0B, 0x32); //S2CREG: S2 COUNTER SET REGISTER  p36 - unused as RPS = 0 in GCONREG
    //mRF89XA_ConfigWrite(0x0C, 0x38); //PACREG: POWER AMPLIFIER CONTROL REGISTER p37 - unused as RPS = 0 in GCONREG

     //Interrupt Configuration Registers
    mRF89XA_ConfigWrite(0x0D, 0x18); //FTXRXIREG: FIFO TRANSMIT AND RECEIVE INTERRUPT REQUEST CONFIGURATION REGISTER p38
    mRF89XA_ConfigWrite(0x0E, 0x3F); //FTPRIREG: FIFO TRANSMIT PLL AND RSSI INTERRUPT REQUEST CONFIGURATION REGISTER p40
    mRF89XA_ConfigWrite(0x0F, 0x00); //RSTHIREG: RSSI THRESHOLD INTERRUPT REQUEST CONFIGURATION REGISTER p41


     //Receiver Configuration Registers
    mRF89XA_ConfigWrite(0x10, 0xA3); //FILCREG: FILTER CONFIGURATION REGISTER p42
    mRF89XA_ConfigWrite(0x11, 0x38); //PFCREG: POLYPHASE FILTER CONFIGURATION REGISTER p43
    mRF89XA_ConfigWrite(0x12, 0x38); //SYNCREG: SYNC CONTROL REGISTER p44
    mRF89XA_ConfigWrite(0x15, 0x00); //OOKCREG: OOK CONFIGURATION REGISTER p46


     //Sync Word Configuration Registers
    mRF89XA_ConfigWrite(0x16, 0x21); //SYNCV31REG:SYNCVALUEFIRSTBYTECONFIGURATIONREGISTER p47
    mRF89XA_ConfigWrite(0x17, 0x22); //SYNCV23REG:SYNCVALUESECONDBYTECONFIGURATIONREGISTER p47
    mRF89XA_ConfigWrite(0x18, 0x23); //SYNCV15REG:SYNCVALUETHIRDBYTECONFIGURATIONREGISTER p48
    mRF89XA_ConfigWrite(0x19, 0x24); //SYNCV07REG:SYNCVALUEFOURTHBYTECONFIGURATIONREGISTER p48


     //Transmitter Configuration Registers
    mRF89XA_ConfigWrite(0x1A, 0x70); //TXCONREG: TRANSMIT PARAMETER CONFIGURATION REGISTER p49


     //Oscillator Configuration Registers
    mRF89XA_ConfigWrite(0x1B, 0x3C); //CLKOUTREG: CLOCK OUTPUT CONTROL REGISTER p50


     //Packet Handling Configuration Registers
    mRF89XA_ConfigWrite(0x1C, 0x01); //PLOADREG: PAYLOAD CONFIGURATION REGISTER p51
    mRF89XA_ConfigWrite(0x1D, 0x00); //NADDSREG: NODE ADDRESS SET REGISTER p51
    mRF89XA_ConfigWrite(0x1E, 0x60); //PKTCREG: PACKET CONFIGURATION REGISTER p52
    mRF89XA_ConfigWrite(0x1F, 0x00); //FCRCREG: FIFO CRC CONFIGURATION REGISTER p53
    
    
    
    mRF89XA_ConfigWrite(0x00, 0x48); //GCONREG: GENERAL CONFIGURATION REGISTER p30
    
    while ((mRF89XA_ConfigRead(0x0E) & 0x02) != 0x02);
    
    mRF89XA_ConfigWrite(0x00, 0x28); //GCONREG: GENERAL CONFIGURATION REGISTER p30
}

void wait(uint8_t value) {
    for (uint8_t i = 0; i < value; i++);
}

uint8_t mRF89XA_DuplexTransmit(uint8_t byteOut) {
    uint8_t byteIn = 0x00;

    for (uint8_t bit = 0x80; bit; bit >>= 1) { //Start bit as MSB only and shift right to test MSB to LSB (when 0x00 is reached the condition is false)
        
        SDO = ((byteOut & bit) == bit); //Bitwise AND operator (sets output - MOSI)

        wait(1);
        
        SCK = 1;

        if (SDI == 1) {
            byteIn |= bit; //Bitwise OR operator (appends current bit if input is high - MISO)
        }
        
        wait(1);
        
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
    bool modeChange = ((startValue & 0xE0) != 0x80);
    
    for (uint8_t i = 0; i < numberOfBytes; i++) { 
        CSDAT = 0;
        
        mRF89XA_DuplexTransmit(data[i]);
        
        CSDAT = 1;
        
        wait(1);
    }
    
    if (modeChange) {
        mRF89XA_ConfigWrite(0x00, (startValue & 0x1F) | 0x80); //Bit mask first 3 bits to 0s, then OR operator to change to '100' (transmit mode)
    }

    while (IRQ1 == 0);
    
    wait(32);
    
    if (modeChange) {
        mRF89XA_ConfigWrite(0x00, startValue); 
    }
}

void mRF89XA_DataFIFORead(void) {
    
    uint8_t startValue = mRF89XA_ConfigRead(0x00);
    bool modeChange = ((startValue & 0xE0) != 0x60);
    
    if (modeChange) {
        mRF89XA_ConfigWrite(0x00, (startValue & 0x1F) | 0x60); 
    }

    for (uint8_t i = 0; i < 200; i++) {
        wait(200);
    }
    
    for (uint8_t i = 0; i < 16; i++) {
        CSDAT = 0;

        mRF89XA_DuplexTransmit(0x00);

        CSDAT = 1;
        
        wait(1);
    }
    
    if (modeChange) {
        mRF89XA_ConfigWrite(0x00, startValue); 
    }
}