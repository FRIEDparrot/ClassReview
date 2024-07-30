#include "ILI9341_lcd.h"
#include "stdlib.h"

_lcd_dev lcddev;

uint16_t POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
uint16_t DeviceCode;

static SPI_HandleTypeDef lcd_spi_handler;
static uint8_t lcd_buffer[LCD_SPI_BUFFER_SIZE];    // buffer for the 

/******************* private function declaration ****************************/
static void __ILI9341_LCD_GPIO_Init(void);
static void __ILI9341_LCD_SPI_Init(void);
static void __ILI9341_LCD_SPI_SetSpeed(SPI_TypeDef* SPIx,uint8_t baud_psc);
static void __LCD_WR_REG(uint8_t data);
static void __LCD_WR_DATA(uint8_t data);
static void __LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);

/*****************************************************************************
 * @name       :void ILI9341__LCD_GPIO_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen GPIO
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
static void __ILI9341_LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	ILI9341_LCD_GPIO_CLK_ENABLE();
	
  	/**  Init SCK, MISO and MOSI GPIO */
	GPIO_InitStructure.Pin		= ILI9341_GPIO_PIN_SCL|ILI9341_GPIO_PIN_MISO|ILI9341_GPIO_PIN_MOSI;
	GPIO_InitStructure.Mode 	= GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull  	= GPIO_PULLUP;
	GPIO_InitStructure.Speed 	= GPIO_SPEED_FAST;
	GPIO_InitStructure.Alternate= ILI9341_LCD_SPI_AF;
	HAL_GPIO_Init(ILI9341_LCD_GPIO_PORT, &GPIO_InitStructure);
	
	/** Init General GPIO (DC, CS, LED, RST) */
	GPIO_InitStructure.Pin 		=  ILI9341_GPIO_PIN_DC| ILI9341_GPIO_PIN_CS | ILI9341_GPIO_PIN_LED| ILI9341_GPIO_PIN_RST;
	GPIO_InitStructure.Mode 	= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed 	= GPIO_SPEED_FAST;
	GPIO_InitStructure.Pull 	= GPIO_PULLUP;
	GPIO_InitStructure.Alternate= 0;
	
	HAL_GPIO_Init(ILI9341_LCD_GPIO_PORT, &GPIO_InitStructure);
}


/*****************************************************************************
 * @name       :void __ILI9341_LCD_SPI_Init(void)	
 * @date       :2018-08-09 
 * @function   :Initialize the STM32 hardware SPI 
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
static void __ILI9341_LCD_SPI_Init(void)
{	
	ILI9341_LCD_SPI_CLK_ENABLE();

	lcd_spi_handler.Instance 			= ILI9341_LCD_SPI;
	lcd_spi_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	lcd_spi_handler.Init.CLKPhase 		= SPI_PHASE_1EDGE;
	lcd_spi_handler.Init.CLKPolarity 	= SPI_POLARITY_LOW;
	lcd_spi_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	lcd_spi_handler.Init.CRCPolynomial  = 7;
	lcd_spi_handler.Init.DataSize 		= SPI_DATASIZE_8BIT;
	lcd_spi_handler.Init.Direction		= SPI_DIRECTION_2LINES;
	lcd_spi_handler.Init.FirstBit		= SPI_FIRSTBIT_MSB;
	lcd_spi_handler.Init.Mode			= SPI_MODE_MASTER;
	lcd_spi_handler.Init.NSS 			= SPI_NSS_SOFT;
	lcd_spi_handler.Init.TIMode			= SPI_TIMODE_DISABLE;
	HAL_SPI_DeInit(&lcd_spi_handler);
	HAL_SPI_Init(&lcd_spi_handler);

	__HAL_SPI_ENABLE(&lcd_spi_handler);  /** enabel the spi handler */
}

/*****************************************************************************
 * @name       :void SPI_SetSpeed(SPI_TypeDef* SPIx,uint8_t SpeedSet)
 * @date       :2018-08-09 
 * @function   :Set hardware SPI Speed
 * @parameters :SPIx: SPI type,x for 1,2,3
                SpeedSet:0-high speed
												 1-low speed
 * @retvalue   :None
******************************************************************************/
static void __ILI9341_LCD_SPI_SetSpeed(SPI_TypeDef* SPIx,uint8_t baud_psc)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(baud_psc));
	__HAL_SPI_DISABLE(&lcd_spi_handler);
	lcd_spi_handler.Instance->CR1 &= 0XFFC7;         /* clear bit 3-5 for set baudrate */ 
    lcd_spi_handler.Instance->CR1 |= baud_psc << 3;  /* set SPI speed */
	__HAL_SPI_ENABLE(&lcd_spi_handler);
}

