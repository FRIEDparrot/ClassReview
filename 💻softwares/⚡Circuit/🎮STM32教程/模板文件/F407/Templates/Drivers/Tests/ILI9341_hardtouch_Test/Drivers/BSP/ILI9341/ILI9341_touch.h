/**************************************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/

#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "ILI9341_lcd.h"

#define TP_PRES_DOWN 0x80   // touch screen is pressed 
#define TP_CATH_PRES 0x40   // catch touch screen pressed first 

/** touch screen controller */
typedef struct
{
    uint8_t (*init)(void);
    uint8_t (*scan)(uint8_t);
    void (*adjust)(void);	
    uint16_t x0;			
    uint16_t y0;
    uint16_t x; 			
    uint16_t y;				
    uint8_t  sta; 

    /** touch pad calibration parameters ------ */
    float xfac;					
    float yfac;
    short xoff;
    short yoff;

    // when touchtype = 0: left-> right is x, up-> down is y
    // else touchtype = 1: left-> right is y, up-> down is x
    uint8_t touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;    /* define  the tp_dev in */

/* GPIO Pin Operation Definitions */
#define ILI9341_TOUCH_GPIO_CLK_ENABLE()  	  __HAL_RCC_GPIOA_CLK_ENABLE()
// #define ILI9341_TOUCH_GPIO_CLK_DISABLE()  	__HAL_RCC_GPIOA_CLK_DISABLE()

#define ILI9341_TOUCH_SPI_CLK_ENABLE()  	__HAL_RCC_SPI1_CLK_ENABLE()
// #define ILI9341_TOUCH_SPI_CLK_DISABLE()  	__HAL_RCC_SPI1_CLK_DISABLE()

#define ILI9341_TOUCH_SPI					    SPI1
#define ILI9341_TOUCH_GPIO_PORT				GPIOA
#define ILI9341_TOUCH_GPIO_AF         GPIO_AF5_SPI1

#define ILI9341_GPIO_PIN_TCS    GPIO_PIN_4  // T_CS : PA4
#define ILI9341_GPIO_PIN_TCLK   GPIO_PIN_5  // T_CLK: PA5
#define ILI9341_GPIO_PIN_TDO    GPIO_PIN_6  // T_DO : PA6
#define ILI9341_GPIO_PIN_TDI    GPIO_PIN_7  // T_DI : PA7
#define ILI9341_GPIO_PIN_TIRQ   GPIO_PIN_8  // T_IRQ: PA8 (used fo  external interrupt)

#define ILI9341_TCS_SET()       HAL_GPIO_WritePin(ILI9341_TOUCH_GPIO_PORT, ILI9341_GPIO_PIN_TCS, GPIO_PIN_SET)
#define ILI9341_TCLK_SET()      HAL_GPIO_WritePin(ILI9341_TOUCH_GPIO_PORT, ILI9341_GPIO_PIN_TCLK, GPIO_PIN_SET)
#define ILI9341_TDO_SET()       HAL_GPIO_WritePin(ILI9341_TOUCH_GPIO_PORT, ILI9341_GPIO_PIN_TDO, GPIO_PIN_SET)
#define ILI9341_TDI_SET()       HAL_GPIO_WritePin(ILI9341_TOUCH_GPIO_PORT, ILI9341_GPIO_PIN_TDI, GPIO_PIN_SET)

#define ILI9341_TCS_RESET()     HAL_GPIO_WritePin(ILI9341_TOUCH_GPIO_PORT, ILI9341_GPIO_PIN_TCS, GPIO_PIN_RESET)
#define ILI9341_TCLK_RESET()    HAL_GPIO_WritePin(ILI9341_TOUCH_GPIO_PORT, ILI9341_GPIO_PIN_TCLK, GPIO_PIN_RESET)
#define ILI9341_TDO_RESET()     HAL_GPIO_WritePin(ILI9341_TOUCH_GPIO_PORT, ILI9341_GPIO_PIN_TDO, GPIO_PIN_RESET)
#define ILI9341_TDI_RESET()     HAL_GPIO_WritePin(ILI9341_TOUCH_GPIO_PORT, ILI9341_GPIO_PIN_TDI, GPIO_PIN_RESET)

#define ILI9341_GET_TIRQ HAL_GPIO_ReadPin(ILI9341_TOUCH_GPIO_PORT, ILI9341_GPIO_PIN_TIRQ)
#define ILI9341_GET_TDO  HAL_GPIO_ReadPin(ILI9341_TOUCH_GPIO_PORT, ILI9341_GPIO_PIN_TDO)

// void TP_Write_Byte(uint8_t num);
// uint16_t TP_Read_AD(uint8_t CMD);
uint16_t TP_Read_Coor(uint8_t xy);
uint8_t TP_Read_XY(uint16_t *x,uint16_t *y);
uint8_t TP_Read_XY2(uint16_t *x,uint16_t *y);
void TP_Drow_Touch_Point(uint16_t x,uint16_t y,uint16_t color);
void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color);
uint8_t TP_Scan(uint8_t tp);
void TP_Adjust(void);
uint8_t TP_Init(void);

// void TP_Save_Adjdata(void);
// uint8_t TP_Get_Adjdata(void);

void TP_Adj_Info_Show(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t fac);

#endif
