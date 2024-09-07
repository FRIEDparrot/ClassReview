//////////////////////////////////////////////////////////////////////////////////	 
//This program is for study purposes only and may not be used for any other purpose without the permission of the author
//Test hardware: MCU STM32F407ZGT6, punctual atomic Explorer STM32F4 development board, main frequency 168MHZ, crystal oscillator 12MHZ
//QDtech-TFT LCD driver for STM32 IO simulation
//Xiao Feng @ Shenzhen Getheco., Ryder
//Company website: www.qdtft.com
//Taobao website: http://qdtech.taobao.com
//Wiki technical site: http://www.lcdwiki.com
//Our company provides technical support, and any technical problems are welcome to communicate and learn at any time
//Fixed line (fax): +86 0755-23594567
//Mobile: 15989313508 (Feng Gong)
//E-mail: lcdwiki01@gmail.com support@lcdwiki.com goodtft@163.com
//Technical support QQ: 3002773612 3002778157
//QQ group for technical exchanges: 324828016
//Creation date: 2018/08/09
//Version: V1.0
//All rights reserved, piracy must be investigated.
//Copyright(C) Shenzhen Quandong Electronic Technology Co., Ltd. 2018-2028
//All rights reserved
/****************************************************************************************************
//========================================= Power Wiring ================================================//
//LCD module STM32 microcontroller
//VCC is connected to DC5V/3.3V//power supply
//GND to GND //power to ground
//======================================= LCD screen data cable wiring ==========================================//
//The default data bus type of this module is SPI bus
//LCD module STM32 microcontroller
//SDI(MOSI) is connected to PB5 //LCD SPI bus data write signal
//SDO (MISO) is connected to PB4 // LCD SPI bus data reading signal, if you don't need to read, you can leave it unwired
//======================================= LCD screen control cable wiring ==========================================//
//LCD module STM32 microcontroller
//LED connected to PB13 // LCD screen backlight control signal, if no control is required, connect to 5V or 3.3V
//The SCK is connected to the PB3 //LCD SPI bus clock signal
//DC/RS is connected to PB14 //LCD screen data/command and control signal
//RST is connected to PB12 // LCD screen reset control signal
//CS is connected to PB15 // LCD screen chip selection control signal
//========================================= touch screen contact cable ========================================= //
//If the module is not touch-enabled or has touch-enabled but does not require touch-capability, touchscreen wiring is not required
//LCD module STM32 microcontroller
//T_IRQ Connect PB1 // touch screen touch interrupt signal
//T_DO Connect PB2 //touch screen SPI bus to read the signal
//T_DIN Connect PF11 //touch screen SPI bus to write signal
//T_CS Connect PC5 // touch screen chip selection control signal
//T_CLK Connect PB0 // touch screen SPI bus clock signal
**************************************************************************************************/	
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#include "ILI9341_GUI.h"
#include "string.h"
#include "font.h"
#include "delay.h"

/*****************************************************************************
 * @name       :void ILI9341_DrawPoint(uint16_t x,uint16_t y)
 * @date       :2018-08-09 
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/	
void ILI9341_DrawPoint(uint16_t x,uint16_t y)
{
	ILI9341_SetCursor(x,y);
	ILI9341_WriteData_16Bit(POINT_COLOR); 
}

/*******************************************************************
 * @name       :void ILI9341_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
 * @date       :2018-08-09 
 * @function   :Draw a line between two points
 * @parameters :x1:the bebinning x coordinate of the line
                y1:the bebinning y coordinate of the line
								x2:the ending x coordinate of the line
								y2:the ending y coordinate of the line
 * @retvalue   :None
********************************************************************/
void ILI9341_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1;
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; 		
	else if(delta_x==0)incx=0;	
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )
	{  
		ILI9341_DrawPoint(uRow,uCol);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

/*****************************************************************************
 * @name       :void ILI9341_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
 * @date       :2018-08-09 
 * @function   :Draw a rectangle
 * @parameters :x1:the bebinning x coordinate of the rectangle
                y1:the bebinning y coordinate of the rectangle
								x2:the ending x coordinate of the rectangle
								y2:the ending y coordinate of the rectangle
 * @retvalue   :None
******************************************************************************/
void ILI9341_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	ILI9341_DrawLine(x1,y1,x2,y1);
	ILI9341_DrawLine(x1,y1,x1,y2);
	ILI9341_DrawLine(x1,y2,x2,y2);
	ILI9341_DrawLine(x2,y1,x2,y2);
}  

