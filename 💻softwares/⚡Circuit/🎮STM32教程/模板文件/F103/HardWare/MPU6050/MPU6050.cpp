#include "MPU6050.h"
#include "I2C_func_general.h"

struct MPU60x0_REGISTER *MPU6050_Register;

/// @brief write a byte to addr 
/// @param addr should choose from MPU6050_Register
/// @param value value to write to it
void MPU6050_WriteRegister(uint8_t addr, uint8_t value){
    I2C_GEN_WriteByte(MPU6050_Address, addr, value);
}

/// @brief read a single byte at the specified address
/// @param addr address to be read 
uint8_t MPU6050_ReadRegister(uint8_t addr){
    return I2C_GEN_ReadByte(MPU6050_Address, addr);
}

/* also can be substitute with: 
I2C2_InitFunction();
    MPU6050_Register = new MPU60x0_REGISTER();
    uint8_t datas2[] = {0x01, 0x00};
    uint8_t datas[] = {0x09, 0x06, 0x08, 0x08};
    I2C2_Write(MPU6050_Address, MPU6050_Register->PWR_MGMT_1, 2, datas2);
    I2C2_Write(MPU6050_Address, MPU6050_Register->SMPLRT_DIV, 4, datas);
*/

void MPU6050_GetData(int16_t &accx, int16_t &accy, int16_t &accz, int16_t &gyrox, int16_t &gyroy, int16_t &gyroz){
    uint8_t MPU6050Data1[6];
    uint8_t MPU6050Data2[6];
    I2C_GEN_Read(MPU6050_Address, MPU6050_Register->ACCEL_XOUT_H, 6, MPU6050Data1);
    I2C_GEN_Read(MPU6050_Address, MPU6050_Register->GYRO_XOUT_H,  6, MPU6050Data2);
    accx = ((uint16_t)MPU6050Data1[0] << 8 | MPU6050Data1[1]);
    accy = ((uint16_t)MPU6050Data1[2] << 8 | MPU6050Data1[3]);
    accz = ((uint16_t)MPU6050Data1[4] << 8 | MPU6050Data1[5]);
    gyrox = ((uint16_t)MPU6050Data2[0] << 8 | MPU6050Data2[1]);
    gyroy = ((uint16_t)MPU6050Data2[2] << 8 | MPU6050Data2[3]);
    gyroz = ((uint16_t)MPU6050Data2[4] << 8 | MPU6050Data2[5]);
}

void MPU6050_Init(){
    I2C_GEN_InitFunction();  // InitI2C;
    MPU6050_Register = new MPU60x0_REGISTER();  // ****** importat ******** 
    MPU6050_WriteRegister(MPU6050_Register->PWR_MGMT_1,0x01);    // disble sleep, use X oscialltor clock for improved stability
    MPU6050_WriteRegister(MPU6050_Register->PWR_MGMT_2, 0x00);   // 

    MPU6050_WriteRegister(MPU6050_Register->SMPLRT_DIV, 0x09);   // sample rate is 1kHz==accelerometer out put rate
    MPU6050_WriteRegister(MPU6050_Register->CONFIG, 0x06);       // 8kHz Fs, FSYNC disabled 
    MPU6050_WriteRegister(MPU6050_Register->GYRO_CONFIG, 0x00);  // not use self-test, 250 degree/s range
    MPU6050_WriteRegister(MPU6050_Register->ACCEL_CONFIG, 0x18); // not use self-test, not use HPF, +-16g range
}

