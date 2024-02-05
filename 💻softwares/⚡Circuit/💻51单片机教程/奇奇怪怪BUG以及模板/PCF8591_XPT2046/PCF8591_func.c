#include "PCF8591_func.h"

void XPT2046_Init(void){
    CS = 1;
    DCLK = 1;
    DIN = 1;  // note P3^4 have no pulling resistance and the default should be high.
    _nop_();
}

u8 data_res[2] = {0x00, 0x00};
// use message for data 
void XPT2046_Read_Data(u8 msg){
    u8 i;
    DIN = 0;
    _nop_(); 
    DCLK = 0;
    _nop_();
    // start send byte 
    CS = 0;  // enable  CS
    _nop_();
    for (i = 0; i < 8; i++){
        DIN = msg >> 7 ;
        _nop_();
        DCLK = 1;
        msg <<= 1; // send byte;
        _nop_(); 
        DCLK = 0;
        _nop_();
    }
    DIN = 0;
    _nop_();
    _nop_();
    DCLK = 1;
    _nop_();
    for (i = 0; i < 8; i++){
        data_res[0] <<=1;
        DCLK = 0; // corresponding to SPI time sequence, read at  
        _nop_();
        if (DOUT) data_res[0]++;
        DCLK = 1; // create rising edge 
        _nop_();
    }
    for (i = 0; i<8; i++){
        data_res[1] <<= 1;
        DCLK = 0;
        _nop_();
        if (DOUT) data_res[1]++;
        DCLK = 1;
        _nop_();
    }
    DCLK = 0;
    _nop_();
    CS = 1;   // disable
    _nop_();
}

// example: 0x94 use XP as AIN0, send 1001 0100
float Get_Voltage_XPT2046(u8 port_msg){
    int res = 0;
    float Voltage_res = 0;
    XPT2046_Read_Data(port_msg);
    res = (int)data_res[0] * 16 + (int)data_res[1] /16;
    Voltage_res = (float)res / 4096 * 5; // reset the voltage level
    return Voltage_res;
}

void PCF8591_Init(void){ // set all as high voltage level
    DIN = 1;
    _nop_(); 
    DCLK = 1;
    _nop_();
}

u8 PCF8591_Read_Data(u8 msg){
    u8 ans = 0, res = 0, result = 0;
    I2C_Start();
    I2C_SendByte(msg);  // such as 0x90 address byte 1001 0000  
    ans = I2C_Wait_Answer();
    // 0100 0100 as I2C bus protocol
    I2C_SendByte(0x04); // Send Control Byte -> 8 digits
    ans = I2C_Wait_Answer();
    I2C_Start();
    I2C_SendByte(0x91);  // Begin read byte;
    ans = I2C_Wait_Answer();
    result = I2C_ReadByte(0);
    I2C_Stop();
    return result;
}

// example: 0x90 use XP as AIN0, 1001 0000
float Get_Voltage_PCF8591(u8 port_msg){
    int res = 0; float Voltage_res = 0; 
    res = (int) PCF8591_Read_Data(port_msg);
    Voltage_res = (float)res / 256 * 5;
    return Voltage_res;
}