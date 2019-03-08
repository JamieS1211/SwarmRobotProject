/* 
 * File:   VL53L0X.c
 * Author: Jamie
 *
 * Created on 01 December 2018, 19:57
 */

#include "vL53L0X.h"
#include "i2C.h"


/*
 * Send data to VL5310X using I2C
 */
void vl53l0x_I2C_SendData(uint8_t slave_address, uint8_t device_register, uint8_t value) {
    uint8_t data[2] = {device_register, value};
    i2C_SendData(slave_address, data, 2);
}


/*
 * Receive data from VL5310X using I2C
 */
uint8_t vl53l0x_I2C_ReceiveData(uint8_t slave_address, uint8_t device_register) {
    uint8_t sendData[1] = {device_register};
    uint8_t recievedData[1] = {0x00};
    i2C_ReceiveData(slave_address, sendData, 1, StopStart, recievedData, 1);  
    
    return recievedData[0];
}


/*
 * Setup VL5310X to desired settings using I2C
 */
void vl5310x_Setup(uint8_t slave_address) {
    //TODO Find documentation that gives details and configure as desired (Possibly Peter knows?)
    //vl5310x_Initialisation(slave_address);
    //vl5310x_Tuning(slave_address);
    vl5310x_SetContinuousReading(slave_address);
}


/*
 * Setup VL5310X with initial values
 */
