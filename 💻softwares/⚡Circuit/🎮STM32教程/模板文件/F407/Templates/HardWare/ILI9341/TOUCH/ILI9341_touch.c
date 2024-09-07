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
#include "ILI9341_lcd.h"
#include "ILI9341_touch.h"
#include "stdlib.h"
#include "math.h"
#include "delay.h"
#include "ILI9341_GUI.h"

#include  <stdio.h>

uint8_t CMD_RDX=0XD0;   // command to read x coordinate
uint8_t CMD_RDY=0X90;   // command to read y coordinate

/** Initialize the global private variables ------------------------------- */ 
static SPI_HandleTypeDef touch_spi_handler;

static _m_tp_dev tp_dev =
{
    TP_Init,       /* Init function */
    TP_Scan,       /* Scan function */
    TP_Adjust,     /* Adjust function */
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,	  	 		
    0,
    0,	  	 		
};					

/*****************************************************************************
 * @name       :uint16_t TP_Read_Coor(uint8_t xy)  
 * @date       :2018-08-09 
 * @function   :Read the touch screen coordinates (x or y),
                                Read the READ_TIMES secondary data in succession 
                                and sort the data in ascending order,
                                Then remove the lowest and highest number of LOST_VAL 
                                and take the average
 * @parameters :xy:Read command(CMD_RDX/CMD_RDY)
 * @retvalue   :Read data
******************************************************************************/  
uint16_t TP_Read_Coor(uint8_t cmd)
{
    /** read 6 times, throw the maximum and minimum, return the average of others */
    uint16_t buf[6];
    uint16_t sum=0;
    uint16_t maxval, minval;

    /*** Change to Hardware SPI read later */
    uint8_t  rx_buf[2] = {0};
    
    for(uint16_t i=0; i< 6; i++){   // read 6 times data
        ILI9341_TCS_RESET(); // enable the chip selection
        delay_us(1);
        HAL_SPI_Transmit(&touch_spi_handler, &cmd, 1, ILI9341_TIMEOUT);
        delay_us(6);
        HAL_SPI_Receive(&touch_spi_handler, rx_buf, 2, ILI9341_TIMEOUT);
        delay_us(1);
        ILI9341_TCS_SET();
        buf[i] = ((uint16_t)rx_buf[0] << 8) | rx_buf[1];
        // only use higher 12 bits , buf[0] is first 7 bytes and buf1 is 8-12 bytes
        buf[i] >>= 3;
    }
    
    ILI9341_TCS_RESET();
    HAL_SPI_Transmit(&touch_spi_handler,&cmd,1, ILI9341_TIMEOUT);
    ILI9341_TCS_SET();
    
    maxval = buf[0]; minval = buf[0];
    for (uint16_t i=0; i < 6; i++)
    {
        if (buf[i] > maxval) maxval = buf[i];
        else if (buf[i] < minval) minval = buf[i];
        sum += buf[i];
    }
    sum -= (maxval + minval); 
    return (sum >> 2);
}

/*****************************************************************************
 * @name       :uint8_t TP_Read_XY(uint16_t *x,uint16_t *y)
 * @date       :2018-08-09 
 * @function   :Read touch screen x and y coordinates,
                                The minimum value can not be less than 100
 * @parameters :x:Read x coordinate of the touch screen
                                y:Read y coordinate of the touch screen
 * @retvalue   :0-fail,1-success
******************************************************************************/ 
uint8_t TP_Read_XY(uint16_t *x,uint16_t *y)
{   
    (*x) = TP_Read_Coor(CMD_RDX);
    (*y) = TP_Read_Coor(CMD_RDY);
    return 1;
}

#define ERR_RANGE 50 // error range  
/*****************************************************************************
 * @name       :uint8_t TP_Read_XY2(uint16_t *x,uint16_t *y) 
 * @date       :2018-08-09 
 * @function   :Read the touch screen coordinates twice in a row, 
                                and the deviation of these two times can not exceed ERR_RANGE, 
                                satisfy the condition, then think the reading is correct, 
                                otherwise the reading is wrong.
                                This function can greatly improve the accuracy.
 * @parameters :x:Read x coordinate of the touch screen
                y:Read y coordinate of the touch screen 
 * @retvalue   :0-fail,1-success
******************************************************************************/ 
uint8_t TP_Read_XY2(uint16_t *x,uint16_t *y) 
{
    uint16_t x1,x2,y1, y2;
    uint8_t flag;

    flag=TP_Read_XY(&x1,&y1);
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))&&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }
    return 0;
} 

