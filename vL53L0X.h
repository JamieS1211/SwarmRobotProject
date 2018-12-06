/* 
 * File:   VL53L0X.c
 * Author: Jamie
 *
 * Created on 01 December 2018, 19:58
 */

#ifndef VL53L0X_H
#define VL53L0X_H

// Data sheet not found

/*
 * Send data to VL5310X using I2C
 */
void vl53l0x_I2C_SendData(unsigned char slave_address, unsigned char device_register, unsigned char value);


/*
 * Receive data from VL5310X using I2C
 */
unsigned char vl53l0x_I2C_ReceiveData(unsigned char slave_address, unsigned char device_register);


/*
 * Setup VL5310X to desired settings using I2C
 */
void vl5310x_Setup(void);



#endif //VL53L0X