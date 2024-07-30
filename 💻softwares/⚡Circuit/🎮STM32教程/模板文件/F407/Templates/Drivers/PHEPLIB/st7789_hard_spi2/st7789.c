#include "st7789.h"
#include "stdlib.h"
#include "st7789_font.h"
#include "delay.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_gpio.h"
#include "usart.h"
#include <stdio.h>

uint16_t BACK_COLOR, POINT_COLOR;  // BKG_COLOR and POINT_COLOR


/* Private variables ---------------------------------------------------------*/
static SPI_HandleTypeDef st7789_spi_handler;
static uint32_t st7789_timeout = 1000;

/// @brief write register in continuous mode
/// @note !!! -> must wait until BSY = 0 
/// @param dat 
void LCD_Write_Bus(uint8_t dat)
{
	uint8_t res;
	HAL_SPI_TransmitReceive(&st7789_spi_handler, &dat, &res ,1, st7789_timeout);
}

void LCD_WR_REG(char dat)
{
    OLED_DC_Clr();
	LCD_Write_Bus(dat);
}

/// @brief used for write only one or small number of uint8_t byte
/// @param dat 
void LCD_WR_DATA8(char dat)
{
    OLED_DC_Set();
	LCD_Write_Bus(dat);
}

/// @brief used for write small number of uint16 bytes
/// @param dat 
void LCD_WR_DATA16(int dat)
{
    OLED_DC_Set();
	LCD_Write_Bus(dat>>8);
    LCD_Write_Bus(dat);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
	LCD_WR_REG(0x2a);
	
	OLED_DC_Set();
	LCD_Write_Bus(x1>>8);
	LCD_Write_Bus(x1);  
	LCD_Write_Bus(x2>>8);
	LCD_Write_Bus(x2);
	
	LCD_WR_REG(0x2b);
	
	OLED_DC_Set();
	LCD_Write_Bus(y1>>8);
	LCD_Write_Bus(y1);
	LCD_Write_Bus(y2>>8);
	LCD_Write_Bus(y2);

	LCD_WR_REG(0x2C);		 						 
}

void LCD_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_SPI2_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_13|GPIO_PIN_15;  // SCL:PB13, SDA:PB15
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
 	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;    // must add this to configure multiplexer
	HAL_GPIO_Init(GPIOB,  &GPIO_InitStructure);        

	// ** PA3, PA6 as DC **
	GPIO_InitStructure.Pin = GPIO_PIN_12|GPIO_PIN_14;   // use PA4 as RST, PA6 as DC
 	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 		 // Push-Pull Output 
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;		 // 50MHz Speed
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Alternate = 0;
 	HAL_GPIO_Init(GPIOB,  &GPIO_InitStructure);	  		              // Init GPIOA 
 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_14,  GPIO_PIN_SET); 
	
	/******** SPI Configurations **************************** */
	st7789_spi_handler.Instance = SPI2;
	st7789_spi_handler.Init.Mode = SPI_MODE_MASTER;
	st7789_spi_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	st7789_spi_handler.Init.Direction = SPI_DIRECTION_2LINES;
	st7789_spi_handler.Init.DataSize  = SPI_DATASIZE_8BIT;
	st7789_spi_handler.Init.NSS = SPI_NSS_SOFT;
	st7789_spi_handler.Init.CLKPolarity =  SPI_POLARITY_HIGH;
	st7789_spi_handler.Init.CLKPhase    =  SPI_PHASE_2EDGE;    // CPOL = CPHA = 1;
	st7789_spi_handler.Init.FirstBit    =  SPI_FIRSTBIT_MSB;   // MSB 
	st7789_spi_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	st7789_spi_handler.Init.CRCPolynomial = 7;
	st7789_spi_handler.Init.TIMode = SPI_TIMODE_DISABLE;
	HAL_SPI_Init(&st7789_spi_handler);

	__HAL_SPI_ENABLE(&st7789_spi_handler);
	/********************************************/
	OLED_DC_Clr();  // D/C Clear (Write Command)
	// reset the OLED
	OLED_RST_Clr();
	HAL_Delay(20);
	OLED_RST_Set();
	HAL_Delay(20);
	// SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;
	// SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	// SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;
	// SPI1_InitStructure.SPI_CPOL = SPI_CPOL_High;   // SCL is low when IDLE
	// SPI1_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;  // rising edge data
	// SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	// SPI1_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	// SPI1_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx; // just transfer
	
//************* Start Initial Sequence **********// 
LCD_WR_REG(0x36);
LCD_WR_DATA8(0x00);   // 0x00 for vertical, 0x70 for horz

LCD_WR_REG(0x3A); 
LCD_WR_DATA8(0x05);

LCD_WR_REG(0xB2);
LCD_WR_DATA8(0x0C);
LCD_WR_DATA8(0x0C);
LCD_WR_DATA8(0x00);
LCD_WR_DATA8(0x33);
LCD_WR_DATA8(0x33);

