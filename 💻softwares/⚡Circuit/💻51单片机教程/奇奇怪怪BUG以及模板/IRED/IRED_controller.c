#include "IRED_controller.h"
#define IRED P3_2

u8 Gired_Data[4] = {0x00,0x00,0x00,0x00};

u8 Control_Code = 0x00;
u8 Received = 0; // if Received = 1, it means have new message received

// receive the data of the remote control.
// enable the ired interruption (use interruption 0, P3_2) here.
void IRED_INIT(void){
    // when the remote control is pushed, it becomes low voltage level
    IT0 = 1;
    EX0 = 1;
    EA = 1;  // open the interruption
    IRED=1;	 // **initialize the port of IRED**
}

void ired() interrupt 0 {
	u8 i = 0,j = 0;
    u16 time_cnt=0;
    u8 ired_high_time=0; // set to high voltage level time 
	if(IRED==0)
	{
        // 9ms low voltage level
		time_cnt = 0;
        while (!IRED) // wait for high voltage 
        {
            Delay(1);
            time_cnt++;
            if (time_cnt > 10) {
                return; // if all are low, exit
            }
        }
        if (time_cnt < 8) return;  // delay 10us can cause great error   
        // 4.5ms high voltage level 
        time_cnt = 0;
        while (IRED){ // wait for normal low voltage level
            Delay10us(1);
            time_cnt++;
            if (time_cnt > 500) return;
        }
        //=====start send code=============
        // receive 8 digit address (32 digit, 4bytes)
        for (i = 0; i < 4; i++){
            for (j = 0; j < 8; j++){
                time_cnt = 60;
                // use time_cnt to avoid disturb 
                while (!IRED){ // no more than 600ms delay 
                    Delay10us(1);
                    time_cnt--;
                    if (time_cnt == 0) return;
                }; 
                // judge high voltage level time for 0 or 1 data 
                while (IRED){
                    Delay10us(10); // test every 0.1ms
                    ired_high_time ++;
                    if (ired_high_time >= 20) return; // 2000 us
                }
                Gired_Data[i] >>= 1;
                if (ired_high_time >= 8){  // 800us width for judge
                    Gired_Data[i] |= 0x80;
                }
                ired_high_time  = 0; // *** dont't forget reset the time !!!**
            }
        }
        // i = 0: address code ;  i = 1: address inverse code;
        // i = 2: control code ;  i = 3; control inverse code; 
        // test code validarity

        if ((Gired_Data[1] == ~Gired_Data[0]) && (Gired_Data[3] == ~Gired_Data[2])){
            Control_Code = Gired_Data[2]; // * is generate value from pointer 
            Received = 1;
        }
    }
}

// input the command, if return 0 means not triggered,1 is triggered.
u8 IRED_Get_Command(u8 *cmd){
    if (Received){
        *cmd = Control_Code; // copy and paste the value
        Received = 0;
        return 1;
    }
    return 0;
}


// mode 0: real; mode 1: simulation -> give code from 0 to 20 as command
u8 IRED_CMD_Mapping(u8 *cmd, u8 mode){
    u8 res = 0x00;
    if (mode==0){
        switch (*cmd)
        {
        case 0x45: res = 0;break;
        case 0x46: res = 1;break;
        case 0x47: res = 2;break;
        case 0x44: res = 3;break;
        case 0x40: res = 4;break;
        case 0x43: res = 5;break;
        case 0x07: res = 6;break;
        case 0x15: res = 7;break;
        case 0x09: res = 8;break;
        case 0x16: res = 9;break;
        case 0x19: res = 10;break;
        case 0x0d: res = 11;break;
        case 0x0C: res = 12;break;
        case 0x18: res = 13;break;
        case 0x5E: res = 14;break;
        case 0x08: res = 15;break;
        case 0x1C: res = 16;break;
        case 0x5A: res = 17;break;
        case 0x42: res = 18;break;
        case 0x52: res = 19;break;
        case 0x4A: res = 20;break;
        default: break;
        }
    }else{
        switch (*cmd)
        {
        case 0x12: res = 0;break;
        case 0x1A: res = 1;break;
        case 0x1E: res = 2;break;
        case 0x01: res = 3;break;
        case 0x02: res = 4;break;
        case 0x03: res = 5;break;
        case 0x04: res = 6;break;
        case 0x05: res = 7;break;
        case 0x06: res = 8;break;
        case 0x07: res = 9;break;
        case 0x08: res = 10;break;
        case 0x09: res = 11;break;
        case 0x0A: res = 12;break;
        case 0x1B: res = 13;break;
        case 0x1F: res = 14;break;
        case 0x0C: res = 15;break;
        case 0x0D: res = 16;break;
        case 0x0E: res = 17;break;
        case 0x00: res = 18;break;
        case 0x0F: res = 19;break;
        case 0x19: res = 20;break;
        default: break;
        }
    }
    return res;
} 