/*****************************************************************************
 * @name       :void ILI9341_DrawRectangleFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
 * @date       :2018-08-09 
 * @function   :Filled a rectangle
 * @parameters :x1:the bebinning x coordinate of the filled rectangle
                y1:the bebinning y coordinate of the filled rectangle
								x2:the ending x coordinate of the filled rectangle
								y2:the ending y coordinate of the filled rectangle
 * @retvalue   :None
******************************************************************************/  
void ILI9341_DrawRectangleFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	ILI9341_Fill(x1,y1,x2,y2,POINT_COLOR);
}
 
/*****************************************************************************
 * @name       :void _draw_circle_8(int xc, int yc, int x, int y, uint16_t c)
 * @date       :2018-08-09 
 * @function   :8 symmetry circle drawing algorithm (internal call)
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center 
								x:the x coordinate relative to the Circular center 
								y:the y coordinate relative to the Circular center 
								c:the color value of the circle
 * @retvalue   :None
******************************************************************************/  
void _draw_circle_8(int xc, int yc, int x, int y, uint16_t c)
{
	POINT_COLOR = c;
	ILI9341_DrawPoint(xc + x, yc + y);
	ILI9341_DrawPoint(xc - x, yc + y);
	ILI9341_DrawPoint(xc + x, yc - y);
	ILI9341_DrawPoint(xc - x, yc - y);
	ILI9341_DrawPoint(xc + y, yc + x);
	ILI9341_DrawPoint(xc - y, yc + x);
	ILI9341_DrawPoint(xc + y, yc - x);
	ILI9341_DrawPoint(xc - y, yc - x);
}