LCD_WR_REG(0xB7);
LCD_WR_DATA8(0x35);  

LCD_WR_REG(0xBB);
LCD_WR_DATA8(0x19);

LCD_WR_REG(0xC0);
LCD_WR_DATA8(0x2C);

LCD_WR_REG(0xC2);
LCD_WR_DATA8(0x01);

LCD_WR_REG(0xC3);
LCD_WR_DATA8(0x12);   

LCD_WR_REG(0xC4);
LCD_WR_DATA8(0x20);  

LCD_WR_REG(0xC6); 
LCD_WR_DATA8(0x0F);    

LCD_WR_REG(0xD0); 
LCD_WR_DATA8(0xA4);
LCD_WR_DATA8(0xA1);

LCD_WR_REG(0xE0);
LCD_WR_DATA8(0xD0);
LCD_WR_DATA8(0x04);
LCD_WR_DATA8(0x0D);
LCD_WR_DATA8(0x11);
LCD_WR_DATA8(0x13);
LCD_WR_DATA8(0x2B);
LCD_WR_DATA8(0x3F);
LCD_WR_DATA8(0x54);
LCD_WR_DATA8(0x4C);
LCD_WR_DATA8(0x18);
LCD_WR_DATA8(0x0D);
LCD_WR_DATA8(0x0B);
LCD_WR_DATA8(0x1F);
LCD_WR_DATA8(0x23);

LCD_WR_REG(0xE1);
LCD_WR_DATA8(0xD0);
LCD_WR_DATA8(0x04);
LCD_WR_DATA8(0x0C);
LCD_WR_DATA8(0x11);
LCD_WR_DATA8(0x13);
LCD_WR_DATA8(0x2C);
LCD_WR_DATA8(0x3F);
LCD_WR_DATA8(0x44);
LCD_WR_DATA8(0x51);
LCD_WR_DATA8(0x2F);
LCD_WR_DATA8(0x1F);
LCD_WR_DATA8(0x1F);
LCD_WR_DATA8(0x20);
LCD_WR_DATA8(0x23);

LCD_WR_REG(0x21);
LCD_WR_REG(0x11);

HAL_Delay(120);

LCD_WR_REG(0x29);
}

void LCD_Clear(uint16_t Color)
{
	Address_set(0,0,LCD_W-1,LCD_H-1);
	OLED_DC_Set();
    for(uint32_t i = 0;i < LCD_W * LCD_H;i++)
	{
		LCD_Write_Bus(Color >> 8);
		LCD_Write_Bus(Color);
	}
}


void showhanzi(unsigned int x,unsigned int y,unsigned char index)	
{
	unsigned char i,j;
	unsigned char *temp=hanzi;
    Address_set(x,y,x+31,y+31);
	temp+=index*128;
	OLED_DC_Set();
	for(j=0;j<128;j++)
	{
		for(i=0;i<8;i++)
		{
		 	if((*temp&(1<<i))!=0)
			{
				LCD_Write_Bus(POINT_COLOR >> 8);
				LCD_Write_Bus(POINT_COLOR);
			}
			else
			{
				LCD_Write_Bus(BACK_COLOR>> 8);
				LCD_Write_Bus(BACK_COLOR);
			}   
		}
		temp++;
	}
}

void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	Address_set(x,y,x,y);
	LCD_WR_DATA16(POINT_COLOR);
}

void LCD_DrawPoint_big(uint16_t x,uint16_t y)
{
	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
}

//  (xend-xsta)*(yend-ysta)
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{
	uint32_t sz = (uint32_t)(yend - ysta + 1) * (uint32_t)(xend - xsta + 1);
	Address_set(xsta,ysta,xend,yend);
	OLED_DC_Set();
	for(uint32_t i = 0; i< sz; i++)
	{
		LCD_Write_Bus(color >> 8);
		LCD_Write_Bus(color);// (color & 0xFF)
	}
}

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
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
		LCD_DrawPoint(uRow,uCol);
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

// draw rectangle 
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}


void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;

		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
}

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode)
{
    uint8_t temp;
    uint8_t pos,t;
	uint16_t x0=x;
	uint16_t colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return; 
	num=num-' ';
	Address_set(x,y,x+8-1,y+16-1);
	if(!mode)
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(uint16_t)num*16+pos];
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_WR_DATA16(POINT_COLOR);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(uint16_t)num*16+pos];
			for(t=0;t<8;t++)
		    {
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);
		        temp>>=1;
		    }
		}
	}
	POINT_COLOR=colortemp;	   	 	  
}

uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
	
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	num=(uint16_t)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0); 
	}
} 

void LCD_Show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len)
{         	
	uint8_t t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 
	}
} 

void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p)
{         
    while(*p!='\0')
    {
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;LCD_Clear(RED);}
        LCD_ShowChar(x,y,*p,0);
        x+=8;
        p++;
    }  
}

