/* 
 * File:   VL53L0X.c
 * Author: Jamie
 *
 * Created on 01 December 2018, 19:58
 */

#ifndef VL53L0X_H
#define VL53L0X_H

#include <stdint.h>
#include <stdbool.h>

// Data sheet not found

/*
 * Send data to VL5310X using I2C
 */
void vl53l0x_I2C_SendData(uint8_t slave_address, uint8_t device_register, uint8_t value);


/*
 * Receive data from VL5310X using I2C
 */
uint8_t vl53l0x_I2C_ReceiveData(uint8_t slave_address, uint8_t device_register);


/*
 * Setup VL5310X to desired settings using I2C
 */
void vl5310x_Setup(uint8_t slave_address);


/*
 * Setup VL5310X with initial values
 */
void vl5310x_Initialisation(uint8_t slave_address);


/*
 * Setup VL5310X to recommended tuning values
 */
void vl5310x_Tuning(uint8_t slave_address);


/*
 * Setup VL5310X to use specified timing budget (min 20000, max 200000) ~ affects accuracy
 */
void vl5310x_SetMeasurementTimingBudget(uint8_t slave_address, uint32_t targetTiming);


/*
 * Setup VL5310X to be in continuous reading mode
 */
void vl5310x_SetContinuousReading(uint8_t slave_address);

#endif //VL53L0X