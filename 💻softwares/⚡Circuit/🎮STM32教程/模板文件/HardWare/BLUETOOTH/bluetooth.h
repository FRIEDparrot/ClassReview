#ifndef __BLUETOOTH_H

#include "stm32f10x.h"

/* judge if RxFlag is true during main loop to see if any byte received */
extern bool BLUETOOTH_RxFlag;

/* Used for check if BLUETOOTH is Connected */
#define BLUETOOTH_STATE GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)

void BLUETOOTH_GPIO_Init(void);  
void BLUETOOTH_USART_Init(uint32_t baudrate);
void BLUETOOTH_SendByte(uint16_t byte);
void BLUETOOTH_Printf(const char* __format, ...);

uint16_t BLUETOOTH_GetData(uint16_t* &data_arr);

#endif // !__BLUETOOTH_H
