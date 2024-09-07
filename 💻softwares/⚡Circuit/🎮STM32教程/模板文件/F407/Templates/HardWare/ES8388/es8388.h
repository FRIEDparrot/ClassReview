/** Driver for es8388 Chip */

#ifndef __ES8388_H__
#define __ES8388_H__

#include "sys.h"


#define ES8388_ADDR          0x20
#define ES8388_MASTER_ADDR   0x00
#define ES8388_TIMEOUT       0xffff

#define ES8388_GPIO_CLK_ENABLE()    do {__HAL_RCC_GPIOB_CLK_ENABLE();__HAL_RCC_GPIOC_CLK_ENABLE();}while (0)
#define ES8388_I2S_CLK_ENABLE()     __HAL_RCC_SPI2_CLK_ENABLE()
#define ES8388_I2S_DMA_CLK_ENABLE() __HAL_RCC_DMA1_CLK_ENABLE()
#define ES8388_I2C_CLK_ENABLE()     __HAL_RCC_I2C1_CLK_ENABLE()

/** I2C PIN definition  */
#define ES8388_I2C_PORT            I2C1

#define ES8388_I2C_EV_IRQn         I2C1_EV_IRQn
#define ES8388_I2C_EV_IRQHandler   I2C1_EV_IRQHandler
#define ES8388_I2C_ER_IRQn         I2C1_ER_IRQn
#define ES8388_I2C_ER_IRQHandler   I2C1_ER_IRQHandler

/** I2S DMA Definitions */
#define ES8388_I2S_SPI_PORT                 SPI2
#define ES8388_I2S_IRQn                     SPI2_IRQn
// #define ES8388_I2S_IRQHandler               SPI2_IRQHandler
#define ES8388_I2S_IRQ_PRIORITY_GROUPING    NVIC_PRIORITYGROUP_2
#define ES8388_I2S_IRQ_PREPRIO    1
#define ES8388_I2S_IRQ_SUBPRIO    0

#define ES8388_I2S_DMA_CHANNEL              DMA_CHANNEL_0
#define ES8388_I2S_DMA_STREAM               DMA1_Stream4     /* used for */

#define ES8388_I2S_DMA_STREAM_IRQn          DMA1_Stream4_IRQn
#define ES8388_I2S_DMA_STREAM_IRQHandler    DMA1_Stream4_IRQHandler
#define ES8388_I2S_DMA_PRIORITY_GROUPING    NVIC_PRIORITYGROUP_1
#define ES8388_I2S_DMA_STREAM_PREPRIO       1
#define ES8388_I2S_DMA_STREAM_SUBPRIO       1
#define ES8388_I2S_DMA_TC_FLAG              DMA_FLAG_TCIF0_4

/** I2S PIN definition  */ 
#define ES8388_LRCK_GPIO     GPIOB
#define ES8388_LRCK_PIN      GPIO_PIN_12   /* PB12: CS */
#define ES8388_SCLK_GPIO     GPIOB
#define ES8388_SCLK_PIN      GPIO_PIN_13   /* PB13: SCLK */
#define ES8388_SDIN_GPIO     GPIOC
#define ES8388_SDIN_PIN      GPIO_PIN_3    /* PC3: SDIN */
#define ES8388_SDOUT_GPIO    GPIOC
#define ES8388_SDOUT_PIN     GPIO_PIN_2    /* PC2: SDOUT */
#define ES8388_MCLK_GPIO     GPIOC
#define ES8388_MCLK_PIN      GPIO_PIN_6    /*  PC6: MCLK */

#define ES8388_I2C_SCL_GPIO  GPIOB
#define ES8388_I2C_SCL_PIN   GPIO_PIN_8     /* PB8: SCL */
#define ES8388_I2C_SDA_GPIO  GPIOB
#define ES8388_I2C_SDA_PIN   GPIO_PIN_9     /* PB9: SDA */

#define ES8388_I2S_AF       GPIO_AF5_SPI2
#define ES8388_I2SEx_AF     GPIO_AF6_I2S2ext
#define ES8388_I2C_AF       GPIO_AF4_I2C1


uint8_t ES8388_Init(uint8_t* buff1, uint8_t* buff2,  uint32_t size);
uint8_t ES8388_OutPut_Init(uint8_t* buff1, uint8_t* buff2, uint32_t buff_size);   /** Init with default output settings */

uint8_t ES8388_SetMode(uint8_t swap, uint8_t polar, uint8_t mode, uint16_t datalen);
uint8_t ES8388_SetVolume_ch1(uint8_t volume);
uint8_t ES8388_SetVolume_ch2(uint8_t volume);

uint8_t ES8388_PwrCtrl(uint8_t adc_disable, uint8_t dac_disable);
uint8_t ES8388_OutputCtrl(uint8_t ch1_enable, uint8_t ch2_enable);

uint8_t ES8388_SetSampleRate_Auto(uint32_t audio_freq);
uint8_t ES8388_SetSampleRate_Accurate(uint32_t audio_freq);

/** start or stop DMA transfer */ 
uint8_t ES8388_DMAStart(void);
uint8_t ES8388_DMAStop(void);

#endif /* __ES8388_H__ */