/*****************************************************************************
 * @name       :void TP_Drow_Touch_Point(uint16_t x,uint16_t y,uint16_t color)
 * @date       :2018-08-09 
 * @function   :Draw a touch point,Used to calibrate							
 * @parameters :x:Read x coordinate of the touch screen
                                y:Read y coordinate of the touch screen
                                color:the color value of the touch point
 * @retvalue   :None
******************************************************************************/  
void TP_Drow_Touch_Point(uint16_t x,uint16_t y,uint16_t color)
{
    POINT_COLOR=color;
    ILI9341_DrawLine(x-12,y,x+13,y); // Draw target line 
    ILI9341_DrawLine(x,y-12,x,y+13); 
    ILI9341_DrawPoint(x+1,y+1);
    ILI9341_DrawPoint(x-1,y+1);
    ILI9341_DrawPoint(x+1,y-1);
    ILI9341_DrawPoint(x-1,y-1);
    ILI9341_DrawCircle(x,y,POINT_COLOR,6,0); 
}	

/*****************************************************************************
 * @name       :void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color)
 * @date       :2018-08-09 
 * @function   :Draw a big point(2*2)					
 * @parameters :x:Read x coordinate of the point
                                y:Read y coordinate of the point
                                color:the color value of the point
 * @retvalue   :None
******************************************************************************/   
void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color)
{
    POINT_COLOR=color;
    ILI9341_DrawPoint(x,y); 
    ILI9341_DrawPoint(x+1,y);
    ILI9341_DrawPoint(x,y+1);
    ILI9341_DrawPoint(x+1,y+1);	 	  	
}	

/*****************************************************************************
 * @name       :uint8_t TP_Scan(uint8_t tp)
 * @date       :2018-08-09 
 * @function   :Scanning touch event				
 * @parameters :tp:0-screen coordinate 
                                     1-Physical coordinates(For special occasions such as calibration)
 * @retvalue   :Current touch screen status,
                                0-no touch
                                1-touch
******************************************************************************/  					  
uint8_t TP_Scan(uint8_t tp)
{
    if(ILI9341_GET_TIRQ == 0)  // detect touch screen is pressed down 
    {
        if(tp) TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        else if(TP_Read_XY2(&tp_dev.x,&tp_dev.y))
        {
            tp_dev.x=tp_dev.xfac*tp_dev.x+tp_dev.xoff;
            tp_dev.y=tp_dev.yfac*tp_dev.y+tp_dev.yoff;  
        }

        /*** when it not pressed down before, set state to caught */
        if((tp_dev.sta & TP_PRES_DOWN)==0)
        {
            tp_dev.sta |= TP_CATH_PRES;
        }
        tp_dev.sta |= TP_PRES_DOWN;
        tp_dev.x0=tp_dev.x;
        tp_dev.y0=tp_dev.y;
    }
    else // reset the touch status 
    {
        if(tp_dev.sta&TP_PRES_DOWN)
        {
            tp_dev.sta&=~(1<<7);
        }
        else
        {
            tp_dev.x0=0;
            tp_dev.y0=0;
            tp_dev.x=0xffff;
            tp_dev.y=0xffff;
        }
    }
    return tp_dev.sta & TP_PRES_DOWN;  // if the tpd is pressed down, return 1 
}


