#ifndef _SERIAL_FUNC_H
#define _SERIAL_FUNC_H
#include "stm32f10x.h"
#include <vector>
#include <queue>
#include <string>

using namespace std;

void Serial_Init(uint32_t baudrate);
void Serial_SendByte(uint16_t byte); 

template <typename ItemType>
void Serial_SendArr(vector<ItemType> arr){
	for (int i = 0; i < arr.size();i++){
		Serial_SendByte(arr[i]);
	}
}

void Serial_SendString(string s);
template <typename ItemType>
void Serial_SendString(ItemType* str){
	for (int i = 0; str[i]!='\0'; i++){
		Serial_SendByte(str[i]);
	}
}
void Serial_SendNumber(int num);
void Serial_Printf(const char* __format, ...);

void Serial_PackSendInit(bool IsTxTMode);
void Serial_SendPack(vector<uint8_t> __pack__);
void Serial_SendCommand(char* _cmd_);
extern vector<uint8_t> Rx_data_vec;
extern vector<uint8_t> Rx_Cmd_Vec;
extern bool Rx_Flag;

#endif // !SERIAL_FUNC_
