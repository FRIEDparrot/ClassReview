//////////////////////////////////////////////////////////////////////////////////	 

//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              CLK   接PA5（SCL）
//              MOSI   接PA7（SDA）
//              RES  接PB0
//              DC   接PB1  
//							BLK  接PA4 可以悬空
//							MISO 可以不接
//              ----------------------------------------------------------------
//******************************************************************************/
#ifndef __ST7789_H
#define __ST7789_H
#include "sys.h"
#include "stdlib.h"

#ifdef __cplusplus
extern "C"{
#endif

#define LCD_W 240
#define LCD_H 240
// #define	u8 unsigned char
// #define	u16 unsigned int
// #define	u32 unsigned long

//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64

//----------------- Extend the OLED port to SPI1 (PA4 - PA7) -----------------
#define OLED_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_3)    //RES:PA4
#define OLED_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_3)

// note if use PA4 can't download?
#define OLED_DC_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_6)    // DC: PA6(Data/Command)
#define OLED_DC_Set() GPIO_SetBits(GPIOA,GPIO_Pin_6)      // 

// #define OLED_BLK_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_4)  // BLK: floating 
// #define OLED_BLK_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_4)    // 

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

extern  u16 BACK_COLOR, POINT_COLOR;   //背景色，画笔色

void LCD_Init(void);
void LCD_Clear(u16 Color);
void LCD_WaitBSY(void);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);

void LCD_Write_Bus(char dat);
void LCD_WR_DATA8(char da); //发送数据-8位参数
void LCD_WR_DATA16(int da);
void LCD_WR_REG(char da);

void LCD_DrawPoint(u16 x,u16 y);//画点
void LCD_DrawPoint_big(u16 x,u16 y);//画一个大点
uint16_t  LCD_ReadPoint(u16 x,u16 y); //读点
void Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode);//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len);//显示数字
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len);//显示2个数字
void LCD_ShowString(u16 x,u16 y,const u8 *p);		 //显示一个字符串,16字体


void showhanzi(unsigned int x,unsigned int y,unsigned char index);



//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


#ifdef __cplusplus
}
#endif
					  		 
#endif
