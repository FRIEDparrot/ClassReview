#include "EEPROM_OP.h"
// RXD : P3_0, TXD : P3_1; 
void UART_INIT(){  // use double frequency 
    // Init Timer 1 as mode 2, M0 = 0, M1 = 1; -> 
    TMOD = 0x20;
    TH1 = 0xFA; TL1 = 0xFA;  // init the timer 
    // PCON configuration 
    PCON = 0x80;
    // SCON configuration  
    EA = 1; ES = 1; // enable the series interruption (not need ET1))
    SM0 = 0; SM1 = 1; REN = 1; // enable the receiver
    TR1 = 1;        // start timer 1;
}

void UART_FUNC() interrupt 4{ // send all to computer.
    unsigned char msg = 0;
    RI = 0; 
    msg = SBUF;
    SBUF = msg;
    while(!TI); // wait for send the data ;
    TI = 0;
}

// Debug Message 
void UART_Debug(char* msg){
    ES = 0; // disable the interruption when sending data.
    TI = 1; // first, we enable the TI before 
    printf(msg);
    while (!TI); // wait for sending;
    TI = 0;
    ES = 1;
}