//////////////////////////////////////////////////////////////////////////
/*****************************************************************************
 * @name       :void TP_Save_Adjdata(void)
 * @date       :2018-08-09 
 * @function   :Save TFTLCD calibration parameter 
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 										    
// void TP_Save_Adjdata(void)
// {
// 	s32 temp;
// 	temp=tp_dev.xfac*100000000; 
//     AT24CXX_WriteLenByte(SAVE_ADDR_BASE,temp,4);   
// 	temp=tp_dev.yfac*100000000; 
//     AT24CXX_WriteLenByte(SAVE_ADDR_BASE+4,temp,4);
//     AT24CXX_WriteLenByte(SAVE_ADDR_BASE+8,tp_dev.xoff,2);		    
// 	AT24CXX_WriteLenByte(SAVE_ADDR_BASE+10,tp_dev.yoff,2);	
// 	AT24CXX_WriteOneByte(SAVE_ADDR_BASE+12,tp_dev.touchtype);
// 	AT24CXX_WriteOneByte(SAVE_ADDR_BASE+13,temp); 
// }

/*****************************************************************************
 * @name       :uint8_t TP_Get_Adjdata(void)
 * @date       :2018-08-09 
 * @function   :Gets the calibration values stored in the EEPROM		
 * @parameters :None
 * @retvalue   :1-get the calibration values successfully
                                0-get the calibration values unsuccessfully and Need to recalibrate
******************************************************************************/ 	
// uint8_t TP_Get_Adjdata(void)
// {					  
// 	s32 tempfac;
// 	tempfac=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+13);
// 	if(tempfac==0X0A)		   
// 	{    												 
// 		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE,4);		   
// 		tp_dev.xfac=(float)tempfac/100000000;//�õ�xУ׼����
// 		tempfac=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+4,4);			          
// 		tp_dev.yfac=(float)tempfac/100000000;//�õ�yУ׼����
// 	    //�õ�xƫ����
// 		tp_dev.xoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+8,2);			   	  
//  	    //�õ�yƫ����
// 		tp_dev.yoff=AT24CXX_ReadLenByte(SAVE_ADDR_BASE+10,2);				 	  
//  		tp_dev.touchtype=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);//��ȡ�������ͱ��?
// 		if(tp_dev.touchtype)//X,Y��������Ļ�෴
// 		{
// 			CMD_RDX=0X90;
// 			CMD_RDY=0XD0;	 
// 		}else				   //X,Y��������Ļ��ͬ
// 		{
// 			CMD_RDX=0XD0;
// 			CMD_RDY=0X90;	 
// 		}		 
// 		return 1;	 
// 	}
// 	return 0;
// }


const uint8_t* TP_REMIND_MSG_TBL="Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";

/*****************************************************************************
 * @name       :void TP_Adj_Info_Show(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t fac)
 * @date       :2018-08-09 
 * @function   :Display calibration results	
 * @parameters :x0:the x coordinates of first calibration point
                                y0:the y coordinates of first calibration point
                                x1:the x coordinates of second calibration point
                                y1:the y coordinates of second calibration point
                                x2:the x coordinates of third calibration point
                                y2:the y coordinates of third calibration point
                                x3:the x coordinates of fourth calibration point
                                y3:the y coordinates of fourth calibration point
                                fac:calibration factor 
 * @retvalue   :None
******************************************************************************/ 	 					  
void TP_Adj_Info_Show(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t fac)
{
    POINT_COLOR=RED;
    ILI9341_ShowString(40,140,16,"x1:",1);
    ILI9341_ShowString(40+80,140,16,"y1:",1);
    ILI9341_ShowString(40,160,16,"x2:",1);
    ILI9341_ShowString(40+80,160, 16,"y2:",1);
    ILI9341_ShowString(40,180, 16,"x3:",1);
    ILI9341_ShowString(40+80,180, 16,"y3:",1);
    ILI9341_ShowString(40,200, 16,"x4:",1);
    ILI9341_ShowString(40+80,200, 16,"y4:",1);  
    ILI9341_ShowString(40,220, 16,"fac is:",1);
    ILI9341_ShowNum(40+24,140,x0,4,16);	
    ILI9341_ShowNum(40+24+80,140,y0,4,16);
    ILI9341_ShowNum(40+24,160,x1,4,16);	
    ILI9341_ShowNum(40+24+80,160,y1,4,16);
    ILI9341_ShowNum(40+24,180,x2,4,16);	
    ILI9341_ShowNum(40+24+80,180,y2,4,16);
    ILI9341_ShowNum(40+24,200,x3,4,16);	
    ILI9341_ShowNum(40+24+80,200,y3,4,16);
    ILI9341_ShowNum(40+56,220,fac,3,16); 
}

