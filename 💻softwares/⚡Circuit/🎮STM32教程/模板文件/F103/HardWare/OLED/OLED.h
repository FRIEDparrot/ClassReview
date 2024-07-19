#ifndef __OLED_H
#define __OLED_H

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

// 100.00 format float
void OLED_ShowFloat(int row, int col, float num){
    float temp = num;
    if (num > 0) OLED_ShowChar(row,col,'+'); else {OLED_ShowChar(row,col,'-'); temp = -temp;}
    OLED_ShowNum(row,col+1, (int)temp,3); OLED_ShowChar(row,col+4,'.');  OLED_ShowNum(row,col+5, (int)(temp * 100)%100, 2);
}

#endif
