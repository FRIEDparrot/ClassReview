#ifndef LED_CONTROLLER
#define LED_CONTROLLER 
#include "public.h"

void LED_ShowNumber(u8 arr[8]);
void LED_ShowDateTime(u8 hour, u8 min, u8 sec, u8 day, u8 state); 
void LED_ShowExactTime(u8 hour, u8 min, u8 sec, u8 xms);
void LED_ShowChar(u8 loc, u8 ch);
void LED_SelfCheck();

#endif 
