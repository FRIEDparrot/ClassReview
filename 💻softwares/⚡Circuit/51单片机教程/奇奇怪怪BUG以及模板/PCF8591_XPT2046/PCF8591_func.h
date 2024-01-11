#ifndef PCF8591_FUNC
#define PCF8591_FUNC
#include "I2C_P34.h"
#include <INTRINS.H>

void XPT2046_Init(void);
void PCF8591_Init(void);

float Get_Voltage_XPT2046(u8 port_msg);
float Get_Voltage_PCF8591(u8 port_msg);

#endif