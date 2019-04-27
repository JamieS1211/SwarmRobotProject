/*
 * File:   SPI.h
 * Author: Jamie
 *
 * Created on 22 April 2019, 12:44
 */

#ifndef SPI_H
#define SPI_H

#include "globals.h"

void mRF89XA_Setup(void);

void mRF89XA_Configuration(void);

void wait(uint8_t value);

uint8_t mRF89XA_DuplexTransmit(uint8_t byteOut);

void mRF89XA_ConfigWrite(uint8_t registerAddress, uint8_t byte);

uint8_t mRF89XA_ConfigRead(uint8_t registerAddress);

void mRF89XA_DataSend(uint8_t data[], uint8_t numberOfBytes);

uint8_t mRF89XA_DataFIFORead(void);


#endif //SPI_H