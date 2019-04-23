/*
 * File:   SPI.h
 * Author: Jamie
 *
 * Created on 22 April 2019, 12:44
 */

#ifndef SPI_H
#define SPI_H

#include "globals.h"

void sPI_Setup(void);

void sPI_Wait(void);

uint8_t sPI_DuplexTransmit(uint8_t byteOut);

void sIP_ConfigWrite(uint8_t registerAddress, uint8_t byte);

uint8_t sIP_ConfigRead(uint8_t registerAddress);


#endif //SPI_H