void vl5310x_Initialisation(uint8_t slave_address) {
    
    vl53l0x_I2C_SendData(slave_address, 0x89, 0x01);   //VHV_CONFIG_PAD_SCL_SDA__EXTSUP_HV
	vl53l0x_I2C_SendData(slave_address, 0x80, 0x01);   //POWER_MANAGEMENT_GO1_POWER_FORCE
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x01);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x00, 0x00);   //SYSRANGE_START
	vl53l0x_I2C_ReceiveData(slave_address, 0x09);   //SYSTEM_RANGE_CONFIG 1
	vl53l0x_I2C_SendData(slave_address, 0x00, 0x01);   //SYSRANGE_START
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x80, 0x00);   //POWER_MANAGEMENT_GO1_POWER_FORCE
	vl53l0x_I2C_ReceiveData(slave_address, 0x60);   //MSRC_CONFIG_CONTROL
	vl53l0x_I2C_SendData(slave_address, 0x60, 0x12);   //MSRC_CONFIG_CONTROL
	//Write 16bit: 0x20, 0x44);   //FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT
	vl53l0x_I2C_SendData(slave_address, 0x01, 0xFF);   //SYSTEM_SEQUENCE_CONFIG
	vl53l0x_I2C_SendData(slave_address, 0x80, 0x01);   //POWER_MANAGEMENT_GO1_POWER_FORCE
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x01);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x00, 0x00);   //SYSRANGE_START
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x06);   //UNKNOWN
	vl53l0x_I2C_ReceiveData(slave_address, 0x83);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x83, 0x04);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x07);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x81, 0x01);   //SYSTEM_HISTOGRAM_BIN
	vl53l0x_I2C_SendData(slave_address, 0x80, 0x01);   //POWER_MANAGEMENT_GO1_POWER_FORCE
	vl53l0x_I2C_SendData(slave_address, 0x09, 0x6B);   //SYSTEM_RANGE_CONFIG 4
	vl53l0x_I2C_SendData(slave_address, 0x83, 0x00);   //UNKNOWN
	//While loop to ensure that register 0x83 is not equal to... line 247
	vl53l0x_I2C_SendData(slave_address, 0x83, 0x1);   //UNKNOWN
	vl53l0x_I2C_ReceiveData(slave_address, 0x09);   //SYSTEM_RANGE_CONFIG 2
	vl53l0x_I2C_SendData(slave_address, 0x81, 0x00);   //SYSTEM_HISTOGRAM_BIN
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x06);   //UNKNOWN
	vl53l0x_I2C_ReceiveData(slave_address, 0x83);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x83, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x01);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x00, 0x01);   //SYSRANGE_START
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x80, 0x00);   //POWER_MANAGEMENT_GO1_POWER_FORCE
//	vl53l0x_I2C_ReceiveData(slave_address, 0xB0);   //GLOBAL_CONFIG_SPAD_ENABLES_REF_0
//	vl53l0x_I2C_ReceiveData(slave_address, 0xB1);   //GLOBAL_CONFIG_SPAD_ENABLES_REF_1
//	vl53l0x_I2C_ReceiveData(slave_address, 0xB2);   //GLOBAL_CONFIG_SPAD_ENABLES_REF_2
//	vl53l0x_I2C_ReceiveData(slave_address, 0xB3);   //GLOBAL_CONFIG_SPAD_ENABLES_REF_3
//	vl53l0x_I2C_ReceiveData(slave_address, 0xB4);   //GLOBAL_CONFIG_SPAD_ENABLES_REF_4
//	vl53l0x_I2C_ReceiveData(slave_address, 0xB5);   //GLOBAL_CONFIG_SPAD_ENABLES_REF_5
//	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x01);   //UNKNOWN
//	vl53l0x_I2C_SendData(slave_address, 0x4F, 0x00);   //DYNAMIC_SPAD_REF_EN_START_OFFSET
//	vl53l0x_I2C_SendData(slave_address, 0x4E, 0x2C);   //DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD
//	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x00);   //UNKNOWN
//	vl53l0x_I2C_SendData(slave_address, 0xB6, 0xB4);   //GLOBAL_CONFIG_REF_EN_START_SELECT
//	vl53l0x_I2C_SendData(slave_address, 0xB0, 0x00);   //RESULT_PEAK_SIGNAL_RATE_REF_0
//	vl53l0x_I2C_SendData(slave_address, 0xB1, 0x00);   //RESULT_PEAK_SIGNAL_RATE_REF_1
//	vl53l0x_I2C_SendData(slave_address, 0xB2, 0x00);   //RESULT_PEAK_SIGNAL_RATE_REF_2
//	vl53l0x_I2C_SendData(slave_address, 0xB3, 0x00);   //RESULT_PEAK_SIGNAL_RATE_REF_3
//	vl53l0x_I2C_SendData(slave_address, 0xB4, 0x00);   //RESULT_PEAK_SIGNAL_RATE_REF_4
//	vl53l0x_I2C_SendData(slave_address, 0xB5, 0x00);   //RESULT_PEAK_SIGNAL_RATE_REF_5
    
    
    vl5310x_Tuning(slave_address);
    
    
    vl53l0x_I2C_SendData(slave_address, 0x0A, 0x04);   //SYSTEM_INTERRUPT_CONFIG_GPIO
	vl53l0x_I2C_ReceiveData(slave_address, 0x84);   //GPIO_HV_MUX_ACTIVE_HIGH
	vl53l0x_I2C_SendData(slave_address, 0x84, 0x00);   //GPIO_HV_MUX_ACTIVE_HIGH
	vl53l0x_I2C_SendData(slave_address, 0x0B, 0x01);   //SYSTEM_INTERRUPT_CLEAR
	vl53l0x_I2C_ReceiveData(slave_address, 0x01);   //SYSTEM_SEQUENCE_CONFIG
	vl53l0x_I2C_ReceiveData(slave_address, 0x50);   //PRE_RANGE_CONFIG_VCSEL_PERIOD
	vl53l0x_I2C_ReceiveData(slave_address, 0x46);   //MSRC_CONFIG_TIMEOUT_MACROP
	vl53l0x_I2C_ReceiveData(slave_address, 0x51);   //PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI
	vl53l0x_I2C_ReceiveData(slave_address, 0x52);   //PRE_RANGE_CONFIG_TIMEOUT_MACROP_LO
	vl53l0x_I2C_ReceiveData(slave_address, 0x70);   //FINAL_RANGE_CONFIG_VCSEL_PERIOD
	vl53l0x_I2C_ReceiveData(slave_address, 0x71);   //FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI
	vl53l0x_I2C_ReceiveData(slave_address, 0x72);   //FINAL_RANGE_CONFIG_TIMEOUT_MACROP_LO
	vl53l0x_I2C_SendData(slave_address, 0x01, 0xE8);   //SYSTEM_SEQUENCE_CONFIG
	vl53l0x_I2C_SendData(slave_address, 0x01, 0x01);   //SYSTEM_SEQUENCE_CONFIG
	vl53l0x_I2C_SendData(slave_address, 0x00, 0x41);   //SYSRANGE_START
	//While loop to ensure... line 713
	vl53l0x_I2C_SendData(slave_address, 0x0B, 0x01);   //SYSTEM_INTERRUPT_CLEAR
	vl53l0x_I2C_SendData(slave_address, 0x00, 0x00);   //SYSRANGE_START
	vl53l0x_I2C_SendData(slave_address, 0x01, 0x02);   //SYSTEM_SEQUENCE_CONFIG
	vl53l0x_I2C_SendData(slave_address, 0x00, 0x01);   //SYSRANGE_START
	//While loop to ensure... line 713
	vl53l0x_I2C_SendData(slave_address, 0x0B, 0x01);   //SYSTEM_INTERRUPT_CLEAR
	vl53l0x_I2C_SendData(slave_address, 0x00, 0x00);   //SYSRANGE_START
	vl53l0x_I2C_SendData(slave_address, 0x01, 0xE8);   //SYSTEM_SEQUENCE_CONFIG
}


