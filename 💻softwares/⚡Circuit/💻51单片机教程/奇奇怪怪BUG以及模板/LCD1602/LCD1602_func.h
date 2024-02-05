#ifndef LCD1602_FUNC_H
#define LCD1602_FUNC_H
#include "public.h"

struct LCD1602_Cursor{
    u8 x ;
    u8 y ;
};

void LCD1602_Init(Func, Disp,  Mode);
void LCD1602_ClearScreen(void);
void LCD1602_ShowChar(u8 x, u8 y, u8 ch);
void LCD1602_ShowStr(u8 x, u8 y, char* str);
u8 LCD1602_ReadDDROM(u8 addr);

void LCD1602_SetMode(u8 mode);
void LCD1602_SetDisp(u8 disp);
void LCD1602_SetFunc(u8 func);

void LCD1602_MoveCursor(u8 direction);
struct LCD1602_Cursor LCD1602_GetCursor(void);
// void LCD1602_Setcursor(u8 x, u8 y);


#endif 