#include "I2C_P34.h"

void I2C_Start(void){
    DIN = 1; // add it first 
    _nop_();
    DCLK = 1;
    _nop_();
    DIN = 0;
    _nop_();
    DCLK = 0;
    _nop_();

}

void I2C_Stop(void){
    DIN = 0; // 
    _nop_();
    DCLK = 1;
    _nop_();
    DIN = 1; // stop the message 
    _nop_();
}

// if get answer it will return 0;
void I2C_SendByte(u8 msg){
    u8 i, ack = 0;
    DIN = 0; // start send data 
    _nop_();
    for (i = 0; i < 8;  i++){
        if ((msg & 0x80 )> 0) { // **CAUTION 
            DIN = 1;
        }
        else {
            DIN = 0;
        }
        msg <<= 1;
        _nop_();
        DCLK = 1;
        _nop_();
        DCLK = 0; // reset to 0 
        _nop_();
    }
}

void I2C_SendAck(void){
    DCLK = 0; // reset DCLK 
    _nop_();

    DIN = 0;
    _nop_(); 
    DCLK = 1;
    _nop_();
    DCLK = 0;
    _nop_();
    DIN = 1; // re-pull-up the bus 
    _nop_();
}

void I2C_SendNack(void){
    DCLK = 0; // Reset DCLK 
    _nop_();

    DIN = 1;
    _nop_();
    DCLK = 1;
    _nop_();
    DCLK = 0;
    _nop_();
}

u8 I2C_Wait_Answer(void){
    u8 time_temp = 0;
    DIN = 1;
    _nop_();
    while (DIN)
    {
        time_temp++;
        if (time_temp >100){
            I2C_Stop();
            return 1;
        }
    }
    // clock pulse for acknowledgement 
    DCLK = 1;
    _nop_();
    DCLK = 0;
    _nop_();
    return 0;
}

u8 I2C_ReadByte(u8 ack){
    u8 dat, i = 0; // at first DIN = 1;
    DIN = 1;
    _nop_();
    for (i = 0; i < 8; i++){
        dat <<= 1;
        if (DIN) dat++;
        DCLK = 1;
        _nop_();
        DCLK = 0;
        _nop_();
    }
    if (ack){
        I2C_SendAck();
    }else{
        I2C_SendNack();
    }
    _nop_();
    return dat;
}