/*
 * Setup VL5310X to recommended tuning values
 */
void vl5310x_Tuning(uint8_t slave_address) {
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x01);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x00, 0x00);   //SYSRANGE_START
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x09, 0x00);   //SYSTEM_RANGE_CONFIG 
	vl53l0x_I2C_SendData(slave_address, 0x10, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x11, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x24, 0x01);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x25, 0xFF);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x75, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x01);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x4E, 0x2C);   //DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD
	vl53l0x_I2C_SendData(slave_address, 0x48, 0x00);   //FINAL_RANGE_CONFIG_VALID_PHASE_HIGH
	vl53l0x_I2C_SendData(slave_address, 0x30, 0x20);   //ALGO_PHASECAL_LIM
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x30, 0x09);   //ALGO_PHASECAL_LIM
	vl53l0x_I2C_SendData(slave_address, 0x54, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x31, 0x04);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x32, 0x03);   //GLOBAL_CONFIG_VCSEL_WIDTH 
	vl53l0x_I2C_SendData(slave_address, 0x40, 0x83);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x46, 0x25);   //MSRC_CONFIG_TIMEOUT_MACROP
	vl53l0x_I2C_SendData(slave_address, 0x60, 0x00);   //MSRC_CONFIG_CONTROL
	vl53l0x_I2C_SendData(slave_address, 0x27, 0x00);   //PRE_RANGE_CONFIG_MIN_SNR
	vl53l0x_I2C_SendData(slave_address, 0x50, 0x06);   //PRE_RANGE_CONFIG_VCSEL_PERIOD
	vl53l0x_I2C_SendData(slave_address, 0x51, 0x00);   //PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI
	vl53l0x_I2C_SendData(slave_address, 0x52, 0x96);   //PRE_RANGE_CONFIG_TIMEOUT_MACROP_LO
	vl53l0x_I2C_SendData(slave_address, 0x56, 0x08);   //PRE_RANGE_CONFIG_VALID_PHASE_LOW
	vl53l0x_I2C_SendData(slave_address, 0x57, 0x30);   //PRE_RANGE_CONFIG_VALID_PHASE_HIGH
	vl53l0x_I2C_SendData(slave_address, 0x61, 0x00);   //PRE_RANGE_CONFIG_SIGMA_THRESH_HI
	vl53l0x_I2C_SendData(slave_address, 0x62, 0x00);   //PRE_RANGE_CONFIG_SIGMA_THRESH_LO
	vl53l0x_I2C_SendData(slave_address, 0x64, 0x00);   //PRE_RANGE_MIN_COUNT_RATE_RTN_LIMIT
	vl53l0x_I2C_SendData(slave_address, 0x65, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x66, 0xA0);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x01);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x22, 0x32);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x47, 0x14);   //FINAL_RANGE_CONFIG_VALID_PHASE_LOW
	vl53l0x_I2C_SendData(slave_address, 0x49, 0xFF);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x4A, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x7A, 0x0A);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x7B, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x78, 0x21);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x01);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x23, 0x34);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x42, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x44, 0xFF);   //FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT
	vl53l0x_I2C_SendData(slave_address, 0x45, 0x26);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x46, 0x05);   //MSRC_CONFIG_TIMEOUT_MACROP
	vl53l0x_I2C_SendData(slave_address, 0x40, 0x40);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x0E, 0x06);   //SYSTEM_THRESH_LOW
	vl53l0x_I2C_SendData(slave_address, 0x20, 0x1A);   //CROSSTALK_COMPENSATION_PEAK_RATE_MCPS
	vl53l0x_I2C_SendData(slave_address, 0x43, 0x40);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x34, 0x03);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x35, 0x44);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x01);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x31, 0x04);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x4B, 0x09);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x4C, 0x05);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x4D, 0x04);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x44, 0x00);   //FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT
	vl53l0x_I2C_SendData(slave_address, 0x45, 0x20);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x47, 0x08);   //FINAL_RANGE_CONFIG_VALID_PHASE_LOW
	vl53l0x_I2C_SendData(slave_address, 0x48, 0x28);   //FINAL_RANGE_CONFIG_VALID_PHASE_HIGH
	vl53l0x_I2C_SendData(slave_address, 0x67, 0x00);   //FINAL_RANGE_CONFIG_MIN_SNR
	vl53l0x_I2C_SendData(slave_address, 0x70, 0x04);   //FINAL_RANGE_CONFIG_VCSEL_PERIOD
	vl53l0x_I2C_SendData(slave_address, 0x71, 0x01);   //FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI
	vl53l0x_I2C_SendData(slave_address, 0x72, 0xFE);   //FINAL_RANGE_CONFIG_TIMEOUT_MACROP_LO
	vl53l0x_I2C_SendData(slave_address, 0x76, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x77, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x01);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x0D, 0x01);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x80, 0x01);   //POWER_MANAGEMENT_GO1_POWER_FORCE
	vl53l0x_I2C_SendData(slave_address, 0x01, 0xF8);   //SYSTEM_SEQUENCE_CONFIG
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x01);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x8E, 0x01);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x00, 0x01);   //SYSRANGE_START
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x80, 0x00);   //POWER_MANAGEMENT_GO1_POWER_FORCE
}


