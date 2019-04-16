/*
 * File:   I2C.h
 * Author: Jamie
 *
 * Created on 30 November 2018, 17:24
 */

#ifndef I2C_H
#define I2C_H

#include "globals.h"

//Second start type definitions
#define RepeatedStart 0
#define StopStart 1


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
void i2C_SendData(uint8_t i2C_address, uint8_t bytes[], uint8_t numberOfBytes);



/*
 * I2C receive data
 * NOTE: i2C_address must be given as bits 7:1 Address, last bit as 0
 * This is so the last bit can be altered to give read or wright
 */
void i2C_ReceiveData(uint8_t i2C_address, uint8_t sendBytes[], uint8_t numberOfSendBytes, uint8_t secondStartType, uint8_t *recievedBytesPointer, uint8_t numberOfReceivedBytes);



#endif //I2C_H