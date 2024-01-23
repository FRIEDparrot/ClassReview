#include "timer_controller.h"
#include <REGX52.H>

// calculate 5ms each time, which means 4608 times of crystal oscallation
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

void TIMER1_START(){
    TMOD &= ~T1_GATE_; TMOD &= ~T1_CT_; // use timer mode
    TMOD |= T1_M0_; TMOD &= ~T1_M1_;  // M0 = 1, M1 = 0
    TH1 = 0xdc; TL1 = 0x00;
    EA = 1; ET1 = 1; 
    TR1 = 1;
}

void TIMER1_STOP(){
    TR1 = 0;
}

