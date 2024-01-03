#include "public.h"
#include "EEPROM_OP.h"
#include "I2C_operation.h"

sbit SCL = P2^1;    // Series Clock 
sbit SDA = P2^0;    // Series 

void I2C_Init(void){
    SCL = 1; 
    SDA = 1; 
}

// send Start signal 
void I2C_Start(void){ // SDA  from 1 to 0 when SCL = 1;
    // **must add SDA = 1, don't forget it.** since SCL = 1, it  will equal to a end signal 
    SDA = 1; // ** caution !!!! must place SDA at first, or it can't read correct message. 
    Delay10us(1); 
    // **note : for  start, ack and nack, return default as SCL = 0**    
    SCL = 1;
    Delay10us(1);
    SDA = 0; // set SDA = 0;
    Delay10us(1);
    SCL = 0; // occupy the I2C bus 
    Delay10us(1); 
}

// send Stop signal 
void I2C_Stop(void){
    SDA = 0; // ** must place before the SCL or the next time to write will be erroe 
    Delay10us(1);
    SCL = 1; // check default
    Delay10us(1);
    SDA = 1; 
    Delay10us(1); 
}

// send acknowledge;
void I2C_ACK(void){
    SCL = 0; // check default
    Delay10us(1);
    SDA = 0;
    Delay10us(1); // send a 0
    SCL = 1;
    Delay10us(1);
    SCL = 0;
    Delay10us(1);
}

// send disacknowledge
void I2C_NACK(void){
    SCL = 0;
    Delay10us(1);
    SDA = 1;
    Delay10us(1);
    SCL = 1;
    Delay10us(1);
    SCL = 0;
    Delay10us(1);
}

// Read Function of the acknowledge.
u8 I2C_Wait_Answer(void){ //**accept the answer, fai as 1 and not succeed as 0**
    u8 time_temp = 0;
    SCL = 1;  // firstly, set SCL to  high voltage level 
    Delay10us(1); // wait for answer 
    // wait for SDA change 
    while (SDA){ // if SDA 
        time_temp++;
        if (time_temp > 100){
            I2C_Stop();  // send stop signal
            return 1;    // SCL = 1;
        }
    }
    SCL = 0;  // allow receive data again;
    Delay10us(1);  
    return 0;
}

// write data by I2C 
void I2C_WriteByte(u8 _data_){
    u8 i, data_temp = 0;
    SCL = 0; // 
    Delay10us(1);
    UART_Debug("send the message: ");
    for (i = 0; i< 8; i++){
        data_temp = _data_ >> 7;  // wrong code : SDA = _data_ & 0x80; -> it will cause all data to set to 0
        if (data_temp == 1){
            UART_Debug("1");
        }else if (data_temp == 0){ 
            UART_Debug("0");
        }
        SDA = data_temp;          // set SDA as data 
        _data_ <<= 1;
        Delay10us(1); 
        SCL = 1;
        Delay10us(1);
        SCL = 0 ;
        Delay10us(1);
    }
    UART_Debug("\n");
}

// read one byte use I2C 
u8 I2C_ReadByte(u8 ack){ // if ack is 
    u8 _data_ = 0, i = 0; 
    UART_Debug("receive the message: ");
    for (i = 0; i < 8; i++){
        SCL = 0;      // use host to set SCL = 0; -> in this time slave will send data;
        Delay10us(1);
        SCL = 1;      // use host to set SCL = 1;

        _data_ <<= 1; // move first 
        if(SDA) { UART_Debug("1"); _data_++;}
        else{UART_Debug("0");}

        Delay10us(1);  // ****** important-> don't forget ***********
    }
    // check if the slave give the answer -> 
    if (ack){
        I2C_ACK();
    }else{
        I2C_NACK();
    }
    UART_Debug("\n");
    return _data_;
}

// write one byte to AT24C02 
void AT24C02_write_byte(u8 addr, u8 _data_){ 
    u8 ans = 0;
    I2C_Start();            // send start singnal 
    I2C_WriteByte(0xA0);    // send address 
    ans = I2C_Wait_Answer();// can be written
    // use ans = 0 as successful to provide reset answer
    I2C_WriteByte(addr);    // send the address to be written 
    ans = I2C_Wait_Answer();
    I2C_WriteByte(_data_);
    ans = I2C_Wait_Answer();
    I2C_Stop();
    Delay(10);
}

// read one byte from AT24C02;
u8 AT24C02_read_byte(u8 addr){
    u8 ans = 0, byte_temp = 0; 
    // first we write the address to read. 
    I2C_Start ();
    I2C_WriteByte (0xA0);  // for write address
    ans = I2C_Wait_Answer();
    I2C_WriteByte(addr); // write the addres of 
    ans = I2C_Wait_Answer(); 
    I2C_Start(); // reuse read mode 
    I2C_WriteByte (0xA1); // for read bytes
    ans = I2C_Wait_Answer();
    byte_temp = I2C_ReadByte(0);
    I2C_Stop();
    Delay(10);
    return byte_temp;
}