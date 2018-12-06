/*
 * File:   I2C.c
 * Author: Jamie
 *
 * Created on 30 November 2018, 17:24
 */


#include <xc.h>

#include "i2C.h"

//Pages 204 - 236 in data sheet

/*
 * Sets up I2C module as Master mode clock = FOSC/(4 * (SSPADD + 1)) - 8MHz/(4 * (SSPADD + 1))
 * Uses TRIS0 and TRIS1 for data and clock
 */
void i2C_Setup(void) {     
    //Two pins are used for data transfer:
    //Serial clock (SCL) ? RB1/AN10/INT1/SCK/SCL
    //Serial data (SDA) ? RB0/AN12/INT0/FLT0/SDI/SDA
    //The user must configure these pins as inputs by setting
    //the associated TRIS bits.
    
    //Setting a TRISB bit (=1) will make the corresponding PORTB pin an input
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    
    //MSSP Control Register 1 (SSPCON1)
    //MSSP Control Register 2 (SSPCON2)
    //MSSP Status Register (SSPSTAT)
    //Serial Receive/Transmit Buffer Register (SSPBUF)
    //MSSP Shift Register (SSPSR) ? Not directly accessible
    //MSSP Address Register (SSPADD) SSPCON1, SSPCON2 and SSPSTAT 
    
    
    //SSPSTAT [MMSP Status Register] (I2C mode) - page 203
    SSPSTATbits.SMP = 0;    //Slew Rate Control bit
    SSPSTATbits.CKE = 0;    //SMBus Select bit
    //SSPSTATbits.DA        //[READ ONLY] Data/Address bit 
    //SSPSTATbits.STOP      //[READ ONLY] Stop bit
    //SSPSTATbits.START     //[READ ONLY] Start bit
    //SSPSTATbits.RW        //[READ ONLY] Read/Wright Information bit
    //SSPSTATbits.BF        //[READ ONLY] Buffer Full Status bit
    
    
    //SSPCON1 [MMSP Control Register 1] (I2C mode) - page 204
    SSPCON1bits.WCOL = 0;   //Write Collision Detect bit
    SSPCON1bits.SSPOV = 0;  //Receive Overflow Indicator bit
    SSPCON1bits.SSPEN = 1;  //Master Synchronous Serial Port Enable bit 
    SSPCON1bits.CKP = 1;    //SCK Release Control bit (Stretching pulse)
    SSPCON1bits.SSPM3 = 1;  //Master Synchronous Serial Port Mode Select bits
    SSPCON1bits.SSPM2 = 0;
    SSPCON1bits.SSPM1 = 0;
    SSPCON1bits.SSPM0 = 0;
    //SSPM3:SSPM0 @ 1000 = I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
            
    //SSPCON2 [MMSP Control Register 2] (I2C master mode) - page 205
    SSPCON2bits.GCEN = 0;   //General Call Enable bit (Slave mode only) 
    SSPCON2bits.ACKSTAT = 0;//Acknowledge Status bit (Master Transmit mode only)
    SSPCON2bits.ACKDT = 0;  //Acknowledge Data bit (Master Receive mode only)
    SSPCON2bits.ACKEN = 0;  //Acknowledge Sequence Enable bit
    SSPCON2bits.RCEN = 0;   //Receive Enable bit (Master Receive mode only)
    SSPCON2bits.PEN = 0;    //Stop Condition Enable bit
    SSPCON2bits.RSEN = 0;   //Repeated Start Condition Enable bit
    SSPCON2bits.SEN = 0;    //Start Condition Enable/Stretch Enable bit
    
    //clock = FOSC/(4 * (SSPADD + 1)) - 2MHz/(SSPADD + 1)
    SSPADD = 0x27; // 100KHz - page 223 - 39
}



/*
 * Waits if the I2C module is busy
 */
void i2C_Wait(void) {
    while(SSPSTATbits.RW == 1 || SSPCON2bits.RCEN == 1 || SSPCON2bits.PEN == 1
            || SSPCON2bits.RSEN == 1 || SSPCON2bits.SEN == 1 ) {
    }
}



/*
 * I2C start for master
 */
void i2C_Start(void) {
    i2C_Wait();
    
    SSPCON2bits.SEN = 1;                //Enable start condition
    while (SSPCON2bits.SEN == 1);       //Wait for start condition to finish
    PIR1bits.SSPIF = 0;                 //Clear SSPIF
}



