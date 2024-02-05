#include "HC_RSO4_OP.h"

u16 t0 = 0;
u8  t0_add = 0;

void TIMER0_START(){
    TMOD &= ~T0_GATE_; TMOD &= ~T0_CT_; // use timer mode 
    TMOD |= T0_M0_; TMOD &= ~T0_M1_; // use mode 1 for timer 
    TH0 = 0xee; TL0 = 0x00;
    EA = 1; ET0 = 1; 
    TR0 = 1; // start_timer 
}

void TIMER0_STOP(){
    TR0 = 0; // stop timer;
}

void TIMER0_Func(void) interrupt 1{
    TH0 = 0xfc; TL0 = 0x18;
    if (t0_add) t0++;
}


float HC_SRO4_GetDis(void){
    float res = 0;
    Trig = 1;
    Delay10us(1);
    Trig = 0;
    while (!Echo);
    t0 = 0; t0_add = 1;
    while (Echo);
    t0_add = 0;
    res = (float)t0 * 342.62/2000;
    t0 = 0; // reset
    return res;
}

void HC_SRO4_Init(void){
    TIMER0_START();
    Trig = 0;
    Echo = 0;
    Delay(10);
}