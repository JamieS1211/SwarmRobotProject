# 1 "vL53L0X.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.00\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "vL53L0X.c" 2








# 1 "./i2C.h" 1
# 17 "./i2C.h"
void i2C_Setup(void);






void i2C_Wait(void);






void i2C_Start(void);






void i2C_RepeatedStart(void);






void i2C_Stop(void);
# 54 "./i2C.h"
void i2C_SendData(unsigned char i2C_address, unsigned char bytes[], unsigned char numberOfBytes);
# 63 "./i2C.h"
void i2C_ReceiveData(unsigned char i2C_address, unsigned char sendBytes[], unsigned char numberOfSendBytes, unsigned char *recievedBytes, unsigned char numberOfReceivedBytes);
# 9 "vL53L0X.c" 2






void vl53l0x_i2C_SendData(unsigned char slave_address, unsigned char device_register, unsigned char value) {
    unsigned char data[2] = {device_register, value};
    i2C_SendData(slave_address, data, 2);
}





unsigned char vl53l0x_I2C_ReceiveData(unsigned char slave_address, unsigned char device_register) {
    unsigned char sendData[1] = {device_register};
    unsigned char recievedData[1] = {0x00};
    i2C_ReceiveData(slave_address, sendData, 1, &recievedData, 1);

    return recievedData[0];
}





void vl5310x_Setup(void) {

}