/*
 * I2C repeated start for master
 */
void i2C_RepeatedStart(void) {
    i2C_Wait();
    
    SSPCON2bits.RSEN = 1;               //Enable repeated start condition
    while (SSPCON2bits.SEN == 1);       //Wait for start condition to finish
    PIR1bits.SSPIF = 0;                 //Clear SSPIF
}



/*
 * I2C stop for master
 */
void i2C_Stop(void) {    
    SSPCON2bits.PEN = 1;                //Initiate stop condition
    while(SSPCON2bits.PEN == 1);        //Wait for stop condition to finish
}



/*
 * I2C send data
 * NOTE: i2C_address must be given as bits 7:1 Address, last bit as 0
 * This is so the last bit can be altered to give read or wright
 */
void i2C_SendData(unsigned char i2C_address, unsigned char bytes[], unsigned char numberOfBytes) {
    i2C_Start();
    
    SSPBUF = i2C_address;               //Load byte into buffer (Sent automatically)
                
    while(PIR1bits.SSPIF == 0);         //This is set after 9th clock
    PIR1bits.SSPIF = 0;                 //Clear SSPIF
        
    if (SSPCON2bits.ACKSTAT == 1) {     //No response was made
        i2C_Stop();
        return;
    }
    
    for (unsigned char i = 0; i < numberOfBytes; i++) {
        SSPBUF = bytes[i];              //Load byte into buffer (Sent automatically)
                
        while(PIR1bits.SSPIF == 0);     //This is set after 9th clock
        PIR1bits.SSPIF = 0;             //Clear SSPIF
        
        if (SSPCON2bits.ACKSTAT == 1) { //No response was made
           break;
        }
    }
    
    i2C_Stop();
}



/*
 * I2C receive data
 * NOTE: i2C_address must be given as bits 7:1 Address, last bit as 0
 * This is so the last bit can be altered to give read or wright
 */
void i2C_ReceiveData(unsigned char i2C_address, unsigned char sendBytes[], unsigned char numberOfSendBytes, unsigned char secondStartType, unsigned char *recievedBytesPointer, unsigned char numberOfReceivedBytes) {
    i2C_Start();
    
    
    //Send i2C address
    SSPBUF = i2C_address;               //Load byte into buffer (Sent automatically)
                
    while(PIR1bits.SSPIF == 0);         //This is set after 9th clock
    PIR1bits.SSPIF = 0;                 //Clear SSPIF
        
    if (SSPCON2bits.ACKSTAT == 1) {     //No response was made
        i2C_Stop();
        return;
    }
    
    
    // Send required data over I2C to request data
    for (unsigned char i = 0; i < numberOfSendBytes; i++) {
        SSPBUF = sendBytes[i];              //Load byte into buffer (Sent automatically)
                
        while(PIR1bits.SSPIF == 0);     //This is set after 9th clock
        PIR1bits.SSPIF = 0;             //Clear SSPIF
        
        if (SSPCON2bits.ACKSTAT == 1) { //No response was made
        i2C_Stop();
        return;
        }
    }
    
    if (secondStartType == RepeatedStart) {
        i2C_RepeatedStart();
    } else if (secondStartType == StopStart) {
        i2C_Stop();
        i2C_Start();
    }
    
    
    //Send i2C address (but modify to be for reading)
    SSPBUF = i2C_address + 1;           //Load byte into buffer (Sent automatically)
                
    while(PIR1bits.SSPIF == 0);         //This is set after 9th clock
    PIR1bits.SSPIF = 0;                 //Clear SSPIF
        
    if (SSPCON2bits.ACKSTAT == 1) {     //No response was made
        i2C_Stop();
        return;
    }
    
    
    // Save received bytes
    for (unsigned char i = 0; i < numberOfReceivedBytes; i++) {
        SSPCON2bits.RCEN = 1;           //Receive byte from slave
        
        while (SSPSTATbits.BF == 0);    //Wait for all bits to be received
        
        recievedBytesPointer[i] = SSPBUF;      //Save received byte
        
        SSPCON2bits.ACKDT = 1;          //Prepare to send NACK
        SSPCON2bits.ACKEN = 1;          //Initiate NACK
        while (SSPCON2bits.ACKEN);      //Wait for NACK to be finished
    }
    
    i2C_Stop();
}