/*****************************************************************************
 * @name       :void __LCD_WR_REG(uint8_t data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
static void __LCD_WR_REG(uint8_t data)
{
   ILI9341_CS_CLR();
   ILI9341_DC_CLR(); /* enable the chip select */
   HAL_SPI_Transmit(&lcd_spi_handler, &data, 1,  ILI9341_TIMEOUT);
   ILI9341_CS_SET();
}

/*****************************************************************************
 * @name       :void __LCD_WR_DATA(uint8_t data)
 * @date       :2018-08-09 
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
static void __LCD_WR_DATA(uint8_t data)
{
	ILI9341_CS_CLR();
	ILI9341_DC_SET();
	HAL_SPI_Transmit(&lcd_spi_handler, &data, 1,  ILI9341_TIMEOUT);
	ILI9341_CS_SET();
}

/*****************************************************************************
 * @name       :void __LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
 * @date       :2018-08-09 
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void __LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{	
	__LCD_WR_REG(LCD_Reg);  
	__LCD_WR_DATA(LCD_RegValue);	    		 
}

/*****************************************************************************
 * @name       :void __LCD_WriteRAM_Prepare(void)
 * @date       :2018-08-09 
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 
void __LCD_WriteRAM_Prepare(void)
{
	__LCD_WR_REG(lcddev.wramcmd);
}

/*****************************************************************************
 * @name       :void ILI9341_WriteData_16Bit(uint16_t Data)
 * @date       :2018-08-09 
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/	 
void ILI9341_WriteData_16Bit(uint16_t Data)
{	
	ILI9341_CS_CLR();
	ILI9341_DC_SET();
	uint8_t dat[2] = {Data >> 8,  Data & 0xff};
	HAL_SPI_Transmit(&lcd_spi_handler, dat, 2,  ILI9341_TIMEOUT);
	ILI9341_CS_SET();
}



/*****************************************************************************
 * @name       :void ILI9341_Clear(uint16_t Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/	
void ILI9341_Clear(uint16_t Color)
{
	ILI9341_SetWindows(0,0,lcddev.width-1,lcddev.height-1);
	ILI9341_CS_CLR();
	ILI9341_DC_SET();
	
	uint8_t ch = Color >> 8, cl = Color|0xff;
	
	/** set the array of the buffer ----------------- */
	for (int i = 0; i < LCD_SPI_BUFFER_SIZE; i += 2){
		lcd_buffer[i] = ch;
		lcd_buffer[i + 1] = cl;
	}

	for(int i=0; i < LCD_SPI_BUFFER_DIV; i++)
	{
		HAL_SPI_Transmit(&lcd_spi_handler, lcd_buffer, LCD_SPI_BUFFER_SIZE, ILI9341_TIMEOUT);
	}
	ILI9341_CS_SET();
}

