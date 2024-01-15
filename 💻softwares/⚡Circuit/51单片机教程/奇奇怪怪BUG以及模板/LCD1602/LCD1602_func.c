#include "LCD1602_func.h"
#include "STRING.H"
#include "INTRINS.H"

sbit LCD_RS = P2^6;
sbit LCD_WR = P2^5;
sbit LCD_EN = P2^7;

sbit D7 = P0^7;
#define LCD_DATA_PORT P0

struct LCD1602_Cursor cursor; // record the cursor location 

void LCD1602_Write_Cmd(u8 cmd){
    LCD_EN = 0;
    _nop_();
    LCD_RS = 0;
    _nop_();
    LCD_WR = 0;
    _nop_();
    LCD_DATA_PORT = cmd;
    Delay(1); // wait for 1ms 
    LCD_EN = 1;
    Delay(1);
    LCD_EN = 0;
    _nop_();
}

void LCD1602_Write_Dat(u8 dat){
    LCD_EN = 0; // create 
    _nop_();
    LCD_RS = 1;   // data mode 
    _nop_();
    LCD_WR = 0;   // write data
    _nop_();
    LCD_DATA_PORT = dat;
    Delay(1);     // wait for 1ms after set port
    LCD_EN = 1;
    Delay(1);
    LCD_EN = 0;
    _nop_();
}

// must be 0x38 for current LED 
void LCD1602_SetFunc(u8 func){
    LCD1602_Write_Cmd(func);
}

// open normally as 0x0c, close as 0x08 but can set more 
void LCD1602_SetDisp(u8 disp){
    LCD1602_Write_Cmd(disp);
}

// 0x06 as default -> cursor move right after write data 
void LCD1602_SetMode(u8 mode){
    LCD1602_Write_Cmd(mode);
}

void LCD1602_ClearScreen(void){
    LCD1602_Write_Cmd(0x01);
    Delay(4);   // need 160 ms to init
}

/// @brief Init LCD1602
/// @param Func must be 0x38 for current LED 
/// @param Disp open normally as 0x0c, close as 0x08 but can set more 
/// @param Mode  0x06 as cursor move right after write data
void LCD1602_Init(Func, Disp,  Mode){
    LCD_EN = 0;
    cursor.x = 0; cursor.y = 0;
    LCD1602_SetFunc(Func);  // must set function first 
    LCD1602_SetDisp(Disp);
    LCD1602_SetMode(Mode);  // cursor move and screen move 
    // Clear Screen
    LCD1602_ClearScreen();
    // firstly write the address
}

void LCD1602_WriteCode(u8 addr, u8 ASCII_code){
    LCD1602_Write_Cmd(0x80|addr);
    Delay10us(5);
    LCD1602_Write_Dat(ASCII_code);  // not need use 
    Delay10us(5);
}

// x from 0 to 15, y from 0 to 1 
void LCD1602_ShowChar(u8 x, u8 y, u8 ch){
    // (all message shown through  show char)
    u8 addr = 0x00;
    if (x > 15) return;
    if (y == 0) {
        addr += x;
        LCD1602_WriteCode(addr, ch);
    }else if (y == 1){
        addr = 0x40 + x;
        LCD1602_WriteCode(addr, ch);
    }
    cursor.x = x+1;
    cursor.y = y;
}

// x = 0-15,  y= 0 or 1, str is string 
void LCD1602_ShowStr(u8 x, u8 y, char* str){
    u8 i;
    u8 len = strlen(str);
    if (x + len > 16) return; // too long !!
    for (i = 0; i < len; i++){
        LCD1602_ShowChar(x + i, y, str[i]);
    }
}

struct LCD1602_Cursor LCD1602_GetCursor(void){
    return cursor;
}

// 0 move left, 1 move right;
void LCD1602_MoveCursor(u8 direction){
    if (direction == 0){
        if (cursor.x <= 0) return;
        LCD1602_Write_Cmd(0x10);
        cursor.x --;
    }else if (direction ==1){
        if (cursor.x >= 16) return; // maximum 16;
        LCD1602_Write_Cmd(0x14);
        cursor.x ++;
    }
}

u8 LCD1602_Read_Dat(){
    u8 res = 0x00;
    LCD_EN = 0;
    _nop_();
    LCD_RS = 1;  // data
    _nop_();
    LCD_WR = 1;
    _nop_();
    res = LCD_DATA_PORT;
    Delay(1); // wait for 1ms 
    LCD_EN = 1;
    Delay(1);
    LCD_EN = 0;
    _nop_();
    return res;
}

// seldom used 
u8 LCD1602_ReadDDROM(u8 addr){
    u8 res;
    LCD1602_Write_Cmd(addr); // output AC pointed data
    res = LCD1602_Read_Dat();
    return res;
}



// // if is busy return 1, automatically called 
// u8 LCD1602_Busy(){
//     LCD_RS = 0;
//     _nop_();
//     LCD_WR = 1;  // read mode;
//     _nop_();
//     if (D7) return 1;
//     else return 0;
// }