/*****************************************************************************
 * @name       :uint8_t TP_Get_Adjdata(void)
 * @date       :2018-08-09 
 * @function   :Calibration touch screen and Get 4 calibration parameters
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 		 
void TP_Adjust(void)
{
    uint16_t pos_temp[4][2];
    uint8_t  cnt=0;	
    uint16_t d1,d2;
    uint32_t tem1,tem2;
    double fac;
    
    cnt=0;

    ILI9341_Clear(WHITE);
    POINT_COLOR=BLACK;
    BACK_COLOR =WHITE;
    
    /* Display adjust message */
    ILI9341_ShowString(10,40,16,"Please use the stylus click",1);
    ILI9341_ShowString(10,56,16,"the cross on the screen.",1); 
    ILI9341_ShowString(10,72,16,"The cross will always move",1);
    ILI9341_ShowString(10,88,16,"until the screen adjustment",1); 
    ILI9341_ShowString(10,104,16,"is completed.",1); 
    TP_Drow_Touch_Point(20,20,RED);
    tp_dev.sta=0;
    tp_dev.xfac=0;

    while(1)
    {   
        tp_dev.scan(1);   // read the state of touch pad
        if((tp_dev.sta&0xc0)==TP_CATH_PRES)
        {
            // outtime=0;
            tp_dev.sta&=~(1<<6);          
            pos_temp[cnt][0]=tp_dev.x;
            pos_temp[cnt][1]=tp_dev.y;
            cnt++;	  
            switch(cnt)
            {			   
                case 1:						 
                    TP_Drow_Touch_Point(20,20,WHITE);
                    TP_Drow_Touch_Point(lcddev.width-20,20,RED);
                    break;
                case 2:
                    TP_Drow_Touch_Point(lcddev.width-20,20,WHITE);
                    TP_Drow_Touch_Point(20,lcddev.height-20,RED);
                    break;
                case 3:
                     TP_Drow_Touch_Point(20,lcddev.height-20,WHITE);
                     TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,RED);
                    break;
                case 4:
                    tem1=abs(pos_temp[0][0]-pos_temp[1][0]); // x1-x2
                    tem2=abs(pos_temp[0][1]-pos_temp[1][1]); // y1-y2
                    tem1*=tem1;
                    tem2*=tem2;
                    d1=sqrt(tem1+tem2); // 

                    tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
                    tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
                    tem1*=tem1;
                    tem2*=tem2;
                    d2=sqrt(tem1+tem2); 
                    fac=(float)d1/d2;
                    if(fac<0.95||fac>1.05||d1==0||d2==0)  // restart the calibration 
                    {
                        cnt=0;
                         TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);  // 4
                            TP_Drow_Touch_Point(20,20,RED);								  // 1
                         TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ʾ����   
                         continue;
                    }
                    tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
                    tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
                    tem1*=tem1;
                    tem2*=tem2;
                    d1=sqrt(tem1+tem2); // distance between point 1 and 3
                    
                    tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
                    tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
                    tem1*=tem1;
                    tem2*=tem2;
                    d2=sqrt(tem1+tem2); // distance between point 2 and 4 
                    fac=(float)d1/d2;
                    if(fac<0.95||fac>1.05)  // restart the calibration 
                    {
                        cnt=0;
                         TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);  // 4
                            TP_Drow_Touch_Point(20,20,RED);								  // 1
                         TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ʾ����   
                        continue;
                    }
                    
                    tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
                    tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
                    tem1*=tem1;
                    tem2*=tem2;
                    d1=sqrt(tem1+tem2); // distance between point 1 and 3
    
                    tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
                    tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
                    tem1*=tem1;
                    tem2*=tem2;
                    d2=sqrt(tem1+tem2); // distance between point 2 and 4 
                    fac=(float)d1/d2;
                    if(fac<0.95||fac>1.05)  // restart the calibration 
                    {
                        cnt=0;
                         TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);
                            TP_Drow_Touch_Point(20,20,RED);							
                         TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//��ʾ����   
                        continue;
                    }
                    tp_dev.xfac=(float)(lcddev.width-40)/(pos_temp[1][0]-pos_temp[0][0]);     //
                    tp_dev.xoff=(lcddev.width-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2; //
                          
                    tp_dev.yfac=(float)(lcddev.height-40)/(pos_temp[2][1]-pos_temp[0][1]);    //
                    tp_dev.yoff=(lcddev.height-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//
                    if(abs(tp_dev.xfac)>2||abs(tp_dev.yfac)>2) // 
                    {
                        cnt=0;
                        TP_Drow_Touch_Point(lcddev.width-20,lcddev.height-20,WHITE);	// 
                        TP_Drow_Touch_Point(20,20,RED);								// 
                        ILI9341_ShowString(40,26, 16,"TP Need readjust!",1);
                        tp_dev.touchtype=!tp_dev.touchtype;
                        if(tp_dev.touchtype) 
                        {
                            CMD_RDX=0X90;
                            CMD_RDY=0XD0;	 
                        }else
                        {
                            CMD_RDX=0XD0;
                            CMD_RDY=0X90;	 
                        }
                        continue;
                    }
                    POINT_COLOR=BLUE;
                    ILI9341_Clear(WHITE);   // 
                    ILI9341_ShowString(35,110, 16,"Touch Screen Adjust OK!",1); 
                    HAL_Delay(1000);
                    // TP_Save_Adjdata();
                    ILI9341_Clear(WHITE);  // 
                    return; 			 
            }
        }
        delay_ms(20); // test button every 20ms 
        // outtime++;
        // if(outtime>1000)
        // {
        //     // get the adjust data 
        //     // TP_Get_Adjdata();
        //     outtime = 0;
        //     // break;
        // }
    }
}		

