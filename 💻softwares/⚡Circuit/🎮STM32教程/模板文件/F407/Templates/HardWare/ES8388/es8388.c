#include "es8388.h"


#define DEFAULT_OUTPUT_VOLUME  20
 
I2S_HandleTypeDef es8388_i2s_handler;
I2C_HandleTypeDef es8388_i2c_handler;
DMA_HandleTypeDef es8388_dma_handler;


void (*ES8388_i2s_tc_callback)(void) = NULL;

/**
 * @brief This table is used for set the best prescaler for sample rate 
 * @note The Sequence is PLLI2SN,  PLLI2SR, I2SDIV, ODD
*/
const uint16_t Accurate_I2S_Prescaler_Table[][4]=
{
    {   256, 5, 12, 1 },   /* 8Khz sample rate */
    {   429, 4, 19, 0 },   /* 11.025Khz sample rate */
    {   213, 2, 13, 0 },   /* 16Khz sample rate */
    {   429, 4,  9, 1 },   /* 22.05Khz sample rate */
    {   213, 2,  6, 1 },   /* 32Khz sample rate */
    {   271, 2,  6, 0 },   /* 44.1Khz sample rate */
    {   258, 3,  3, 1 },   /* 48Khz sample rate   */
    {   316, 2,  3, 1 },   /* 88.2Khz sample rate */
    {   344, 2,  3, 1 },   /* 96Khz sample rate   */
    {   393, 2,  2, 0 },   /* 192Khz sample rate   */
};

/** private function prototypes ---------------------------- */

static void ES8388_ioInit(void);
static void ES8388_i2cInit(void);
static void ES8388_i2sInit(void);
static void ES8388_dmaInit(uint8_t* buff1, uint8_t* buff2,  uint32_t size);

static uint8_t ES8388_write_reg(uint16_t reg_addr, uint8_t data);
static uint8_t ES8388_read_reg(uint16_t reg_addr, uint8_t *data);
static void ES8388_Unlock(void);

/** Interruption function redefinitions ------------------- */

/** I2C Event Handler */
void ES8388_I2C_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(&es8388_i2c_handler);
}

/** I2C Error Handler */
void ES8388_I2C_ER_IRQHandler(void)
{   
    HAL_I2C_ER_IRQHandler(&es8388_i2c_handler);
}

/** I2S DMA Stream Handler */
void ES8388_I2S_DMA_STREAM_IRQHandler(void)
{
    if (__HAL_DMA_GET_FLAG(&es8388_dma_handler, ES8388_I2S_DMA_TC_FLAG) != RESET){
        
        // HAL_DMA_IRQHandler(&es8388_dma_handler);  /** clear the interrupt Flags  */
        __HAL_DMA_CLEAR_FLAG(&es8388_dma_handler, ES8388_I2S_DMA_TC_FLAG);
        if (ES8388_i2s_tc_callback != NULL){
            ES8388_i2s_tc_callback();
        }
    }
}

