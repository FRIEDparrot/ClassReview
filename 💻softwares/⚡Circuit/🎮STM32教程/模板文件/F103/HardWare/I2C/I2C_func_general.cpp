#include "I2C_func_general.h"
#include "Delay.h"

unsigned int I2C_GEN_time_out = 0;
bool I2C_GEN_ErrorFlag = false;

void I2C_GEN_W_SCL(unsigned char value){
    GPIO_WriteBit(GPIOB, GPIO_Pin_10,(BitAction) value);
    Delay_us(5);
}

void I2C_GEN_W_SDA(unsigned char value){
    GPIO_WriteBit(GPIOB, GPIO_Pin_11,(BitAction) value);
    Delay_us(5);
}
unsigned char I2C_GEN_R_SDA(void){
    uint8_t BitValue;
    Delay_us(5);
    BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    return BitValue;
}

/// @brief Init Function, must be called before use 
void I2C_GEN_InitFunction(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, FunctionalState::ENABLE);
    GPIO_InitTypeDef *GPIO_InitStruct = new GPIO_InitTypeDef();
    GPIO_InitStruct->GPIO_Mode = GPIOMode_TypeDef::GPIO_Mode_Out_OD; // must be out
    GPIO_InitStruct->GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, GPIO_InitStruct);
    // initialize it as high voltage level
    I2C_GEN_W_SCL(1);
    I2C_GEN_W_SDA(1);
}

/*********************** I2C functions ************************/
/// @brief start I2C function 
void I2C_GEN_Start(void){
    I2C_GEN_W_SDA(1);  // release SCL -> must 
    I2C_GEN_W_SCL(1);  // release SCL 
    I2C_GEN_W_SDA(0);
}

/// @brief stop I2C function
void I2C_GEN_Stop(void){
    I2C_GEN_W_SDA(0);
    I2C_GEN_W_SCL(1);
    I2C_GEN_W_SDA(1);
}

/// @brief send a byte time sequence
/// @param Data 
void I2C_GEN_SendByte(uint8_t Data){
    I2C_GEN_W_SCL(0);
    for (uint8_t i = 0; i < 8; i++){
        I2C_GEN_W_SDA(Data & 0x80 >> i);
        I2C_GEN_W_SCL(1);
        I2C_GEN_W_SCL(0);
    }
}

/// @brief read a byte time sequence
/// @return 
unsigned char I2C_GEN_ReceiveByte(){
    unsigned char Byte = 0x00;
    I2C_GEN_W_SDA(1);  // pull the I2C to high voltage level;
    for (uint8_t i = 0; i < 8; i++){
        I2C_GEN_W_SCL(1);
        if (I2C_GEN_R_SDA()) Byte |= (0x80 >> i);
        I2C_GEN_W_SCL(0);
    }
    return Byte;
}

/// @brief  wait for acknowledge of slave device
/// @return 
unsigned char I2C_GEN_WaitAck(){
    unsigned char Ack;
    I2C_GEN_W_SDA(1);  // firsly we pull up the line
    I2C_GEN_W_SCL(1);  // 
    Ack = I2C_GEN_R_SDA();
    I2C_GEN_W_SCL(0);
    if (Ack == 1) I2C_GEN_ErrorFlag = true;
    return Ack;
}

/// @brief send acknowledge to slave device 
/// @param send ack if use 0, nack if use 1
void I2C_GEN_SendAck(unsigned char ack){
    I2C_GEN_W_SDA(ack);
    // give a clock
    I2C_GEN_W_SCL(1);
    I2C_GEN_W_SCL(0);
    I2C_GEN_W_SDA(1);  // recover data transfer
}

/// @brief Write a byte to specified Register 
/// @param slave_address 
/// @param reg_addr 
/// @param Data 
void I2C_GEN_WriteByte(unsigned char slave_address, unsigned char reg_addr, unsigned const Data){
    I2C_GEN_Start();
    I2C_GEN_SendByte(slave_address & 0xFE);  // 0xD0 | 0x00,
    I2C_GEN_WaitAck();
    I2C_GEN_SendByte(reg_addr);
    I2C_GEN_WaitAck();
    I2C_GEN_SendByte(Data);
    I2C_GEN_WaitAck();
    I2C_GEN_Stop();
}

/// @brief send data to slave device
/// @param slave_address 
/// @param reg_addr 
/// @param length 
/// @param Data must be an array not shorter than length
void I2C_GEN_Write(unsigned char slave_address,unsigned char reg_addr, unsigned char length, unsigned char const *Data){
    I2C_GEN_Start();
    I2C_GEN_SendByte(slave_address & 0xFE);  // 0xD0 | 0x00
    I2C_GEN_WaitAck();
    I2C_GEN_SendByte(reg_addr);
    I2C_GEN_WaitAck();
    for (int i = 0; i < length; i++){
        I2C_GEN_SendByte(Data[i]);
        I2C_GEN_WaitAck();
    }
    I2C_GEN_Stop();
}

/// @brief read a byte from the specified Register of slave device
/// @param slave_address 
/// @param reg_addr 
/// @return the byte readed
unsigned char I2C_GEN_ReadByte(unsigned char slave_address, unsigned char reg_addr){
    I2C_GEN_Start();
    I2C_GEN_SendByte(slave_address & 0xFE);
    I2C_GEN_WaitAck();
    I2C_GEN_SendByte(reg_addr);
    I2C_GEN_WaitAck();

    I2C_GEN_Start();
    I2C_GEN_SendByte(slave_address | 0x01);
    I2C_GEN_WaitAck();
    unsigned char value  = I2C_GEN_ReceiveByte();
    I2C_GEN_SendAck(1);  // nack
    I2C_GEN_Stop();
    return value;
}

/// @brief general I2C write function, use length to sepcify the length of data to write
/// @param slave_address slave device address 
/// @param reg_addr address of register to start to write 
/// @param length length of data to write
/// @param Data Data Array (must be of length specified)
void I2C_GEN_Read(unsigned char slave_address, unsigned char reg_addr, unsigned char length, unsigned char *Data){
    I2C_GEN_Start();
    I2C_GEN_SendByte(slave_address & 0xFE);
    I2C_GEN_WaitAck();
    I2C_GEN_SendByte(reg_addr);
    I2C_GEN_WaitAck();
    // regenerate start
    I2C_GEN_Start();
    I2C_GEN_SendByte(slave_address | 0x01);
    I2C_GEN_WaitAck();
    for (int i = 0; i < length - 1; i++){
        Data[i] = I2C_GEN_ReceiveByte();
        I2C_GEN_SendAck(0);
    }
    Data[length - 1] = I2C_GEN_ReceiveByte();
    I2C_GEN_SendAck(1);  // nack
    I2C_GEN_Stop();
}