/*
 * Setup VL5310X to use specified timing budget (min 20000, max 200000) ~ affects accuracy
 */
void vl5310x_SetMeasurementTimingBudget(uint8_t slave_address, uint32_t targetTiming) {
	vl53l0x_I2C_ReceiveData(slave_address, 0x01);   //SYSTEM_SEQUENCE_CONFIG
	vl53l0x_I2C_ReceiveData(slave_address, 0x50);   //PRE_RANGE_CONFIG_VCSEL_PERIOD
	vl53l0x_I2C_ReceiveData(slave_address, 0x46);   //MSRC_CONFIG_TIMEOUT_MACROP
	vl53l0x_I2C_ReceiveData(slave_address, 0x51);   //PRE_RANGE_CONFIG_TIMEOUT_MACROP_HI	
	vl53l0x_I2C_ReceiveData(slave_address, 0x52);   //PRE_RANGE_CONFIG_TIMEOUT_MACROP_LO
	vl53l0x_I2C_ReceiveData(slave_address, 0x70);   //FINAL_RANGE_CONFIG_VCSEL_PERIOD
	vl53l0x_I2C_ReceiveData(slave_address, 0x71);   //FINAL_RANGE_CONFIG_TIMEOUT_MACROP_HI
	vl53l0x_I2C_ReceiveData(slave_address, 0x72);   //FINAL_RANGE_CONFIG_TIMEOUT_MACROP_LO
    
    //REST OF ALGORITHM
}


/*
 * Setup VL5310X to be in continuous reading mode
 */
void vl5310x_SetContinuousReading(uint8_t slave_address) {
	vl53l0x_I2C_SendData(slave_address, 0x80, 0x01);   //POWER_MANAGEMENT_GO1_POWER_FORCE
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x01);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x00, 0x00);   //SYSRANGE_START
	vl53l0x_I2C_SendData(slave_address, 0x09, 0x00);   //SYSTEM_RANGE_CONFIG 1
	vl53l0x_I2C_SendData(slave_address, 0x00, 0x01);   //SYSRANGE_START
	vl53l0x_I2C_SendData(slave_address, 0xFF, 0x00);   //UNKNOWN
	vl53l0x_I2C_SendData(slave_address, 0x80, 0x00);   //POWER_MANAGEMENT_GO1_POWER_FORCE
	vl53l0x_I2C_SendData(slave_address, 0x00, 0x02);   //SYSRANGE_START
}