/** private functions -------------------------------------- */
static void ES8388_ioInit(void){
    ES8388_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    /** I2S GPIO Init Section */
    
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = ES8388_I2S_AF;
    
    GPIO_InitStruct.Pin = ES8388_LRCK_PIN;
    HAL_GPIO_Init(ES8388_LRCK_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = ES8388_MCLK_PIN;
    HAL_GPIO_Init(ES8388_MCLK_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ES8388_SCLK_PIN;
    HAL_GPIO_Init(ES8388_SCLK_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = ES8388_SDIN_PIN;
    HAL_GPIO_Init(ES8388_SDIN_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = ES8388_SDOUT_PIN;
    HAL_GPIO_Init(ES8388_SDOUT_GPIO, &GPIO_InitStruct);
    
    /** I2C GPIO Init Section */
    GPIO_InitStruct.Pin = ES8388_I2C_SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;    /** @note must use AF_PP, can't use AF_OD */
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = ES8388_I2C_AF;
    HAL_GPIO_Init(ES8388_I2C_SCL_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = ES8388_I2C_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    HAL_GPIO_Init(ES8388_I2C_SDA_GPIO, &GPIO_InitStruct);
}

static void ES8388_i2sInit(void){
    __HAL_RCC_PLLI2S_ENABLE();   /** Must Initialize the PLLI2S Clock Source as Clock Source */

    ES8388_I2S_CLK_ENABLE();
    es8388_i2s_handler.Instance         = ES8388_I2S_SPI_PORT;
    es8388_i2s_handler.Init.Mode        = I2S_MODE_MASTER_TX;    /* Init as I2S TX mode */
    es8388_i2s_handler.Init.Standard    = I2S_STANDARD_PHILIPS;  /* Use Philips Standard */
    es8388_i2s_handler.Init.DataFormat  = I2S_DATAFORMAT_16B_EXTENDED;  /* 16 bit data format */
    es8388_i2s_handler.Init.MCLKOutput  = I2S_MCLKOUTPUT_ENABLE;
    es8388_i2s_handler.Init.AudioFreq   = I2S_AUDIOFREQ_DEFAULT;            /* I2S_AUDIOFREQ_44K */
    es8388_i2s_handler.Init.CPOL        = I2S_CPOL_LOW;
    es8388_i2s_handler.Init.ClockSource = I2S_CLOCK_PLL;         /* I2S ClockSource Configuration as PLL I2S Clock */
    es8388_i2s_handler.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
    HAL_I2S_DeInit(&es8388_i2s_handler);
    if (HAL_I2S_Init(&es8388_i2s_handler)!=HAL_OK){
        // Error_Handler();
    };
    /** NVIC configur for I2S -> enable request */
    HAL_NVIC_SetPriorityGrouping(ES8388_I2S_IRQ_PRIORITY_GROUPING);
    HAL_NVIC_SetPriority(ES8388_I2S_IRQn, ES8388_I2S_IRQ_PREPRIO, ES8388_I2S_IRQ_SUBPRIO);
    HAL_NVIC_EnableIRQ(ES8388_I2S_IRQn);

    ES8388_I2S_SPI_PORT->CR2 |=  (1 << 1);  /** set the DMA enable bit */
    __HAL_I2S_ENABLE(&es8388_i2s_handler);  /* start I2S enable bit -> must be added */
}

static void ES8388_dmaInit(uint8_t* buff1, uint8_t* buff2,  uint32_t size){
    ES8388_I2S_DMA_CLK_ENABLE();
    __HAL_LINKDMA(&es8388_i2s_handler, hdmatx, es8388_dma_handler);

    /** DMA Configuration */
    es8388_dma_handler.Instance     = ES8388_I2S_DMA_STREAM;
    es8388_dma_handler.Init.Channel = ES8388_I2S_DMA_CHANNEL;
    es8388_dma_handler.Init.Direction = DMA_MEMORY_TO_PERIPH;
    es8388_dma_handler.Init.Mode      = DMA_CIRCULAR;
    es8388_dma_handler.Init.PeriphInc = DMA_PINC_DISABLE;
    es8388_dma_handler.Init.MemInc    = DMA_MINC_ENABLE;
    es8388_dma_handler.Init.Priority  = DMA_PRIORITY_HIGH;
    es8388_dma_handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;  /** 16 byte mode */
    es8388_dma_handler.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    es8388_dma_handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    es8388_dma_handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    es8388_dma_handler.Init.MemBurst = DMA_MBURST_SINGLE;
    es8388_dma_handler.Init.PeriphBurst = DMA_PBURST_SINGLE;
    
    HAL_DMA_DeInit(&es8388_dma_handler);
    if (HAL_DMA_Init(&es8388_dma_handler)!= HAL_OK){
        // Error_Handler();
    }
    
    HAL_DMAEx_MultiBufferStart(&es8388_dma_handler,(uint32_t)buff1, (uint32_t)&ES8388_I2S_SPI_PORT->DR, (uint32_t)buff2, size/2);
    // HAL_DMAEx_MultiBufferStart_IT(&es8388_dma_handler, (uint32_t)buff1, (uint32_t)(&ES8388_I2S_SPI_PORT->DR), (uint32_t)buff2, size);
    __HAL_DMA_DISABLE(&es8388_dma_handler);                      /** Disable DMA as default */
    HAL_Delay(2);
    __HAL_DMA_CLEAR_FLAG(&es8388_dma_handler, ES8388_I2S_DMA_TC_FLAG);   /** Clear DMA transfer complete flag */
    __HAL_DMA_ENABLE_IT(&es8388_dma_handler, DMA_IT_TC);         /** Enable DMA tc it process function */
    /** NVIC Configuration for DMA */
    HAL_NVIC_SetPriorityGrouping(ES8388_I2S_DMA_PRIORITY_GROUPING);
    HAL_NVIC_SetPriority(ES8388_I2S_DMA_STREAM_IRQn, ES8388_I2S_DMA_STREAM_PREPRIO, ES8388_I2S_DMA_STREAM_SUBPRIO);
    HAL_NVIC_EnableIRQ(ES8388_I2S_DMA_STREAM_IRQn);
}

/**
 * @brief Initlize the I2C peripheral
 */
static void ES8388_i2cInit(void){
    ES8388_I2C_CLK_ENABLE();
    es8388_i2c_handler.Instance = ES8388_I2C_PORT;
    es8388_i2c_handler.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    es8388_i2c_handler.Init.ClockSpeed = 100000;            /** 100 kHz I2C standard mode */
    es8388_i2c_handler.Init.DutyCycle = I2C_DUTYCYCLE_2;    /** fit for high-speed communication */
    es8388_i2c_handler.Init.OwnAddress1 = ES8388_MASTER_ADDR;  /**  I2C port master address of stm32 */
    es8388_i2c_handler.Init.OwnAddress2 = 0x00;                /** Dual Address register */
    es8388_i2c_handler.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    es8388_i2c_handler.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    es8388_i2c_handler.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

    HAL_I2C_DeInit(&es8388_i2c_handler);
    HAL_I2C_Init(&es8388_i2c_handler);
    __HAL_I2C_ENABLE(&es8388_i2c_handler);
}

/**
 * @brief Write the register of ES8388
 * @note  The register address is 8-bit, and the data is 8-bit 
 */
static uint8_t ES8388_write_reg(uint16_t reg_addr, uint8_t data){
    uint8_t res = 0;
    HAL_I2C_IsDeviceReady(&es8388_i2c_handler,ES8388_ADDR, 10, 200);
    res = HAL_I2C_Mem_Write(&es8388_i2c_handler,ES8388_ADDR ,reg_addr,I2C_MEMADD_SIZE_8BIT, &data, 1, ES8388_TIMEOUT);
    HAL_Delay(2);  // This Delay must be add during the write and next operation */
    return res;
}

/**
 * @brief Read the register of ES8388
 * @param reg_addr 
 * @param data 
 * @return  
 */
static uint8_t ES8388_read_reg(uint16_t reg_addr, uint8_t *data){
    uint8_t res = 0;
    HAL_I2C_IsDeviceReady(&es8388_i2c_handler,ES8388_ADDR, 10, 200);
    res = HAL_I2C_Mem_Read(&es8388_i2c_handler, ES8388_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, data, 1, ES8388_TIMEOUT);
    HAL_Delay(2); // This Delay must be add during the write and next operation */
    return res;
}

/**
 * @brief Unlock the I2C Bus by software generate
 * @caution : This function shouldn't be called after ES8388_Init funtion
 * @note      it will automatically called by Init function 
 */
static void ES8388_Unlock(void){
    /** Use GPIO for Init */
    ES8388_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Pin = ES8388_I2C_SCL_PIN;
    HAL_GPIO_Init(ES8388_I2C_SCL_GPIO, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = ES8388_I2C_SDA_PIN;
    HAL_GPIO_Init(ES8388_I2C_SDA_GPIO, &GPIO_InitStruct);
    
    /** Generate Stop Signal In GPIO Mode */ 
    HAL_GPIO_WritePin(ES8388_I2C_SCL_GPIO, ES8388_I2C_SCL_PIN, GPIO_PIN_SET);
    HAL_Delay(2);
    HAL_GPIO_WritePin(ES8388_I2C_SDA_GPIO, ES8388_I2C_SDA_PIN, GPIO_PIN_SET);
    HAL_Delay(2);
    /* write 16 clock pulse */
    for (int i = 0; i < 16; i++) {
        HAL_GPIO_WritePin(ES8388_I2C_SCL_GPIO, ES8388_I2C_SCL_PIN, GPIO_PIN_RESET);
        HAL_Delay(2);
        HAL_GPIO_WritePin(ES8388_I2C_SCL_GPIO, ES8388_I2C_SCL_PIN, GPIO_PIN_SET);
        HAL_Delay(2);
    }
    /* Stop Singal  */
    HAL_GPIO_WritePin(ES8388_I2C_SDA_GPIO, ES8388_I2C_SDA_PIN, GPIO_PIN_RESET);
    HAL_Delay(2);
    HAL_GPIO_WritePin(ES8388_I2C_SCL_GPIO, ES8388_I2C_SCL_PIN, GPIO_PIN_SET);
    HAL_Delay(2);
    HAL_GPIO_WritePin(ES8388_I2C_SDA_GPIO, ES8388_I2C_SDA_PIN, GPIO_PIN_SET);
    HAL_Delay(300);
}

/**
 * @brief Init function of ESP8388 DAC chip peripheral 
 * @return 0 : succeed , 1 : failed  
 */
uint8_t ES8388_Init(uint8_t* buff1, uint8_t* buff2,  uint32_t size){
    /** Init GPIO Clock,  I2C and DMA peripherals */ 
    ES8388_Unlock();     /* avoid I2C lock */
    ES8388_ioInit();
    ES8388_i2cInit();
    ES8388_i2sInit();
    ES8388_dmaInit(buff1, buff2, size);
    
    HAL_Delay(100);
    /** Init  */
    if (HAL_I2C_IsDeviceReady(&es8388_i2c_handler,ES8388_ADDR, 10, 200)!=HAL_OK){
        return 1;
    }
    return 0;
}

/**
 * @brief Init peripheral and initialize using i2c 
 * @note  This function also set the music player into proper mode
 * @param buff1 
 * @param buff2 
 * @param buff_size 
 * @return 0: succeed , 1: failed
 */
uint8_t ES8388_OutPut_Init(uint8_t* buff1, uint8_t* buff2, uint32_t buff_size){
    if (ES8388_Init(buff1, buff2, buff_size) != HAL_OK) return 1;
    /** es8388 Initialize Sequence  */
    ES8388_write_reg(0x00, 0x80);    /** Reset the ES8388 */
    ES8388_write_reg(0x00, 0x00);    /** Set   to  0x00   */
    
    HAL_Delay(100);                  /** Wait for the ES8388 to be ready */ 
    
    ES8388_write_reg(0x01, 0x58);    /** R1: Power on the Analog Power first */  
    ES8388_write_reg(0x01, 0x50);    /** R1: exit low power mode, Start the  */
    ES8388_write_reg(0x02, 0xF3);    /** R2: Start ADC DLL first */
    ES8388_write_reg(0x02, 0xF0);    /** R2: Reset all the state machine */

    ES8388_write_reg(0x03, 0x09);
    // ES8388_write_reg(0x03, 0xF8);    /* Power down analog input              */
    ES8388_write_reg(0x00, 0x06);    /* enable internal powerdown/up circuit */
    ES8388_write_reg(0x04, 0x00);    /* R4: enable all output ports          */
    ES8388_write_reg(0x08, 0x00);    /* R8: BCLK settings: 0 DIV for MCLK    */
    ES8388_write_reg(0x2B, 0x80);    /* R43(DAC set): share ADC and DAC clk  */
    
    // Configure ADC (Analog-to-Digital Converter)
    ES8388_write_reg(0x09, 0x88);
    // ES8388_write_reg(0x09, 0x44);    // Set ADC left/right PGA gain to +12dB
    /* @caution : this will cause the left and right mix */
    ES8388_write_reg(0x0C, 0x4C);    // R12: Set ADC data selection to left data = left ADC, right data = left ADC, audio data format to 16-bit
    ES8388_write_reg(0x0D, 0x02);    // R13: Set ADC configuration: MCLK/sampling rate = 256
    ES8388_write_reg(0x10, 0x00);    // R16 Set ADC digital volume control: attenuation to minimum 0 dB
    ES8388_write_reg(0x11, 0x00);    // R17 Set ADC digital volume control: attenuation to minimum 0 dB
    
    // Configure DAC (Digital-to-Analog Converter)
    ES8388_write_reg(0x17, 0x18);    // R23: Set DAC audio data format to 16-bit(I2S format)
    ES8388_write_reg(0x18, 0x02);    // R24: Set DAC configuration: MCLK/sampling rate = 256 
    //ES8388_write_reg(0x1A, 0x40);    // R26: Set DAC digital volume control: attenuation to -4 dB
    ES8388_write_reg(0x1A, 0x00);
    //ES8388_write_reg(0x1B, 0x40);    // R27: Set DAC digital volume control: attenuation to -4 dB
    ES8388_write_reg(0x1B, 0x00);

    // Configure Mixer 
    ES8388_write_reg(0x27, 0xB8);    // R39: Configure left mixer (enable without LIN)
    ES8388_write_reg(0x2A, 0xB8);    // R42: Configure right mixer (enable without RIN)

    /* note -> DAC Analog  reference power down */
    HAL_Delay(100);
    
    ES8388_PwrCtrl(1, 0);        /** Power down ADC and up DAC */
    ES8388_OutputCtrl(1, 1);      /** Enable DAC output */
    ES8388_SetVolume_ch1(DEFAULT_OUTPUT_VOLUME);    /** Set volume of DAC output */
    ES8388_SetVolume_ch2(DEFAULT_OUTPUT_VOLUME);    /** Set volume of DAC output */

    return 0;
}

/**
 * @brief Set DAC Mode (DAC Control 1) 
 * @param swap 0: 
 * @param mode 00:I2S serial audio, 01:left justify  10:right justify  11:PCM/DSP mode 
 * @param datalen 00 : 24bit, 01: 20bit, 10: 18bit, 11: 16bit, 100: 32bit
 * @return uint8_t 
 */
uint8_t ES8388_SetMode(uint8_t swap, uint8_t polar, uint8_t mode, uint16_t datalen){
    swap&= 0x01; polar &= 0x01; mode &= 0x03; datalen &= 0x07;    /* limite the */
    ES8388_write_reg(23, (swap << 7) | (polar << 6)|(mode << 1) | (datalen << 3));  /* R23,ES8388工作模式设置 */
    return 0;
}


/**
 * @brief Set the volume of ES8388 output
 * @note  The operation is modify R46 and R47 
 * @param volume from 0 to 33 
 * @return * uint8_t 
 */
uint8_t ES8388_SetVolume_ch1(uint8_t volume){
    if (volume > 0x21)     volume = 0x21;
    ES8388_write_reg(0x2E, volume);
    ES8388_write_reg(0x2F, volume);
    return 0;
}

/**
 * @brief  Set Volume of ROUT2 and LOUT2
 * @param volume 0 to 33
 * @return uint8_t 
 */
uint8_t ES8388_SetVolume_ch2(uint8_t volume){
    if (volume > 0x21)     volume = 0x21;
    ES8388_write_reg(0x30, volume);
    ES8388_write_reg(0x31, volume); // may no need to read for validation 
    return 0;
}


/**
 * @brief Power control of ADC and DAC 
 * @param adc_disable 
 * @param dac_disable 
 * @return 0
 */
uint8_t ES8388_PwrCtrl(uint8_t adc_disable, uint8_t dac_disable){
    uint8_t temp = 0;
    if (adc_disable) {
        temp |= 0x0A;   // 0000 1010
    }
    if (dac_disable) {
        temp |= 0x05;   // 0000 0101
    }
    ES8388_write_reg(0x02, temp);
    return 0;
}

/**
 * @brief control diable of LROUT1 and LROUT2
 * @param ch1_diable 
 * @param ch2_diable 
 * @return uint8_t 
 */
uint8_t ES8388_OutputCtrl(uint8_t ch1_enable, uint8_t ch2_enable){
    uint8_t tempreg = 0;
    if (ch1_enable) {
        tempreg |= 0x30;    // 0011 0000
    }
    if (ch2_enable) {
        tempreg |= 0x0C;    // 0000 1100
    }
    ES8388_write_reg(0x04, tempreg);
    return 0;
}

/**
 * @brief Set the sample rate of ES8388
 * @param audio_freq support from 8kHz to 96kHz
 * @return uint8_t 
 */
uint8_t ES8388_SetSampleRate_Auto(uint32_t audio_freq){
    __HAL_I2S_DISABLE(&es8388_i2s_handler);
    switch (audio_freq){
        case 8000: es8388_i2s_handler .Init.AudioFreq = I2S_AUDIOFREQ_8K; break;
        case 11025: es8388_i2s_handler.Init.AudioFreq = I2S_AUDIOFREQ_11K; break;
        case 16000: es8388_i2s_handler.Init.AudioFreq = I2S_AUDIOFREQ_16K; break;
        case 22050: es8388_i2s_handler.Init.AudioFreq = I2S_AUDIOFREQ_22K; break;
        case 32000: es8388_i2s_handler.Init.AudioFreq = I2S_AUDIOFREQ_32K; break;
        case 44100: es8388_i2s_handler.Init.AudioFreq = I2S_AUDIOFREQ_44K; break;
        case 48000: es8388_i2s_handler.Init.AudioFreq = I2S_AUDIOFREQ_48K; break;
        case 96000: es8388_i2s_handler.Init.AudioFreq = I2S_AUDIOFREQ_96K; break;
        case 192000: es8388_i2s_handler.Init.AudioFreq = I2S_AUDIOFREQ_192K; break;
        default: return 1;  /* unsupported sample rate */
    }
    if (HAL_I2S_Init(&es8388_i2s_handler) != HAL_OK){  /* I2S init fail */
        return 1;
    }
    __HAL_I2S_ENABLE(&es8388_i2s_handler);  /** keep the  i2s enable --> not disable it */
    return 0;
}

/**
 * @brief Set the sample rate of ES8388 accurate by table data
 * @note  This can substitude ES8388_SetSampleRate_Auto and get a better performance. 
 * @param audio_freq 
 * @return uint8_t 
 */
uint8_t ES8388_SetSampleRate_Accurate(uint32_t audio_freq){
    uint8_t idx;
    switch (audio_freq){
        case 8000:  idx = 0; break;
        case 11025: idx = 1; break;
        case 16000: idx = 2; break;
        case 22050: idx = 3; break;
        case 32000: idx = 4; break;
        case 44100: idx = 5; break;
        case 48000: idx = 6; break;
        case 96000: idx = 7; break;
        case 192000: idx = 8; break;
        default: return 1;  /* unsupported sample rate */
    }
    uint32_t tempreg = 0;
    RCC_PeriphCLKInitTypeDef rcc_i2s_clkinit_struct;

    rcc_i2s_clkinit_struct.PeriphClockSelection = RCC_PERIPHCLK_I2S;        /* clock source configuration */
    rcc_i2s_clkinit_struct.PLLI2S.PLLI2SN = (uint32_t)Accurate_I2S_Prescaler_Table[idx][0];    /* set PLLI2SN */
    rcc_i2s_clkinit_struct.PLLI2S.PLLI2SR = (uint32_t)Accurate_I2S_Prescaler_Table[idx][1];    /* set PLLI2SR */
    HAL_RCCEx_PeriphCLKConfig(&rcc_i2s_clkinit_struct);

    tempreg = Accurate_I2S_Prescaler_Table[idx][2] << 0;   /* set I2S DIV */
    tempreg |= Accurate_I2S_Prescaler_Table[idx][3] << 8;  /* set I2S ODD */
    tempreg |= 1 << 9;                  /* Enable MCKOE */
    ES8388_I2S_SPI_PORT->I2SPR = tempreg;           /* set I2SPR register */
    return 0;
}

uint8_t ES8388_DMAStart(void){
    __HAL_DMA_ENABLE(&es8388_dma_handler);
    return 0;
}

uint8_t ES8388_DMAStop(void){
    __HAL_DMA_DISABLE(&es8388_dma_handler);
    /* delay_us(10) after use it */
    return 0;
}


// /**
//  * @brief       Set 3D surround sound
//  * @param       depth: 0 ~ 7 (3D strength, 0 = off, 7 = strongest)
//  * @retval      None
//  */
// void es8388_3d_set(uint8_t depth)
// {
//     depth &= 0x7; /* Limit the range */
//     es8388_write_reg(0x1D, depth << 2); /* R7, 3D surround setting */
// }

// /**
//  * @brief       ES8388 DAC/ADC configuration
//  * @param       dacen: DAC enable (1) / disable (0)
//  * @param       adcen: ADC enable (1) / disable (0)
//  * @retval      None
//  */
// void es8388_adda_cfg(uint8_t dacen, uint8_t adcen)
// {
//     uint8_t tempreg = 0;

//     tempreg |= ((!dacen) << 0);
//     tempreg |= ((!adcen) << 1);
//     tempreg |= ((!dacen) << 2);
//     tempreg |= ((!adcen) << 3);
//     es8388_write_reg(0x02, tempreg);
// }

// /**
//  * @brief       ES8388 DAC output channel configuration
//  * @param       o1en: Channel 1 enable (1) / disable (0)
//  * @param       o2en: Channel 2 enable (1) / disable (0)
//  * @retval      None
//  */
// void es8388_output_cfg(uint8_t o1en, uint8_t o2en)
// {
//     uint8_t tempreg = 0;
//     tempreg |= o1en * (3 << 4);
//     tempreg |= o2en * (3 << 2);
//     es8388_write_reg(0x04, tempreg);
// }

// /**
//  * @brief       ES8388 MIC gain setting (MIC PGA gain)
//  * @param       gain: 0~8, corresponding to 0~24dB (3dB/step)
//  * @retval      None
//  */
// void es8388_mic_gain(uint8_t gain)
// {
//     gain &= 0x0F;
//     gain |= gain << 4;
//     es8388_write_reg(0x09, gain); /* R9, left/right channel PGA gain setting */
// }

// /**
//  * @brief       ES8388 ALC (Automatic Level Control) setting
//  * @param       sel:
//  *   @arg       0: Disable ALC
//  *   @arg       1: Right channel ALC
//  *   @arg       2: Left channel ALC
//  *   @arg       3: Stereo ALC
//  * @param       maxgain: 0~7, corresponding to -6.5~+35.5dB
//  * @param       mingain: 0~7, corresponding to -12~+30dB (6dB/step)
//  * @retval      None
//  */
// void es8388_alc_ctrl(uint8_t sel, uint8_t maxgain, uint8_t mingain)
// {
//     uint8_t tempreg = 0;

//     tempreg = sel << 6;
//     tempreg |= (maxgain & 0x07) << 3;
//     tempreg |= mingain & 0x07;
//     es8388_write_reg(0x12, tempreg); /* R18, ALC setting */
// }

// /**
//  * @brief       ES8388 ADC input channel configuration
//  * @param       in: Input channel
//  *    @arg      0: Channel 1 input
//  *    @arg      1: Channel 2 input
//  * @retval      None
//  */
// void es8388_input_cfg(uint8_t in)
// {
//     es8388_write_reg(0x0A, (5 * in) << 4); /* ADC1 input channel selection L/R INPUT1 */
// }
