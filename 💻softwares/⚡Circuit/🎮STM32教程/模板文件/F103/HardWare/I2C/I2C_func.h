#ifndef I2C_FUNC_H
#define I2C_FUNC_H
#include "stm32f10x.h"

void I2C2_InitFunction(void);
void I2C2_Write(unsigned char slave_address, unsigned char reg_addr, unsigned char length, unsigned char* const Data);
void I2C2_WriteByte(unsigned char slave_address, unsigned char reg_addr, unsigned const Data);
void I2C2_Read(unsigned char slave_address, unsigned char reg_addr, unsigned char length, unsigned char *Data);
unsigned char I2C2_ReadByte(unsigned char slave_address, unsigned char reg_addr);

extern bool I2C2_ErrorFlag;   // I2C2 Error Flag, If any wait error it will be set to 1
extern unsigned int I2C2_time_out;

#endif // !I2C_FUNC_H