/*****************************************************************************
 * @name       :void ILI9341_RESET(void)
 * @date       :2018-08-09 
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void ILI9341_RESET(void)
{
	ILI9341_RST_CLR();
	HAL_Delay(100);
	ILI9341_RST_SET();
	HAL_Delay(50);
}

/*****************************************************************************
 * @name       :void ILI9341_Init (void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	 	 
void ILI9341_Init(void)
{
	__ILI9341_LCD_GPIO_Init();
	__ILI9341_LCD_SPI_Init();
 	ILI9341_RESET();

//*************2.8inch ILI9341_DrawPoint screen start sequence **********//	
	__LCD_WR_REG(0xCF);
	__LCD_WR_DATA(0x00); 
	__LCD_WR_DATA(0xC9); // C1 
	__LCD_WR_DATA(0X30); 
	__LCD_WR_REG(0xED);  
	__LCD_WR_DATA(0x64); 
	__LCD_WR_DATA(0x03); 
	__LCD_WR_DATA(0X12); 
	__LCD_WR_DATA(0X81); 
	__LCD_WR_REG(0xE8);  
	__LCD_WR_DATA(0x85); 
	__LCD_WR_DATA(0x10); 
	__LCD_WR_DATA(0x7A); 
	__LCD_WR_REG(0xCB);  
	__LCD_WR_DATA(0x39); 
	__LCD_WR_DATA(0x2C); 
	__LCD_WR_DATA(0x00); 
	__LCD_WR_DATA(0x34); 
	__LCD_WR_DATA(0x02); 
	__LCD_WR_REG(0xF7);  
	__LCD_WR_DATA(0x20); 
	__LCD_WR_REG(0xEA);  
	__LCD_WR_DATA(0x00); 
	__LCD_WR_DATA(0x00); 
	__LCD_WR_REG(0xC0);		// Power control 
	__LCD_WR_DATA(0x1B);   	// VRH[5:0] 
	__LCD_WR_REG(0xC1);    	// Power control 
	__LCD_WR_DATA(0x00);   	// SAP[2:0];BT[3:0] 01 
	__LCD_WR_REG(0xC5);    	// VCM control 
	__LCD_WR_DATA(0x30); 	// 3F
	__LCD_WR_DATA(0x30); 	// 3C
	__LCD_WR_REG(0xC7);    	// VCM control2 
	__LCD_WR_DATA(0XB7); 
	__LCD_WR_REG(0x36);    	// Memory Access Control 
	__LCD_WR_DATA(0x08); 
	__LCD_WR_REG(0x3A);   
	__LCD_WR_DATA(0x55); 
	__LCD_WR_REG(0xB1);   
	__LCD_WR_DATA(0x00);   
	__LCD_WR_DATA(0x1A); 
	__LCD_WR_REG(0xB6);    	// Display Function Control 
	__LCD_WR_DATA(0x0A); 
	__LCD_WR_DATA(0xA2); 
	__LCD_WR_REG(0xF2);    	// 3Gamma Function Disable 
	__LCD_WR_DATA(0x00); 
	__LCD_WR_REG(0x26);    	// Gamma curve selected 
	__LCD_WR_DATA(0x01); 
	__LCD_WR_REG(0xE0);    	// Set Gamma 
	__LCD_WR_DATA(0x0F); 
	__LCD_WR_DATA(0x2A); 
	__LCD_WR_DATA(0x28); 
	__LCD_WR_DATA(0x08); 
	__LCD_WR_DATA(0x0E); 
	__LCD_WR_DATA(0x08); 
	__LCD_WR_DATA(0x54); 
	__LCD_WR_DATA(0XA9); 
	__LCD_WR_DATA(0x43); 
	__LCD_WR_DATA(0x0A); 
	__LCD_WR_DATA(0x0F); 
	__LCD_WR_DATA(0x00); 
	__LCD_WR_DATA(0x00); 
	__LCD_WR_DATA(0x00); 
	__LCD_WR_DATA(0x00); 		 
	__LCD_WR_REG(0XE1);    	// Set Gamma 
	__LCD_WR_DATA(0x00); 
	__LCD_WR_DATA(0x15); 
	__LCD_WR_DATA(0x17); 
	__LCD_WR_DATA(0x07); 
	__LCD_WR_DATA(0x11); 
	__LCD_WR_DATA(0x06); 
	__LCD_WR_DATA(0x2B); 
	__LCD_WR_DATA(0x56); 
	__LCD_WR_DATA(0x3C); 
	__LCD_WR_DATA(0x05); 
	__LCD_WR_DATA(0x10); 
	__LCD_WR_DATA(0x0F); 
	__LCD_WR_DATA(0x3F); 
	__LCD_WR_DATA(0x3F); 
	__LCD_WR_DATA(0x0F); 
	__LCD_WR_REG(0x2B); 
	__LCD_WR_DATA(0x00);
	__LCD_WR_DATA(0x00);
	__LCD_WR_DATA(0x01);
	__LCD_WR_DATA(0x3f);
	__LCD_WR_REG(0x2A); 
	__LCD_WR_DATA(0x00);
	__LCD_WR_DATA(0x00);
	__LCD_WR_DATA(0x00);
	__LCD_WR_DATA(0xef);	 
	__LCD_WR_REG(0x11); 	// Exit Sleep
	HAL_Delay(120);
	__LCD_WR_REG(0x29); 	// display on		

  	ILI9341_Set_direction(USE_HORIZONTAL);  
	ILI9341_LED_SET();
	ILI9341_Clear(WHITE);   // clear the screen 
}

