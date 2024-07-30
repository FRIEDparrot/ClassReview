/**************************************************************************************************/		
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#ifndef __ILI9341_LCD_H
#define __ILI9341_LCD_H
#include "sys.h"	 
#include "stdlib.h"
#include "stm32f4xx_hal.h"

typedef struct
{										    
	uint16_t width;			
	uint16_t height;		
	uint16_t id;			
	uint8_t  dir;
	uint16_t  wramcmd;	
	uint16_t  setxcmd;
	uint16_t  setycmd;		
}_lcd_dev;

extern _lcd_dev lcddev;
extern uint16_t  POINT_COLOR;
extern uint16_t  BACK_COLOR;

#define USE_HORIZONTAL  	 0  // use 0-4 for 4 directions show 

#define LCD_W 240
#define LCD_H 320

#define LCD_SPI_BUFFER_DIV              10
#define LCD_SPI_BUFFER_SIZE             2 * LCD_W * LCD_H / LCD_SPI_BUFFER_DIV
#define ILI9341_TIMEOUT 1000

/***********************Only change the following definition when change ********************/

#define ILI9341_LCD_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define ILI9341_LCD_SPI_CLK_ENABLE()        __HAL_RCC_SPI2_CLK_ENABLE()

/** Note : put all the LCD relevant pins (7 pins above) in 1 GPIO port for convience */
#define ILI9341_LCD_GPIO_PORT               GPIOB
#define ILI9341_LCD_SPI_AF                  GPIO_AF5_SPI2
#define ILI9341_LCD_SPI                     SPI2

#define ILI9341_GPIO_PIN_CS   GPIO_PIN_7    // CS : PB7
#define ILI9341_GPIO_PIN_DC   GPIO_PIN_8    // DC : PB8
#define	ILI9341_GPIO_PIN_LED  GPIO_PIN_9    // LED: PB9
#define ILI9341_GPIO_PIN_RST  GPIO_PIN_12	  // RST: PB12 
#define ILI9341_GPIO_PIN_SCL  GPIO_PIN_13   // SCL: PB13
#define ILI9341_GPIO_PIN_MISO GPIO_PIN_14   // SDO: PB14
#define ILI9341_GPIO_PIN_MOSI GPIO_PIN_15   // SDI: PB15

/***********************Only change the following definition when change Pin used  **********/

#define ILI9341_LED_SET() 	HAL_GPIO_WritePin(ILI9341_LCD_GPIO_PORT, ILI9341_GPIO_PIN_LED, GPIO_PIN_SET) // PB9
#define	ILI9341_DC_SET()	HAL_GPIO_WritePin(ILI9341_LCD_GPIO_PORT  , ILI9341_GPIO_PIN_DC,  GPIO_PIN_SET) // PB10
#define	ILI9341_CS_SET()  	HAL_GPIO_WritePin(ILI9341_LCD_GPIO_PORT, ILI9341_GPIO_PIN_CS,  GPIO_PIN_SET) // PB11
#define	ILI9341_RST_SET()	HAL_GPIO_WritePin(ILI9341_LCD_GPIO_PORT  , ILI9341_GPIO_PIN_RST, GPIO_PIN_SET) // PB12

#define ILI9341_LED_CLR() 	HAL_GPIO_WritePin(ILI9341_LCD_GPIO_PORT, ILI9341_GPIO_PIN_LED, GPIO_PIN_RESET) // PB9
#define	ILI9341_DC_CLR()	HAL_GPIO_WritePin(ILI9341_LCD_GPIO_PORT  , ILI9341_GPIO_PIN_DC,  GPIO_PIN_RESET) // PB10
#define	ILI9341_CS_CLR()  	HAL_GPIO_WritePin(ILI9341_LCD_GPIO_PORT, ILI9341_GPIO_PIN_CS,  GPIO_PIN_RESET) // PB11
#define	ILI9341_RST_CLR()	HAL_GPIO_WritePin(ILI9341_LCD_GPIO_PORT  , ILI9341_GPIO_PIN_RST, GPIO_PIN_RESET) // PB12

/****************** color definitions ***************************/ 
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 		0XFFE0
#define GBLUE		0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 		0XBC40
#define BRRED 		0XFC07 
#define GRAY  		0X8430 

#define DARKBLUE      	 0X01CF
#define LIGHTBLUE      	 0X7D7C
#define GRAYBLUE       	 0X5458
 
#define LIGHTGREEN     	0X841F
#define LIGHTGRAY     	0XEF5B
#define LGRAY 			 	

#define LGRAYBLUE      	0XA651
#define LBBLUE          0X2B12

void ILI9341_Init(void);
void ILI9341_DisplayOn(void);
void ILI9341_DisplayOff(void);
void ILI9341_Clear(uint16_t Color);
void ILI9341_SetCursor(uint16_t Xpos, uint16_t Ypos);
void ILI9341_WriteData_16Bit(uint16_t Data);
void ILI9341_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);
void ILI9341_Set_direction(uint8_t direction );
void ILI9341_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);

// void __LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
// void __LCD_WR_DATA(uint8_t data);
// void __LCD_WriteRAM_Prepare(void);
// void LCD_SetParam(void);

/** Read Function  */

// uint16_t LCD_RD_DATA(void);
// uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y);    // used for hand recognition  
// uint16_t LCD_ReadReg(uint8_t LCD_Reg);

// void LCD_WriteRAM(uint16_t RGB_Code);
// uint16_t LCD_ReadRAM(void);		   
// uint16_t LCD_BGR2RGB(uint16_t c);



#endif  