/*****************************************************************************
 * @name       :uint8_t TP_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization touch screen
 * @parameters :None
 * @retvalue   :0-no calibration
               :1-Has been calibrated
******************************************************************************/  
uint8_t TP_Init(void)
{   
    ILI9341_TOUCH_GPIO_CLK_ENABLE();	    // GPIOA
    ILI9341_TOUCH_SPI_CLK_ENABLE();		    // SPI1, use hardware transfer

    GPIO_InitTypeDef GPIO_InitStructure;	// GPIO Initilization 
    
    GPIO_InitStructure.Pin  = ILI9341_GPIO_PIN_TIRQ;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Alternate = 0;
    HAL_GPIO_Init(ILI9341_TOUCH_GPIO_PORT, &GPIO_InitStructure);  // Initialize Gpio
    
    /** Initialize the TCS Pin */
    GPIO_InitStructure.Pin = ILI9341_GPIO_PIN_TCS;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(ILI9341_TOUCH_GPIO_PORT, &GPIO_InitStructure);

    /**  Initialize the TCLK,  TO(M) and  TDI */
    GPIO_InitStructure.Pin  = ILI9341_GPIO_PIN_TCLK | ILI9341_GPIO_PIN_TDI | ILI9341_GPIO_PIN_TDO;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;         // if use hardware SPI, this line is AF_PP 
    GPIO_InitStructure.Alternate = ILI9341_TOUCH_GPIO_AF;
    HAL_GPIO_Init(ILI9341_TOUCH_GPIO_PORT, &GPIO_InitStructure);
    
    touch_spi_handler.Instance = ILI9341_TOUCH_SPI;
    touch_spi_handler.Init.Mode = SPI_MODE_MASTER;
    touch_spi_handler.Init.Direction = SPI_DIRECTION_2LINES;
    touch_spi_handler.Init.DataSize = SPI_DATASIZE_8BIT;
    touch_spi_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    touch_spi_handler.Init.CLKPolarity = SPI_POLARITY_LOW;
    touch_spi_handler.Init.CLKPhase = SPI_PHASE_1EDGE;
    touch_spi_handler.Init.NSS = SPI_NSS_SOFT;
    touch_spi_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;
    touch_spi_handler.Init.TIMode = SPI_TIMODE_DISABLE;
    touch_spi_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    touch_spi_handler.Init.CRCPolynomial = 7;
    HAL_SPI_DeInit(&touch_spi_handler);
    HAL_SPI_Init(&touch_spi_handler);
    __HAL_SPI_ENABLE(&touch_spi_handler);

    /*********** Init tp_dev structure  ***********/
    tp_dev.x = 0;
    tp_dev.y = 0;
    TP_Read_XY(&tp_dev.x,&tp_dev.y);

    // if(TP_Get_Adjdata())
    //     return 0;						//
    // else
    // { 										    
    //     ILI9341_Clear(WHITE);							// 
    //     TP_Adjust();									// 
    //     TP_Save_Adjdata();								
    // }
    return 1;								 
}


