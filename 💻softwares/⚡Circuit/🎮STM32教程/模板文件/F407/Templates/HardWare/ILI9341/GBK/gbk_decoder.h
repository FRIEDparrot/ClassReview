#ifndef __GBK_DECODER_H
#define __GBK_DECODER_H

#include "stm32f4xx_hal.h"

#define FONT_FOLDER         		"font"
#define UNI2GBK_FILE_NAME   		"UNI2GBK.BIN"
#define GBK12_STD_FONT_FILE_NAME    "GBK12_songti.fon"
#define GBK16_STD_FONT_FILE_NAME    "GBK16_songti.fon"
#define GBK24_STD_FONT_FILE_NAME    "GBK24_songti.fon"

/** font size code ->  Not Modify it ! */
#define GBK_FONT_SIZE_12 	(0x02)
#define GBK_FONT_SIZE_16 	(0x04)
#define GBK_FONT_SIZE_24 	(0x08)

/****  gbk_font.h ---------------------*/
typedef struct{
    uint8_t Font_exist;
    uint8_t Uni2Gbk_exist;
    uint32_t gbk12addr;    // 12-hight gbk address
    uint32_t gbk12size;    // 12-hight gbk size 
    uint32_t gbk16addr;    // 16-hight gbk address
    uint32_t gbk16size;    // 16-hight gbk size 
    uint32_t gbk24addr;    // 24-hight gbk address
    uint32_t gbk24size;    // 24-hight gbk size
}GBK_Font_Info;



uint8_t GBK_FONT_Init(void);
uint8_t GBK_FONT_GetCode(uint16_t gbk_code, uint8_t* buff, uint8_t font_size);
uint8_t GBK_Font_Draw(uint16_t x, uint16_t y, uint8_t *buff, uint8_t font_size, void(*draw_point_func)(uint16_t, uint16_t));

#endif // !__GBK_DECODER_H
