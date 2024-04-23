#include "bluetooth.h"
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <vector>
#include <stdlib.h>

/* Circuit: 
PA5 -> STATE,  PA6 -> EN 
B10 -> RXD B11 ->  TXD 
*/
using namespace std;

bool BLUETOOTH_RxFlag = false;

vector<uint16_t> BLUETOOTH_DataBuffer;

/// @brief BLUETOOTH GPIO Init Function, used for test bluetooth connection state;
///        called before any functions
void BLUETOOTH_GPIO_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
    GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, GPIO_InitStruct);
}

/// @brief BLUETOOTH USART Init Function, called before any functions
/// @param baudrate 
void BLUETOOTH_USART_Init(uint32_t baudrate){
    // GPIO_Init (Init as GPIO Push-pull mode)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, FunctionalState::ENABLE);

    // Init USART3 TXD 
    GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_AF_OD;      // out Open Drain output
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_10;    
    GPIO_InitStruct->GPIO_Speed =  GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, GPIO_InitStruct);

    // Init USART3 RXD
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_IPU;
    GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, GPIO_InitStruct);

    // Init USART3 and USART3 Clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, FunctionalState::ENABLE);
    USART_InitTypeDef* USART_InitStruct = new USART_InitTypeDef();
    USART_StructInit(USART_InitStruct);
    USART_InitStruct->USART_BaudRate = baudrate;
    USART_InitStruct->USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  // Receive | 
    USART_InitStruct->USART_WordLength = USART_WordLength_8b;
    USART_InitStruct->USART_Parity = USART_Parity_No;
    USART_InitStruct->USART_StopBits = USART_StopBits_1;
    USART_InitStruct->USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3, USART_InitStruct);
    
    // USART_DMACmd(USART3, USART_DMAReq_Tx | USART_DMAReq_Rx, FunctionalState::ENABLE);

    // Init the USART Interruption
    USART_ITConfig(USART3, USART_IT_RXNE, FunctionalState::ENABLE);  // enable the interruption when receive bits
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef* NVIC_InitStruct = new NVIC_InitTypeDef();
    NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct->NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStruct->NVIC_IRQChannelCmd = FunctionalState::ENABLE;
    NVIC_InitStruct->NVIC_IRQChannel = USART3_IRQn;
    NVIC_Init(NVIC_InitStruct);

    USART_Cmd(USART3, FunctionalState::ENABLE);
}

/// @brief Send a byte
/// @param byte 
void BLUETOOTH_SendByte(uint16_t byte){
    USART_SendData(USART3, byte);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE)!=FlagStatus::SET);
}

/// @brief send string
/// @param str 
void BLUETOOTH_SendString(string str){
	for (int i = 0; i < str.length(); i++){
		BLUETOOTH_SendByte(str[i]);
	}
}

/// @brief Printf to Bluetooth
/// @param __format 
/// @param  
void BLUETOOTH_Printf(const char* __format, ...){
	char ch[200];
	va_list arg; 
	va_start(arg, __format); // receive the parameter from format 
	vsprintf(ch, __format, arg);
	va_end(arg); // release parameter list 
	BLUETOOTH_SendString(ch);
}

/// @brief get the bluetooth data by send an array to it
/// @param data_arr 
/// @return length of data_arr returned
/// example: uint16_t l = BLUETOOTH_GetData(&data_arr);
uint16_t BLUETOOTH_GetData(uint16_t* &data_arr){
    // note: if called
    int16_t num = BLUETOOTH_DataBuffer.size();
    // @caution: if malloc in function, the p would be released
    free(data_arr);   // if 
    data_arr = (uint16_t*)malloc(sizeof(uint16_t) * num);
    
    for (int i = 0; i < num; i++){
        data_arr[i] = BLUETOOTH_DataBuffer[i];
    }
    BLUETOOTH_DataBuffer.clear();
    BLUETOOTH_RxFlag = false;
    return num;
}

/// interruption function 
extern "C"{
    void USART3_IRQHandler(void){
        /*don't forget use = SET */
        if (USART_GetITStatus(USART3, USART_IT_RXNE) == FlagStatus::SET) {
            uint16_t data_temp = USART_ReceiveData(USART3);
            BLUETOOTH_DataBuffer.push_back(data_temp);
            BLUETOOTH_RxFlag = true;
        }
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}
