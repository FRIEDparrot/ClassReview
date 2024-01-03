#ifndef I2C_OPERATION
#define I2C_OPERATION

#include "public.h"

void I2C_Init(void); 
void I2C_Start(void);
void I2C_Stop(void);
void I2C_ACK(void);
void I2C_NACK(void);
u8 I2C_Wait_Answer(void);
void I2C_WriteByte(u8 _data_);
u8 I2C_ReadByte(u8 ack);

void AT24C02_write_byte(u8 addr, u8 _data_);
u8 AT24C02_read_byte(u8 addr);

#endif 