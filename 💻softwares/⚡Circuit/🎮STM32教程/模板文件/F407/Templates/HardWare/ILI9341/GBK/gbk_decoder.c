#include "gbk_decoder.h"
#include "W25Qxx.h"
#include "ff.h"
#include "stdio.h"
#include "string.h"

#if (SYS_USER_MALLOC == 0)
#include "stdlib.h"
#define __gbk_malloc(size) malloc(size);
#define __gbk_free(p) free(p);
#else
#include "../MALLOC/mymalloc.h"
#define __gbk_malloc(size) mymalloc(SRAMIN,size);
#define __gbk_free(p) myfree(SRAMIN, p);
#endif

#define MAX_ABSOLUTE_PATH_LENGTH 255

static GBK_Font_Info gbk_info = 
{
	0,
	0,
	0,   
	12 * 12 / 8,    // 18 bytes
	0,
	16 * 16 / 8,    // 32 bytes
	0,
	24 * 24 / 8	  	// 72 bytes 
};  /* use a struct for storge the basic message of the work bank*/


/** check if the file exist in "font" folder */
void __gbk_checkfile(char* FileName, uint8_t* exist){
    FIL fp;
    char *fname = (char*)__gbk_malloc(MAX_ABSOLUTE_PATH_LENGTH);
	
    sprintf(fname, "%s/%s/%s", W25Qxx_ROOTDIRECTORY, FONT_FOLDER, FileName);
    if (f_open(&fp, fname, FA_READ) == FR_OK){
        (*exist) = 1;
    }else{
        (*exist) = 0;
    }
	__gbk_free(fname);
    f_close(&fp);
}


/*** Init GBK font and check if file is  exist */  
/// @brief 
/// @param 
/// @return 0: Init succeed, 1: init failed
uint8_t GBK_FONT_Init(void){
    if (W25Qxx_InitFS()) return 1;
	gbk_info.Font_exist = 0;
	uint8_t font12_exist = 0, font16_exist = 0, font24_exist = 0;
    __gbk_checkfile(GBK12_STD_FONT_FILE_NAME, (& font12_exist));
    __gbk_checkfile(GBK16_STD_FONT_FILE_NAME, (& font16_exist));
    __gbk_checkfile(GBK24_STD_FONT_FILE_NAME, (& font24_exist));
    __gbk_checkfile(UNI2GBK_FILE_NAME, &(gbk_info.Uni2Gbk_exist));
	
	gbk_info.Font_exist |= (font12_exist << 1);
	gbk_info.Font_exist |= (font16_exist << 2); 
	gbk_info.Font_exist |= (font24_exist << 3);

    if (!gbk_info.Font_exist){
        return 1;  // no font file;
    }
    return 0;
}

/// @brief get the matrix of pixels from gbk code 
/// @param gbk_code  
/// @param buff 	 buffer to receive the matrix of pixels 
/// @param font_size use  GBK_FONT_SIZE_12, GBK_FONT_SIZE_16, GBK_FONT_SIZE_24
/// @note  Flash  must be initialized before using gbk
/// @return 0: succeed ,1 file open error, 2: not valid gbk code 
uint8_t GBK_FONT_GetCode(uint16_t gbk_code, uint8_t* buff, uint8_t font_size){
	FIL fp; uint8_t font_size_bytes;
	uint8_t hb = (gbk_code >> 8), lb = (gbk_code & 0xff);
	char *fname = (char*)__gbk_malloc(MAX_ABSOLUTE_PATH_LENGTH);
	
	if ((gbk_info.Font_exist & font_size) == 0){
		return 1;   // no font file 
	}
	switch (font_size){
	case GBK_FONT_SIZE_12:
					sprintf(fname, "%s/%s/%s", W25Qxx_ROOTDIRECTORY, FONT_FOLDER, GBK12_STD_FONT_FILE_NAME);
					font_size_bytes = gbk_info.gbk12size;
					break;
	case GBK_FONT_SIZE_16:
					sprintf(fname, "%s/%s/%s", W25Qxx_ROOTDIRECTORY, FONT_FOLDER, GBK16_STD_FONT_FILE_NAME);
					font_size_bytes = gbk_info.gbk16size;
					break;
	case GBK_FONT_SIZE_24:
					sprintf(fname, "%s/%s/%s", W25Qxx_ROOTDIRECTORY, FONT_FOLDER, GBK24_STD_FONT_FILE_NAME);
					font_size_bytes = gbk_info.gbk24size;
					break;
	}
	
	/*** Firstly, judge if the character is in gbk ***/
	if (hb < 0x81 || lb < 0x40 || lb == 0xff || hb == 0xff){
		return 2;
	}
	FRESULT res = f_open(&fp, fname, FA_READ);
	__gbk_free(fname);
	if (res != FR_OK){
		return 1;
	}
	unsigned long font_addr = 0 ;
	if (lb < 0x7f){  // GBK Code  
		font_addr = ((unsigned long)190 * (hb - 0x81) + lb - 0x40) * font_size_bytes;
	}
	else if (lb > 0x7f)
	{
		font_addr = ((unsigned long)190 * (hb - 0x81) + lb - 0x41) * font_size_bytes;
	}
    else
    {
        return 2;  // can't find the gbk code
	}
	if (f_lseek(&fp, font_addr)!=FR_OK || f_read(&fp, buff, font_size_bytes, NULL)!=FR_OK){ // file destrooyed
		f_close(&fp);
		return 1;
	}
	f_close(&fp);
	return 0;
}

uint8_t GBK_Font_Draw(uint16_t x, uint16_t y, uint8_t *buff, uint8_t font_size, void(*draw_point_func)(uint16_t, uint16_t)){
	uint8_t width;
	switch (font_size){
	case GBK_FONT_SIZE_12:
					width = 12;
					break;
	case GBK_FONT_SIZE_16:
					width = 16;
					break;
	case GBK_FONT_SIZE_24:
					width = 24;
					break;
	default:
		return 1;  // no font input 
	}
    for (uint8_t i = 0; i < width; i++) {
        uint16_t tmp = ((uint16_t)buff[2* i]  << 8)  + (buff[2* i + 1]);
		/** from up to down to fill the y coor */
		for (uint8_t j = 0; j < width; j++){
			if (tmp & 0x8000){
                draw_point_func(x + i, y + j);
			}
			tmp <<= 1;
		}
    }
	return 0;
}