/*****************************************************************************
 * @name       :void ILI9341_DrawCircle(int xc, int yc,uint16_t c,int r, int fill)
 * @date       :2018-08-09 
 * @function   :Draw a circle of specified size at a specified location
 * @parameters :xc:the x coordinate of the Circular center 
                yc:the y coordinate of the Circular center 
								r:Circular radius
								fill:1-filling,0-no filling
 * @retvalue   :None
******************************************************************************/  
void ILI9341_DrawCircle(int xc, int yc,uint16_t c, int r, int fill)
{
	int x = 0, y = r, yi, d;
	d = 3 - 2 * r;
	if (fill) 
	{
		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				_draw_circle_8(xc, yc, x, yi, c);

			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
	else
	{
		// not filled 
		while (x <= y) {
			_draw_circle_8(xc, yc, x, y, c);
			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}

/*****************************************************************************
 * @name       :void ILI9341_DrawTriangel(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
 * @date       :2018-08-09 
 * @function   :Draw a triangle at a specified position
 * @parameters :x0:the bebinning x coordinate of the triangular edge 
                y0:the bebinning y coordinate of the triangular edge 
								x1:the vertex x coordinate of the triangular
								y1:the vertex y coordinate of the triangular
								x2:the ending x coordinate of the triangular edge 
								y2:the ending y coordinate of the triangular edge 
 * @retvalue   :None
******************************************************************************/ 
void ILI9341_DrawTriangel(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	ILI9341_DrawLine(x0,y0,x1,y1);
	ILI9341_DrawLine(x1,y1,x2,y2);
	ILI9341_DrawLine(x2,y2,x0,y0);
}

static void _swap(uint16_t *a, uint16_t *b)
{
	uint16_t tmp;
  tmp = *a;
	*a = *b;
	*b = tmp;
}

/*****************************************************************************
 * @name       :void ILI9341_DrawTriangelFill(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
 * @date       :2018-08-09 
 * @function   :filling a triangle at a specified position
 * @parameters :x0:the bebinning x coordinate of the triangular edge 
                y0:the bebinning y coordinate of the triangular edge 
								x1:the vertex x coordinate of the triangular
								y1:the vertex y coordinate of the triangular
								x2:the ending x coordinate of the triangular edge 
								y2:the ending y coordinate of the triangular edge 
 * @retvalue   :None
******************************************************************************/ 
void ILI9341_DrawTriangelFill(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	uint16_t a, b, y, last;
	int dx01, dy01, dx02, dy02, dx12, dy12;
	long sa = 0;
	long sb = 0;
 	if (y0 > y1) 
	{
    _swap(&y0,&y1); 
		_swap(&x0,&x1);
 	}
 	if (y1 > y2) 
	{
    _swap(&y2,&y1); 
		_swap(&x2,&x1);
 	}
  if (y0 > y1) 
	{
    _swap(&y0,&y1); 
		_swap(&x0,&x1);
  }
	if(y0 == y2) 
	{ 
		a = b = x0;
		if(x1 < a)
    {
			a = x1;
    }
    else if(x1 > b)
    {
			b = x1;
    }
    if(x2 < a)
    {
			a = x2;
    }
		else if(x2 > b)
    {
			b = x2;
    }
		ILI9341_Fill(a,y0,b,y0,POINT_COLOR);
    return;
	}
	dx01 = x1 - x0;
	dy01 = y1 - y0;
	dx02 = x2 - x0;
	dy02 = y2 - y0;
	dx12 = x2 - x1;
	dy12 = y2 - y1;
	
	if(y1 == y2)
	{
		last = y1; 
	}
  else
	{
		last = y1-1; 
	}
	for(y=y0; y<=last; y++) 
	{
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
    sb += dx02;
    if(a > b)
    {
			_swap(&a,&b);
		}
		ILI9341_Fill(a,y,b,y,POINT_COLOR);
	}
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) 
	{
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if(a > b)
		{
			_swap(&a,&b);
		}
		ILI9341_Fill(a,y,b,y,POINT_COLOR);
	}
}

/*****************************************************************************
 * @name       :void ILI9341_ShowChar(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, uint8_t num,uint8_t size,uint8_t mode)
 * @date       :2018-08-09 
 * @function   :Display a single English character
 * @parameters :x:the bebinning x coordinate of the Character display position
                y:the bebinning y coordinate of the Character display position
								fc:the color value of display character
								bc:the background color of display character
								num:the ascii code of display character(0~94)
								size:the size of display character
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/ 
void ILI9341_ShowChar(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, uint8_t num,uint8_t size,uint8_t mode)
{  
    uint8_t temp;
    uint8_t pos,t;
	uint16_t colortemp=POINT_COLOR;      
		   
	num=num-' '; 
	ILI9341_SetWindows(x,y,x+size/2-1,y+size-1);
	if(!mode) 
	{		
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];// 1206 font 
			else temp=asc2_1608[num][pos];		 // 1608 font
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)ILI9341_WriteData_16Bit(fc); 
				else ILI9341_WriteData_16Bit(bc); 
				temp>>=1; 
				
		    }
			
		}
	}else//Overlay method
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];//Invoke 1206 fonts
			else temp=asc2_1608[num][pos];		 //Call 1608 fonts
			for(t=0;t<size/2;t++)
		    {   
				POINT_COLOR=fc;              
		        if(temp&0x01)ILI9341_DrawPoint(x+t,y+pos);//Draw a dot
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	
	ILI9341_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//The recovery window is full screen
}

/*****************************************************************************
 * @name       :void ILI9341_ShowString(uint16_t x,uint16_t y,uint8_t size,uint8_t *p,uint8_t mode)
 * @date       :2018-08-09 
 * @function   :Display English string
 * @parameters :x:the bebinning x coordinate of the English string
                y:the bebinning y coordinate of the English string
								p:the start address of the English string
								size:the size of display character
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/   	  
void ILI9341_ShowString(uint16_t x,uint16_t y,uint8_t size,uint8_t *p,uint8_t mode)
{         
    while((*p<='~')&&(*p>=' '))//Determine if it is an illegal character!
    {   
		if(x>(lcddev.width-1)||y>(lcddev.height-1)) 
		return;     
        ILI9341_ShowChar(x,y,POINT_COLOR,BACK_COLOR,*p,size,mode);
        x+=size/2;
        p++;
    }  
} 

/*****************************************************************************
 * @name       :uint32_t mypow(uint8_t m,uint8_t n)
 * @date       :2018-08-09 
 * @function   :get the nth power of m (internal call)
 * @parameters :m:the multiplier
                n:the power
 * @retvalue   :the nth power of m
******************************************************************************/ 
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}

/*****************************************************************************
 * @name       :void ILI9341_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
 * @date       :2018-08-09 
 * @function   :Display number
 * @parameters :x:the bebinning x coordinate of the number
                y:the bebinning y coordinate of the number
								num:the number(0~4294967295)
								len:the length of the display number
								size:the size of display number
 * @retvalue   :None
******************************************************************************/  			 
void ILI9341_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				ILI9341_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	ILI9341_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,temp+'0',size,0); 
	}
} 

