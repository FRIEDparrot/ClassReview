#include "st7789.h"
#include "stdlib.h"
#include "st7789_font.h"  	 
#include "Delay.h"
#include "stm32f10x_spi.h"

uint16_t BACK_COLOR, POINT_COLOR;  // BKG_COLOR and POINT_COLOR

/// @brief write register in continuous mode
/// @note !!! -> must wait until BSY = 0 
/// @param dat 
void LCD_Write_Bus(char dat)
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); // when TXE = 1, can 
    SPI_I2S_SendData(SPI1, dat);
}

void LCD_WR_REG(char dat)	 
{	//	OLED_CS_Clr();
    OLED_DC_Clr();
	LCD_Write_Bus(dat);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);  // wait for busy flag
	//OLED_CS_Set();
}

void LCD_WaitBSY(void){
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);  // wait for busy flag
}

/// @brief used for write only one or small number of uint8_t byte
/// @param dat 
void LCD_WR_DATA8(char dat)
{	
	//OLED_CS_Clr();
    OLED_DC_Set();
	LCD_Write_Bus(dat);
	LCD_WaitBSY();
	//OLED_CS_Set();
}

/// @brief used for write small number of uint16 bytes
/// @param dat 
void LCD_WR_DATA16(int dat)
{
	//	OLED_CS_Clr();
    OLED_DC_Set();
	LCD_Write_Bus(dat>>8);
    LCD_Write_Bus(dat);
	LCD_WaitBSY();
	//OLED_CS_Set();
}


void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
	LCD_WR_REG(0x2a);
	
	OLED_DC_Set();
	LCD_Write_Bus(x1>>8);
	LCD_Write_Bus(x1);  
	LCD_Write_Bus(x2>>8);
	LCD_Write_Bus(x2);
	LCD_WaitBSY();
	
	LCD_WR_REG(0x2b);
	
	OLED_DC_Set();
	LCD_Write_Bus(y1>>8);
	LCD_Write_Bus(y1);
	LCD_Write_Bus(y2>>8);
	LCD_Write_Bus(y2);
	LCD_WaitBSY();

	LCD_WR_REG(0x2C);		 						 
}

void LCD_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef   SPI1_InitStructure;
	
	// Clock Cofiguration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	// GPIO Configuration(PA5, PA7)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;  // SCL:PA5, SDA:PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// ** PA3, PA6 as DC **
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6;   // use PA3 as RST, PA6 as DC
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 // Push-Pull Output 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // 50MHz Speed
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 // Init GPIOA
 	GPIO_SetBits(GPIOA, GPIO_Pin_3|GPIO_Pin_6);
	
	// ** PA6 as IPU (used for DC) *****
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 // input 
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    //
 	// GPIO_Init(GPIOA, &GPIO_InitStructure);
 	// GPIO_SetBits(GPIOA,GPIO_Pin_6);
	
	/******** SPI Configurations **************************** */
	SPI_StructInit(&SPI1_InitStructure);
	SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI1_InitStructure.SPI_CPOL = SPI_CPOL_High;   // SCL is low when IDLE
	SPI1_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;  // rising edge data
	SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI1_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI1_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx; // just transfer
	
	SPI_Init(SPI1, &SPI1_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
	/********************************************/
	OLED_DC_Clr();  // D/C Clear (Write Command)
	// reset the OLED
	OLED_RST_Clr();
	Delay_ms(20);
	OLED_RST_Set();
	Delay_ms(20);

//************* Start Initial Sequence **********// 
LCD_WR_REG(0x36);
LCD_WR_DATA8(0x00);   // 0x00 for vertical, 0x70 for horz

LCD_WR_REG(0x3A); 
LCD_WR_DATA8(0x05);

LCD_WR_REG(0xB2);
OLED_DC_Set();
LCD_Write_Bus(0x0C);
LCD_Write_Bus(0x0C);
LCD_Write_Bus(0x00);
LCD_Write_Bus(0x33);
LCD_Write_Bus(0x33);
LCD_WaitBSY();

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
OLED_DC_Set();
LCD_Write_Bus(0xD0);
LCD_Write_Bus(0x04);
LCD_Write_Bus(0x0D);
LCD_Write_Bus(0x11);
LCD_Write_Bus(0x13);
LCD_Write_Bus(0x2B);
LCD_Write_Bus(0x3F);
LCD_Write_Bus(0x54);
LCD_Write_Bus(0x4C);
LCD_Write_Bus(0x18);
LCD_Write_Bus(0x0D);
LCD_Write_Bus(0x0B);
LCD_Write_Bus(0x1F);
LCD_Write_Bus(0x23);
LCD_WaitBSY();

LCD_WR_REG(0xE1);
OLED_DC_Set();
LCD_Write_Bus(0xD0);
LCD_Write_Bus(0x04);
LCD_Write_Bus(0x0C);
LCD_Write_Bus(0x11);
LCD_Write_Bus(0x13);
LCD_Write_Bus(0x2C);
LCD_Write_Bus(0x3F);
LCD_Write_Bus(0x44);
LCD_Write_Bus(0x51);
LCD_Write_Bus(0x2F);
LCD_Write_Bus(0x1F);
LCD_Write_Bus(0x1F);
LCD_Write_Bus(0x20);
LCD_Write_Bus(0x23);
LCD_WaitBSY();

LCD_WR_REG(0x21);
LCD_WR_REG(0x11); 
LCD_WR_REG(0x29);
} 

void LCD_Clear(u16 Color)
{
	Address_set(0,0,LCD_W-1,LCD_H-1);
	OLED_DC_Set();
    for(uint32_t i = 0;i < LCD_W * LCD_H;i++)
	{
		LCD_Write_Bus(Color >> 8);
		LCD_Write_Bus(Color);
	}
	LCD_WaitBSY();
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
	LCD_WaitBSY();
}

void LCD_DrawPoint(u16 x,u16 y)
{
	Address_set(x,y,x,y);
	LCD_WR_DATA16(POINT_COLOR);
}

void LCD_DrawPoint_big(u16 x,u16 y)
{
	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
}

//  (xend-xsta)*(yend-ysta)
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{
	uint32_t sz = (uint32_t)(yend - ysta + 1) * (uint32_t)(xend - xsta + 1);
	Address_set(xsta,ysta,xend,yend);
	OLED_DC_Set();
	for(uint32_t i = 0; i< sz; i++)
	{
		LCD_Write_Bus(color >> 8);
		LCD_Write_Bus(color);// (color & 0xFF)
	}
	LCD_WaitBSY();
}

void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
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
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, u16 x2, uint16_t y2)
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

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode)
{
    u8 temp;
    u8 pos,t;
	u16 x0=x;
	u16 colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return; 
	num=num-' ';
	Address_set(x,y,x+8-1,y+16-1);
	if(!mode)
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(u16)num*16+pos];
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
		    temp=asc2_1608[(u16)num*16+pos];
			for(t=0;t<8;t++)
		    {
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);
		        temp>>=1;
		    }
		}
	}
	POINT_COLOR=colortemp;	   	 	  
}

u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
	
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len)
{         	
	u8 t,temp;
	u8 enshow=0;
	num=(u16)num;
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

void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 
	}
} 

void LCD_ShowString(u16 x,u16 y,const u8 *p)
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

