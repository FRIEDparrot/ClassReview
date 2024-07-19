#ifndef MPU6050_H
#define MPU6050_H
#include "stm32f10x.h"

using namespace std;

#define MPU6050_Address 0xD0 
#define MPU6050_AcroMeter_Range (16*9.8)
#define MPU6050_Gyroscope_Range 250

struct MPU60x0_REGISTER{
    public:
        uint8_t SMPLRT_DIV  = 0x19;   // 0x19
        uint8_t CONFIG      = 0x1A;   // 0x1A
        uint8_t GYRO_CONFIG = 0x1B;   // 0x1 
        uint8_t ACCEL_CONFIG= 0x1C;   // 0x1c
        uint8_t ACCEL_XOUT_H= 0x3B;   // 0x3B
        uint8_t ACCEL_XOUT_L= 0x3C;   // 0x3C
        uint8_t ACCEL_YOUT_H= 0x3D;   // 0x3D
        uint8_t	ACCEL_YOUT_L= 0x3E;   // 0x3E
        uint8_t	ACCEL_ZOUT_H= 0x3F;   // 0x3F
        uint8_t	ACCEL_ZOUT_L= 0x40;   // 0x40
        uint8_t	TEMP_OUT_H	= 0x41;   // 0x41
        uint8_t	TEMP_OUT_L	= 0x42;   // 0x42
        uint8_t	GYRO_XOUT_H	= 0x43;   // 0x43
        uint8_t	GYRO_XOUT_L	= 0x44;   // 0x44
        uint8_t	GYRO_YOUT_H	= 0x45;   // 0x45
        uint8_t	GYRO_YOUT_L	= 0x46;   // 0x46
        uint8_t	GYRO_ZOUT_H	= 0x47;   // 0x47
        uint8_t	GYRO_ZOUT_L	= 0x48;   // 0x48
        uint8_t PWR_MGMT_1  = 0x6B;   // 0x6B
        uint8_t PWR_MGMT_2  = 0x6C;   // 0x6C
        uint8_t WHO_AM_I    = 0x75;   // 0x75
        uint8_t	SlaveAddress= 0xD0;   // 0xD0
};  // don't forget use new to initialize 

extern MPU60x0_REGISTER *MPU6050_Register; // MPU6050_Rgister Object 

// write one byte to specific register
void MPU6050_Init();
uint8_t MPU6050_ReadRegister(uint8_t addr);
void MPU6050_WriteRegister(uint8_t addr, uint8_t value);
void MPU6050_GetData(int16_t &accx, int16_t &accy, int16_t &accz, int16_t &gyrox, int16_t &gyroy, int16_t &gyroz);

#endif // !MPU6050_H 