/*****************************************************************************
 * @name       :void GUI_DrawFont16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode)
 * @date       :2018-08-09 
 * @function   :Display a single 16x16 Chinese character
 * @parameters :x:the bebinning x coordinate of the Chinese character
                y:the bebinning y coordinate of the Chinese character
								fc:the color value of Chinese character
								bc:the background color of Chinese character
								s:the start address of the Chinese character
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/ 
void GUI_DrawFont16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode)
{
	uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;
	uint16_t x0=x;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16); // count the number of Chinese characters
	
			
	for (k=0;k<HZnum;k++) 
	{
	  if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
	  { 	ILI9341_SetWindows(x,y,x+16-1,y+16-1);
		    for(i=0;i<16*2;i++)
		    {
				for(j=0;j<8;j++)
		    	{	
					if(!mode)  // not overlaying mode 
					{
						if(tfont16[k].Msk[i]&(0x80>>j))	ILI9341_WriteData_16Bit(fc);
						else ILI9341_WriteData_16Bit(bc);
					}
					else
					{
						POINT_COLOR=fc;
						if(tfont16[k].Msk[i]&(0x80>>j))	ILI9341_DrawPoint(x,y); // Draw a point
						x++;
						if((x-x0)==16)
						{
							x=x0;
							y++;
							break;
						}
					}

				}
				
			}
			
			
		}				  	
		continue;  // find the corresponding character and exit, to avoid multiple characters being displayed
	}

	ILI9341_SetWindows(0,0,lcddev.width-1,lcddev.height-1); 
} 

/*****************************************************************************
 * @name       :void GUI_DrawFont24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode)
 * @date       :2018-08-09 
 * @function   :Display a single 24x24 Chinese character
 * @parameters :x:the bebinning x coordinate of the Chinese character
                y:the bebinning y coordinate of the Chinese character
								fc:the color value of Chinese character
								bc:the background color of Chinese character
								s:the start address of the Chinese character
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/ 
void GUI_DrawFont24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode)
{
	uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;
	uint16_t x0=x;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	// 
		
			for (k=0;k<HZnum;k++) 
			{
			  if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
			  { 	ILI9341_SetWindows(x,y,x+24-1,y+24-1);
				    for(i=0;i<24*3;i++)
				    {
							for(j=0;j<8;j++)
							{
								if(!mode) 
								{
									if(tfont24[k].Msk[i]&(0x80>>j))	ILI9341_WriteData_16Bit(fc);
									else ILI9341_WriteData_16Bit(bc);
								}
							else
							{
								POINT_COLOR=fc;
								if(tfont24[k].Msk[i]&(0x80>>j))	ILI9341_DrawPoint(x,y);
								x++;
								if((x-x0)==24)
								{
									x=x0;
									y++;
									break;
								}
							}
						}
					}
					
					
				}				  	
				continue;
			}

	ILI9341_SetWindows(0,0,lcddev.width-1,lcddev.height-1); 
}

/*****************************************************************************
 * @name       :void GUI_DrawFont32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode)
 * @date       :2018-08-09 
 * @function   :Display a single 32x32 Chinese character
 * @parameters :x:the bebinning x coordinate of the Chinese character
                y:the bebinning y coordinate of the Chinese character
								fc:the color value of Chinese character
								bc:the background color of Chinese character
								s:the start address of the Chinese character
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/ 
void GUI_DrawFont32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode)
{
	uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;
	uint16_t x0=x;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);
	for (k=0;k<HZnum;k++) 
			{
			  if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
			  { 	ILI9341_SetWindows(x,y,x+32-1,y+32-1);
				    for(i=0;i<32*4;i++)
				    {
						for(j=0;j<8;j++)
				    	{
							if(!mode)
							{
								if(tfont32[k].Msk[i]&(0x80>>j))	ILI9341_WriteData_16Bit(fc);
								else ILI9341_WriteData_16Bit(bc);
							}
							else
							{
								POINT_COLOR=fc;
								if(tfont32[k].Msk[i]&(0x80>>j))	ILI9341_DrawPoint(x,y); 
								x++;
								if((x-x0)==32)
								{
									x=x0;
									y++;
									break;
								}
							}
						}
					}
					
					
				}				  	
				continue; 
			}
	
	ILI9341_SetWindows(0,0,lcddev.width-1,lcddev.height-1);  
} 

/*****************************************************************************
 * @name       :void Show_Str(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,uint8_t size,uint8_t mode)
 * @date       :2018-08-09 
 * @function   :Display Chinese and English strings
 * @parameters :x:the bebinning x coordinate of the Chinese and English strings
                y:the bebinning y coordinate of the Chinese and English strings
								fc:the color value of Chinese and English strings
								bc:the background color of Chinese and English strings
								str:the start address of the Chinese and English strings
								size:the size of Chinese and English strings
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/	   		   
void Show_Str(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,uint8_t size,uint8_t mode)
{					
	uint16_t x0=x;							  	  
  	uint8_t bHz=0;     //Characters or Chinese
    while(*str!=0)//The data is not finished
    { 
        if(!bHz)
        {
			if(x>(lcddev.width-size/2)||y>(lcddev.height-size)) 
			return; 
	        if(*str>0x80)bHz=1; // Chinese
	        else                // English
	        {          
		        if(*str==0x0D)  // "\r\n"
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else
				{
					if(size>16) 
					{  
					ILI9341_ShowChar(x,y,fc,bc,*str,16,mode);
					x+=8;
					}
					else
					{
					ILI9341_ShowChar(x,y,fc,bc,*str,size,mode);
					x+=size/2;
					}
				} 
				str++; 
		        
	        }
        }else // 
        {   
			if(x>(lcddev.width-size)||y>(lcddev.height-size)) 
			return;  
            bHz=0;  
			if(size==32)
			GUI_DrawFont32(x,y,fc,bc,str,mode);	 	
			else if(size==24)
			GUI_DrawFont24(x,y,fc,bc,str,mode);	
			else
			GUI_DrawFont16(x,y,fc,bc,str,mode);
				
	        str+=2; 
	        x+=size; // next    
        }						 
    }   
}

/*****************************************************************************
 * @name       :void Gui_StrCenter(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,uint8_t size,uint8_t mode)
 * @date       :2018-08-09 
 * @function   :Centered display of English and Chinese strings
 * @parameters :x:the bebinning x coordinate of the Chinese and English strings
                y:the bebinning y coordinate of the Chinese and English strings
								fc:the color value of Chinese and English strings
								bc:the background color of Chinese and English strings
								str:the start address of the Chinese and English strings
								size:the size of Chinese and English strings
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/ 
void Gui_StrCenter(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,uint8_t size,uint8_t mode)
{
	uint16_t len=strlen((const char *)str);
	uint16_t x1=(lcddev.width-len*8)/2;
	Show_Str(x1,y,fc,bc,str,size,mode);
} 
 
/*****************************************************************************
 * @name       :void Gui_Drawbmp16(uint16_t x,uint16_t y,const unsigned char *p)
 * @date       :2018-08-09 
 * @function   :Display a 16-bit BMP image
 * @parameters :x:the bebinning x coordinate of the BMP image
                y:the bebinning y coordinate of the BMP image
								p:the start address of image array
 * @retvalue   :None
******************************************************************************/ 
void Gui_Drawbmp16(uint16_t x,uint16_t y,const unsigned char *p) //Display 40*40 QQ pictures
{
  	int i; 
	unsigned char picH,picL; 
	ILI9341_SetWindows(x,y,x+40-1,y+40-1);//Window settings
    for(i=0;i<40*40;i++)
	{	
	 	picL=*(p+i*2);	//The data is low
		picH=*(p+i*2+1);				
		ILI9341_WriteData_16Bit(picH<<8|picL);  						
	}	
	ILI9341_SetWindows(0,0,lcddev.width-1,lcddev.height-1);//The recovery display window is full screen
}