/*****************************************************************************
 * @name       :void ILI9341_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
 * @date       :2018-08-09 
 * @function   :Setting LCD display window
 * @parameters :xStar:the bebinning x coordinate of the LCD display window
								yStar:the bebinning y coordinate of the LCD display window
								xEnd:the endning x coordinate of the LCD display window
								yEnd:the endning y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/ 
void ILI9341_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{	
	__LCD_WR_REG(lcddev.setxcmd);	
	__LCD_WR_DATA(xStar>>8);
	__LCD_WR_DATA(0x00FF&xStar);		
	__LCD_WR_DATA(xEnd>>8);
	__LCD_WR_DATA(0x00FF&xEnd);

	__LCD_WR_REG(lcddev.setycmd);	
	__LCD_WR_DATA(yStar>>8);
	__LCD_WR_DATA(0x00FF&yStar);		
	__LCD_WR_DATA(yEnd>>8);
	__LCD_WR_DATA(0x00FF&yEnd);

	__LCD_WriteRAM_Prepare();  // prepare for write RAM
}

/*****************************************************************************
 * @name       :void ILI9341_SetCursor(uint16_t Xpos, uint16_t Ypos)
 * @date       :2018-08-09 
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/ 
void ILI9341_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	  	    			
	ILI9341_SetWindows(Xpos,Ypos,Xpos,Ypos);	
} 

/// @name ILI9341_Set_direction(uint8_t direction)
/// @brief Setting the display direction of LCD screen
/// @date       :2018-08-09 
/// @param direction 0-0 degree
/// 				 1-90 degree
/// 				 2-180 degree
/// 				 3-270 degree
/// @return	None
void ILI9341_Set_direction(uint8_t direction)
{
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	lcddev.wramcmd=0x2C;
	switch(direction){		  
		case 0:						 	 		
			lcddev.width =LCD_W;
			lcddev.height=LCD_H;		
			__LCD_WriteReg(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 1:
			lcddev.width =LCD_H;
			lcddev.height=LCD_W;
			__LCD_WriteReg(0x36,(1<<3)|(0<<7)|(1<<6)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;
		case 2:						 	 		
			lcddev.width=LCD_W;
			lcddev.height=LCD_H;	
			__LCD_WriteReg(0x36,(1<<3)|(1<<6)|(1<<7));//BGR==1,MY==0,MX==0,MV==0
		break;
		case 3:
			lcddev.width=LCD_H;
			lcddev.height=LCD_W;
			__LCD_WriteReg(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
		break;	
		default:break;
	}		
}	 

/*******************************************************************
 * @name       :void ILI9341_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
 * @date       :2018-08-09 
 * @function   :fill the specified area
 * @parameters :sx:the bebinning x coordinate of the specified area
                sy:the bebinning y coordinate of the specified area
								ex:the ending x coordinate of the specified area
								ey:the ending y coordinate of the specified area
								color:the filled color value
 * @retvalue   :None
 * optimized by @FriedParrot at 2024-07-24 for faster speed 
********************************************************************/
void ILI9341_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{
	uint16_t width=ex-sx+1; 		// get the fill width
	uint16_t height=ey-sy+1;		// get the fill height
	ILI9341_SetWindows(sx,sy,ex,ey);    // set the windows 
	
	uint32_t nums = height * width * 2;  // get the number of pixels
	uint16_t tmp = nums > LCD_SPI_BUFFER_SIZE ? LCD_SPI_BUFFER_SIZE : nums;
	
	// fill the buffer with color
	for (int i = 0; i < tmp; i+=2) {
		lcd_buffer[i] = (color >> 8);
		lcd_buffer[i+1] = (color & 0xFF);
	}

	while (nums > 0) {
		ILI9341_CS_CLR();
		ILI9341_DC_SET();
		uint16_t fillnum = nums > LCD_SPI_BUFFER_SIZE ? LCD_SPI_BUFFER_SIZE : nums;
		HAL_SPI_Transmit(&lcd_spi_handler, lcd_buffer, fillnum, ILI9341_TIMEOUT);
		nums -= fillnum;
		ILI9341_CS_SET();
	}
	ILI9341_SetWindows(0,0,lcddev.width-1,lcddev.height-1); // restore setting to the whole screen
}

