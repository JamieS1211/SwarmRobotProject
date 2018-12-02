/* 
 * File:   VL53L0X.c
 * Author: Jamie
 *
 * Created on 01 December 2018, 19:57
 */


#include "i2C.h"


/*
 * Send data to VL5310X using I2C
 */
void vl53l0x_i2C_SendData(unsigned char slave_address, unsigned char device_register, unsigned char value) {
    unsigned char data[2] = {device_register, value};
    i2C_SendData(slave_address, data, 2);
}


/*
 * Receive data from VL5310X using I2C
 */
unsigned char vl53l0x_I2C_ReceiveData(unsigned char slave_address, unsigned char device_register) {
    unsigned char sendData[1] = {device_register};
    unsigned char recievedData[1] = {0x00};
    i2C_ReceiveData(slave_address, sendData, 1, &recievedData, 1);  
    
    return recievedData[0];
}


/*
 * Setup VL5310X to desired settings using I2C
 */
void vl5310x_Setup(void) {
    //TODO Find documentation that gives details and configure as desired (Possibly Peter knows?)
}