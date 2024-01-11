#include <REGX52.H>
#include "LED_controller.h" 
#define LED_COL_PORT P0

u8 gsm_code[17] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 
				    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71}; 

u8 week_code[7] = {0x7f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d}; 

void LED_SelfCheck(){
    u8 a[8];
    int i, j, x;
    for (i = 0; i <= 9; i++){
        for (j = 0; j < 8 ; j++) a[j] = i;
        for (x = 0; x < 70; x++){
            LED_ShowNumber(a);
            Delay(1);
        }
    }
}

// use 74HC138 decoder for each LED light 
void LED_SET(u8 num){
	u8 n = num;
	P2_2 = n%2; n/=2;
	P2_3 = n%2; n/=2;
	P2_4 = n%2; n/=2;
}

// clear the Controlling PORT
void LED_CLEAR(){
    P0 = 0x00; 
}

void LED_ShowNumber(u8 Arr[8]){
    u8 i; 
    for (i = 0; i < 8; i++){
        LED_SET(7-i); LED_COL_PORT = gsm_code[Arr[i]]; Delay(1); LED_CLEAR(); Delay(1);
    }
}

void LED_ShowNum(u8 loc, u8 num, u8 dot){
    u8 res = gsm_code[num];
    if (dot) res|=0x80;
    LED_SET(7-loc); LED_COL_PORT = res; Delay(1); LED_CLEAR(); Delay(1);
}

// use state to chose which to ignite for a period 
void LED_ShowDateTime(u8 hour, u8 min, u8 sec, u8 day, u8 Setstate){
    if (Setstate!=1) {
        LED_SET(7); LED_COL_PORT = gsm_code[hour/10]; Delay(1); LED_CLEAR(); Delay(1);
        LED_SET(6); LED_COL_PORT = gsm_code[hour%10]| 0x80; Delay(1); LED_CLEAR(); Delay(1);
    }
    if (Setstate!=2) {
        LED_SET(5); LED_COL_PORT = gsm_code[min/10]; Delay(1); LED_CLEAR(); Delay(1);
        LED_SET(4); LED_COL_PORT = gsm_code[min%10]| 0x80; Delay(1); LED_CLEAR(); Delay(1);
    }
    if (Setstate!=3) {
        LED_SET(3); LED_COL_PORT = gsm_code[sec/10]; Delay(1); LED_CLEAR(); Delay(1);
        LED_SET(2); LED_COL_PORT = gsm_code[sec%10]; Delay(1); LED_CLEAR(); Delay(1);
    }
    if (Setstate!=4 && day >= 0 && day < 7) { // if day < 0, don't show it 
        LED_SET(0); LED_COL_PORT = week_code[day]; Delay(1); LED_CLEAR(); Delay(1);
    }
}

void LED_ShowExactTime(u8 hour, u8 min, u8 sec, u8 xms){
    LED_SET(7); LED_COL_PORT = gsm_code[hour/10]; Delay(1); LED_CLEAR(); Delay(1);
    LED_SET(6); LED_COL_PORT = gsm_code[hour%10]| 0x80; Delay(1); LED_CLEAR(); Delay(1);
    LED_SET(5); LED_COL_PORT = gsm_code[min/10]; Delay(1); LED_CLEAR(); Delay(1);
    LED_SET(4); LED_COL_PORT = gsm_code[min%10]| 0x80; Delay(1); LED_CLEAR(); Delay(1);
    LED_SET(3); LED_COL_PORT = gsm_code[sec/10]; Delay(1); LED_CLEAR(); Delay(1);
    LED_SET(2); LED_COL_PORT = gsm_code[sec%10]| 0x80; Delay(1); LED_CLEAR(); Delay(1);
    LED_SET(1); LED_COL_PORT = gsm_code[xms/10]; Delay(1); LED_CLEAR(); Delay(1);
    LED_SET(0); LED_COL_PORT = gsm_code[xms%10]; Delay(1); LED_CLEAR(); Delay(1);
}

// show any random chararacter by define P2
void LED_ShowChar(u8 loc, u8 ch){
    LED_SET(7 - loc); LED_COL_PORT = ch; Delay(1); LED_CLEAR(); Delay(1);
}
/*
state 0: all light; 
state 1: set Hour;  state 2: set Minute; state 3: set Second; state 4: set day;
which will not render this in the LED port. 
*/