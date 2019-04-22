/*
 * File:   SPI.h
 * Author: Jamie
 *
 * Created on 22 April 2019, 12:44
 */

#ifndef SPI_H
#define SPI_H

#include "globals.h"

#define SCK PORTAbits.RA0
#define SDI PORTAbits.RA1
#define SDO PORTAbits.RA2
#define CSCON PORTAbits.RA3
#define CSDAT PORTAbits.RA4



void sPI_Setup(void);

void sIP_ConfigWrite(uint8_t registerAddress, uint8_t byte);


#endif //SPI_H