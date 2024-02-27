#ifndef I2C_FUNC_GENERAL_H
#include "stm32f10x.h"

extern unsigned int I2C_GEN_time_out; // reserve value

void I2C_GEN_InitFunction();
void I2C_GEN_WriteByte(unsigned char slave_address, unsigned char reg_addr, unsigned const Data);
void I2C_GEN_Write(unsigned char slave_address,unsigned char reg_addr, unsigned char length, unsigned char const *Data);
unsigned char I2C_GEN_ReadByte(unsigned char slave_address, unsigned char reg_addr);
void I2C_GEN_Read(unsigned char slave_address, unsigned char reg_addr, unsigned char length, unsigned char *Data);

extern bool I2C_GEN_ErrorFlag;

#endif // !I2C_FUNC_GENERAL_H
