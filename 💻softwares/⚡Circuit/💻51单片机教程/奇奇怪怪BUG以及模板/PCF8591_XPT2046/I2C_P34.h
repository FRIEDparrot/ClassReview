#ifndef I2C_P34_H
#define I2C_P34_H
#include <REGX52.H>
#include "INTRINS.H"
#include "public.h"

// A/D Changer to view the different voltage level using I2C and SPI time sequence 
sbit DIN  = P3^4;  // IN port -> also act as SDA for PCF8591;
sbit DCLK = P3^6;  // clock input -> also act as SCL for PCF8591;
sbit CS   = P3^5;  // enable port ;
sbit DOUT = P3^7;  // OUT port

void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(u8 msg);
void I2C_SendAck(void);
u8 I2C_Wait_Answer(void);
u8 I2C_ReadByte(u8 ack);

#endif 