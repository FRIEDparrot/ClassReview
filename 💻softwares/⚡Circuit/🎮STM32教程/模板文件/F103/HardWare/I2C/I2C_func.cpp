#include "I2C_func.h"

unsigned int I2C2_time_out= 10000;
bool I2C2_ErrorFlag = false;

/// @brief wait for a event occured, 
/// @param I2C_EVENT 
/// @param timeout can be set about to 10000 
/// @return if event occurs, return 0, or timeout return 1;
void I2C2_WaitEvent(unsigned int I2C_EVENT, unsigned int timeout)
{
    uint16_t temp = timeout;
	if (temp == 0) return;
    while (I2C_CheckEvent(I2C2, I2C_EVENT) != SUCCESS){
        temp--;
        if (temp == 0){
            I2C2_ErrorFlag = true;
            return;
        }
    }
}

/// @brief general I2C write function, use length to sepcify the length of data to write
/// @param slave_address slave device address 
/// @param reg_addr address of register to start to write 
/// @param legnth length of data to write 
/// @param Data   Data Array 
void I2C2_Write(unsigned char slave_address, unsigned char reg_addr, unsigned char length, unsigned char* const Data){
    I2C_GenerateSTOP(I2C2, FunctionalState::ENABLE);
    I2C_GenerateSTART(I2C2, FunctionalState::ENABLE);
    I2C2_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT, I2C2_time_out); // EV5
    
    I2C_Send7bitAddress(I2C2, slave_address, I2C_Direction_Transmitter);
    I2C2_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, I2C2_time_out);  // EV6 
    I2C_SendData(I2C2, reg_addr);
    I2C2_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING, I2C2_time_out); // EV8
    for (int i = 0; i< length; i++){
        I2C_SendData(I2C2,  Data[i]);
        I2C2_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING, I2C2_time_out); // EV8
    }
    I2C2_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED,I2C2_time_out);  // EV 8_2;
    I2C_GenerateSTOP(I2C2, FunctionalState::ENABLE);
}

/// @brief Write one byte to the given address of the slave device
/// @param slave_address slave device address 
/// @param reg_addr 
/// @param Data 
void I2C2_WriteByte(unsigned char slave_address, unsigned char reg_addr, unsigned const Data){
    I2C_GenerateSTOP(I2C2, FunctionalState::ENABLE);     // Generate Stop First
    I2C_GenerateSTART(I2C2, FunctionalState::ENABLE);    // Generate Start Signal
    I2C2_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT, I2C2_time_out); // EV5

    I2C_Send7bitAddress(I2C2, slave_address, I2C_Direction_Transmitter);  // 1001000 + 0(Write Bit)
    I2C2_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED,I2C2_time_out);  // EV6 
    
    I2C_SendData(I2C2, reg_addr);
    I2C2_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING, I2C2_time_out); // EV8
    
    I2C_SendData(I2C2, Data);
    I2C2_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED, I2C2_time_out); // EV8_1
    I2C_GenerateSTOP(I2C2, FunctionalState::ENABLE);
}

/// @brief Read One Byte from the slave address
/// @param slave_address 
/// @param reg_addr 
/// @param Data 
unsigned char I2C2_ReadByte(unsigned char slave_address, unsigned char reg_addr){
    unsigned char value = 0x00;
    // write register address
    I2C_GenerateSTOP(I2C2, FunctionalState::ENABLE);
    I2C_GenerateSTART(I2C2, FunctionalState::ENABLE);
    I2C2_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT, I2C2_time_out);                // EV5 

    I2C_Send7bitAddress(I2C2, slave_address, I2C_Direction_Transmitter);
    I2C2_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, I2C2_time_out);  // EV6
    
    I2C_SendData(I2C2, reg_addr);
    I2C2_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED, I2C2_time_out);           // EV8_2
    
    // regenerate start condition and receive data 
    I2C_GenerateSTART(I2C2, FunctionalState::ENABLE);
    I2C2_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT, I2C2_time_out);                // EV5 
    I2C_Send7bitAddress(I2C2, slave_address, I2C_Direction_Receiver);
    I2C2_WaitEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED, I2C2_time_out);     // EV6

    // disable data transfer
    I2C_AcknowledgeConfig(I2C2, FunctionalState::DISABLE);
    I2C2_WaitEvent(I2C_EVENT_MASTER_BYTE_RECEIVED, I2C2_time_out);              // EV7
    
    value = I2C_ReceiveData(I2C2);
    I2C_GenerateSTOP(I2C2, FunctionalState::ENABLE);
    I2C_AcknowledgeConfig(I2C2, FunctionalState::ENABLE);  // dont't forget reset the acknowledge config bit
    return value;
}


/// @brief use I2C2 bus to read the data needed to Data Array;
/// @param slave_address slave device address 
/// @param reg_addr register address 
/// @param length length of data, must be >= 1
/// @param Data Data Array @attention you should have a big enough array to storge the data to read 
/// @note  this function would firstly locate the register probe to specified location and then receive data
void I2C2_Read(unsigned char slave_address, unsigned char reg_addr, unsigned char length, unsigned char *Data){
	I2C_GenerateSTOP(I2C2, FunctionalState::ENABLE);
    I2C_GenerateSTART(I2C2, FunctionalState::ENABLE);
    I2C2_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT, I2C2_time_out);              // EV5

    I2C_Send7bitAddress(I2C2, slave_address, I2C_Direction_Transmitter);
    I2C2_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, I2C2_time_out);// EV6

    I2C_SendData(I2C2, reg_addr);
    I2C2_WaitEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED, I2C2_time_out);         // EV8_2

    // regenerate start condition
    I2C_GenerateSTART(I2C2, FunctionalState::ENABLE);
    I2C2_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT, I2C2_time_out);              // EV5
    I2C_Send7bitAddress(I2C2, slave_address, I2C_Direction_Receiver);
    I2C2_WaitEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED, I2C2_time_out);   // EV6

    for (int i = 0; i < length -1; i++){
        I2C2_WaitEvent(I2C_EVENT_MASTER_BYTE_RECEIVED, I2C2_time_out);        // EV7
        Data[i] = I2C_ReceiveData(I2C2);
    }
    I2C_AcknowledgeConfig(I2C2, FunctionalState::DISABLE); // disable the acknowedgement
    
    I2C2_WaitEvent(I2C_EVENT_MASTER_BYTE_RECEIVED, I2C2_time_out);            // EV7
    Data[length - 1] = I2C_ReceiveData(I2C2);

    I2C_GenerateSTOP(I2C2, FunctionalState::ENABLE);
    I2C_AcknowledgeConfig(I2C2, FunctionalState::ENABLE); // re-enable the acknowledgement
}

/// @brief I2C2 Init Function, must be called before using I2C2
void I2C2_InitFunction(void){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, FunctionalState::ENABLE);  // start the I2C2 Clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, FunctionalState::ENABLE);

    GPIO_InitTypeDef* GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_AF_OD;  // use I2C port settings 
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct->GPIO_Speed = GPIOSpeed_TypeDef::GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, GPIO_InitStruct);

    I2C_InitTypeDef* I2C_InitStruct = new I2C_InitTypeDef();
    I2C_InitStruct->I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct->I2C_Ack = I2C_Ack_Enable;  // Enable Automatically acknowledge
    I2C_InitStruct->I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct->I2C_ClockSpeed = 100000;  // 50kHz, if use 400000 is 400kHz
    I2C_InitStruct->I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct->I2C_OwnAddress1 = 0xCF;
    I2C_Init(I2C2, I2C_InitStruct);
    I2C_Cmd(I2C2, FunctionalState::ENABLE);
}
