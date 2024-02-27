#include "Serial_func.h"
#include <stdarg.h>

using namespace std;

uint8_t TXT_Start_Str[] = "CMD ";
uint8_t TXT_END_Str[] = " END";
int Data_Size = 8;
bool Rx_Flag = false;
bool WorkMode = false;

vector<uint8_t> Rx_data_vec(Data_Size, 0x00);
vector<uint8_t> StartStr_CheckVec(4, ' ');
vector<uint8_t> EndStr_CheckVec;
vector<uint8_t> Rx_Cmd_Vec;

enum class RxHexState{
	START,
	WORK,
	END
};
RxHexState curRxState = RxHexState::START;

enum class RxTxTState{
	START,
	RECEIVE
};
RxTxTState curTxTState = RxTxTState::START;

#pragma region Basic Printing
void Serial_Init(uint32_t baudrate){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, FunctionalState::ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, FunctionalState::ENABLE);
	GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
	GPIO_InitStruct -> GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_AF_OD;
	GPIO_InitStruct -> GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct -> GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, GPIO_InitStruct);

	GPIO_InitStruct -> GPIO_Pin  = GPIO_Pin_10;
	GPIO_InitStruct -> GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_IPU;
	GPIO_InitStruct -> GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, GPIO_InitStruct);

	USART_InitTypeDef* USART1_InitStruct = new USART_InitTypeDef();
	USART1_InitStruct -> USART_Mode = USART_Mode_Tx | USART_Mode_Rx ; // note if we use both send and receive, use "|" for init
	USART1_InitStruct -> USART_BaudRate = baudrate;  /*directly write a value to baud rate */
	USART1_InitStruct -> USART_HardwareFlowControl = USART_HardwareFlowControl_None; // not use flow control
	USART1_InitStruct -> USART_WordLength = USART_WordLength_8b;
	USART1_InitStruct -> USART_Parity = USART_Parity_No; // use this to set word verification (check)
	USART1_InitStruct -> USART_StopBits = USART_StopBits_1;
	USART_Init(USART1, USART1_InitStruct);

	// Interruption Configuration 
	USART_ITConfig(USART1, USART_IT_RXNE, FunctionalState::ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef* NVIC_InitStruct = new NVIC_InitTypeDef();
	NVIC_InitStruct -> NVIC_IRQChannel = USART1_IRQn; // USART1 Interruption 
	NVIC_InitStruct -> NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct -> NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStruct -> NVIC_IRQChannelCmd = FunctionalState::ENABLE;
	NVIC_Init(NVIC_InitStruct);
	
	USART_Cmd(USART1, FunctionalState::ENABLE);
}

void Serial_SendByte(uint16_t byte){
	USART_SendData(USART1, byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == FlagStatus::RESET);
	// wait for data sent -> if we send before data transfer from TDR to Shift Register
}

void Serial_SendString(string str){
	for (int i = 0; i < str.size(); i++){
		Serial_SendByte(str[i]);
	}
}

// send a integral
void Serial_SendNumber(int num){
	char *str;
	sprintf(str, "%d\0", num);
	Serial_SendString(str);
}

void Serial_Printf(const char* __format, ...){
	char ch[200];
	va_list arg; 
	va_start(arg, __format); // receive the parameter from format 
	vsprintf(ch, __format, arg);
	va_end(arg); // release parameter list 
	Serial_SendString(ch);
}
#pragma endregion

#pragma region Pack_Sending
void Serial_PackSendInit(bool IsTxTMode){
	WorkMode = IsTxTMode;
}

bool check_equal(uint8_t* A, vector<uint8_t> B){
	for (int i = 0; i < 4; i++) if (A[i]!=B[i]) return false;
	return true;
}

// handler is in md.s
extern "C"{
	static uint8_t index = 0;
	void USART1_IRQHandler(void){
		if (USART_GetITStatus(USART1, USART_IT_RXNE) == FlagStatus::SET){
			uint16_t byte = USART_ReceiveData(USART1);
			if (!WorkMode){
				switch (curRxState)
				{
				case RxHexState::START:
					if (byte == 0xFF) {
						curRxState = RxHexState::WORK;
						index = 0; // reset index
					}
					break;
				case RxHexState::WORK:
					Rx_data_vec[index] = byte;
					index++;
					if (index == Data_Size){
						curRxState = RxHexState::END;
					}
					break;
				case RxHexState::END:
					Rx_Flag = true;
					if (byte == 0xFE) curRxState = RxHexState::START; // wait for next byte;
					break;
				}	
			}else{
				switch (curTxTState)
				{
				case RxTxTState::START:
					StartStr_CheckVec.erase(StartStr_CheckVec.begin());  // deposite
					StartStr_CheckVec.push_back(byte);
					if (check_equal(TXT_Start_Str, StartStr_CheckVec)){
						curTxTState = RxTxTState::RECEIVE;
						EndStr_CheckVec.clear();
						Rx_Cmd_Vec.clear();
					}
					break;
				case RxTxTState::RECEIVE:
					if (EndStr_CheckVec.size() < 4){
						EndStr_CheckVec.push_back(byte);
					}else{
						Rx_Cmd_Vec.push_back(EndStr_CheckVec[0]);  // put this into
						EndStr_CheckVec.erase(EndStr_CheckVec.begin());
						EndStr_CheckVec.push_back(byte);
					}
					// check if should end
					if (EndStr_CheckVec.size() == 4 && check_equal(TXT_END_Str, EndStr_CheckVec)){
						curTxTState = RxTxTState::START;  // END
						Rx_Flag = true;
						StartStr_CheckVec = vector<uint8_t>(4, '*');
					}
				}
			}
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		}
	}
}

// dataPack must be length 8 for each datapack
void Serial_SendPack(vector<uint8_t> __pack__){
	Serial_SendByte(0xFF);
	for (int i = 0; i < 8; i++){
		Serial_SendByte(__pack__[i]);
	}
	Serial_SendByte(0xFE);
}

// send a variable command in the form of datapack, CMD as packhead, END as packedtail
void Serial_SendCommand(char* _cmd_){
	/* start sequence */
	Serial_SendString(TXT_Start_Str);
	/* data sequence */
	for (int i = 0; _cmd_[i]!= '\0'; i++){
		Serial_SendByte(_cmd_[i]);
	}
	Serial_SendString(TXT_END_Str);
	/* end sequence */
}
#pragma endregion
