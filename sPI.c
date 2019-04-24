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
    //Serial data out (SDO) - RA1/AN1
    //Serial data in (SDI) - RA2/AN2/VREF-/CVREF
    //CSCON - RA3/AN3/VREF+
    //CSDAT - RA4/T0CKI/C1OUT/RCV
         
    //Setting a TRISA bit (=1) will make the corresponding PORTA pin an input
    TRISAbits.RA0 = 0;
    TRISAbits.RA1 = 0;
    TRISAbits.RA2 = 1;
    TRISAbits.RA3 = 0;
    TRISAbits.RA4 = 0;
    
    CSCON = 1;
    CSDAT = 1;
}

void sPI_Configuration(void) {
    
    //General Configuration Registers
    sPI_ConfigWrite(0x00, 0x28); //GCONREG: GENERAL CONFIGURATION REGISTER p30
    sPI_ConfigWrite(0x01, 0x88); //DMODREG: DATA AND MODULATION CONFIGURATION REGISTER p31
    sPI_ConfigWrite(0x02, 0x03); //FDEVREG: FREQUENCY DEVIATION CONTROL REGISTER p32
    sPI_ConfigWrite(0x03, 0x07); //BRSREG: BIT RATE SET REGISTER p32
    sPI_ConfigWrite(0x04, 0x0C); //FLTHREG: FLOOR THRESHOLD CONTROL REGISTER p33
    sPI_ConfigWrite(0x05, 0x0F); //FIFOCREG: FIFO CONFIGURATION REGISTER p33
    sPI_ConfigWrite(0x06, 0x77); //R1CREG: R1 COUNTER SET REGISTER p34
    sPI_ConfigWrite(0x07, 0x64); //P1CREG: P1 COUNTER SET REGISTER p34
    sPI_ConfigWrite(0x08, 0x32); //S1CREG: S1 COUNTER SET REGISTER p35
    sPI_ConfigWrite(0x09, 0x74); //R2CREG: R2 COUNTER SET REGISTER p35
    sPI_ConfigWrite(0x0A, 0x62); //P2CREG: P2 COUNTER SET REGISTER p36
    sPI_ConfigWrite(0x0B, 0x32); //S2CREG: S2 COUNTER SET REGISTER  p36
    sPI_ConfigWrite(0x0C, 0x38); //PACREG: POWER AMPLIFIER CONTROL REGISTER p37
    
    //Interrupt Configuration Registers
    sPI_ConfigWrite(0x0D, 0x00); //FTXRXIREG: FIFO TRANSMIT AND RECEIVE INTERRUPT REQUEST CONFIGURATION REGISTER p38
    sPI_ConfigWrite(0x0E, 0x01); //FTPRIREG: FIFO TRANSMIT PLL AND RSSI INTERRUPT REQUEST CONFIGURATION REGISTER p40
    sPI_ConfigWrite(0x0F, 0x00); //RSTHIREG: RSSI THRESHOLD INTERRUPT REQUEST CONFIGURATION REGISTER p41
    
    
    //Receiver Configuration Registers
    sPI_ConfigWrite(0x10, 0xA3); //FILCREG: FILTER CONFIGURATION REGISTER p42
    sPI_ConfigWrite(0x11, 0x38); //PFCREG: POLYPHASE FILTER CONFIGURATION REGISTER p43
    sPI_ConfigWrite(0x12, 0x18); //SYNCREG: SYNC CONTROL REGISTER p44
    sPI_ConfigWrite(0x13, 0x07); //RESVREG: RESERVED REGISTER p45
    //sPI_ConfigWrite(0x14, 0x14); //STSREG: RSSI STATUS READ REGISTER p45
    sPI_ConfigWrite(0x15, 0x00); //OOKCREG: OOK CONFIGURATION REGISTER p46
    
    
    //Sync Word Configuration Registers
    sPI_ConfigWrite(0x16, 0x00); //SYNCV31REG:SYNCVALUEFIRSTBYTECONFIGURATIONREGISTER p47
    sPI_ConfigWrite(0x17, 0x00); //SYNCV23REG:SYNCVALUESECONDBYTECONFIGURATIONREGISTER p47
    sPI_ConfigWrite(0x18, 0x00); //SYNCV15REG:SYNCVALUETHIRDBYTECONFIGURATIONREGISTER p48
    sPI_ConfigWrite(0x19, 0x00); //SYNCV07REG:SYNCVALUEFOURTHBYTECONFIGURATIONREGISTER p48
    
    
    //Transmitter Configuration Registers
    sPI_ConfigWrite(0x1A, 0x7C); //TXCONREG: TRANSMIT PARAMETER CONFIGURATION REGISTER p49
    
    
    //Oscillator Configuration Registers
    sPI_ConfigWrite(0x1B, 0xBC); //CLKOUTREG: CLOCK OUTPUT CONTROL REGISTER p50
    
    
    //Packet Handling Configuration Registers
    sPI_ConfigWrite(0x1C, 0x00); //PLOADREG: PAYLOAD CONFIGURATION REGISTER p51
    sPI_ConfigWrite(0x1D, 0x00); //NADDSREG: NODE ADDRESS SET REGISTER p51
    sPI_ConfigWrite(0x1E, 0x48); //PKTCREG: PACKET CONFIGURATION REGISTER p52
    sPI_ConfigWrite(0x1F, 0x00); //FCRCREG: FIFO CRC CONFIGURATION REGISTER p53
    
    
    
    //Notes p90
    sPI_ConfigWrite(0x0E, 0x03); //FTPRIREG: FIFO TRANSMIT PLL AND RSSI INTERRUPT REQUEST CONFIGURATION REGISTER p40
    
    
    sPI_ConfigWrite(0x00, 0x48); //GCONREG: GENERAL CONFIGURATION REGISTER p30
    
    while ((sPI_ConfigRead(0x0E) & 0x02) != 0x02) {
        sPI_Wait();
        sPI_Wait();
        sPI_Wait();
    }
    
    sPI_ConfigWrite(0x00, 0x28); //GCONREG: GENERAL CONFIGURATION REGISTER p30
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

void sPI_ConfigWrite(uint8_t registerAddress, uint8_t byte) {
    uint8_t addressByte = 0x00;
    
    CSCON = 0;
    
    // Bits 7, 6 and 0 represent start, read/write and stop respectively
    addressByte |= (registerAddress << 1); // Append register address
    
    sPI_DuplexTransmit(addressByte);
    sPI_DuplexTransmit(byte);
    
    CSCON = 1;
}

uint8_t sPI_ConfigRead(uint8_t registerAddress) {
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