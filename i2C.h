/*
 * File:   I2C.h
 * Author: Jamie
 *
 * Created on 30 November 2018, 17:24
 */

#ifndef I2C_H
#define I2C_H

//Pages 204 - 236 in data sheet

/*
 * Sets up I2C module as Master mode clock = FOSC/(4 * (SSPADD + 1)) - 8MHz/(4 * (SSPADD + 1))
 * Uses TRIS0 and TRIS1 for data and clock
 */
void i2C_Setup(void);



/*
 * Waits if the I2C module is busy
 */
void i2C_Wait(void);



/*
 * I2C start for master
 */
void i2C_Start(void);



/*
 * I2C start for master
 */
void i2C_RepeatedStart(void);



/*
 * I2C stop for master
 */
void i2C_Stop(void);



/*
 * I2C send data
 * NOTE: i2C_address must be given as bits 7:1 Address, last bit as 0
 * This is so the last bit can be altered to give read or wright
 */
void i2C_SendData(unsigned char i2C_address, unsigned char bytes[], unsigned char numberOfBytes);



/*
 * I2C receive data
 * NOTE: i2C_address must be given as bits 7:1 Address, last bit as 0
 * This is so the last bit can be altered to give read or wright
 */
void i2C_ReceiveData(unsigned char i2C_address, unsigned char sendBytes[], unsigned char numberOfSendBytes, unsigned char *recievedBytes, unsigned char numberOfReceivedBytes);



#endif //I2C_H