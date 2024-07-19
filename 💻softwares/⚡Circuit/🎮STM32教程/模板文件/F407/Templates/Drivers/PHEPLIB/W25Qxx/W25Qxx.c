#include "stm32f4xx_hal.h"
#include "W25Qxx.h"

/* Private variables ---------------------------------------------------------*/

static DMA_HandleTypeDef W25Qxx_dma_tx_handler;
static DMA_HandleTypeDef W25Qxx_dma_rx_handler;
static GPIO_InitTypeDef  W25Qxx_gpio_initstruct;
static SPI_HandleTypeDef W25Qxx_spi_handler;

/* Private function prototypes -----------------------------------------------*/

static void __W25Qxx_SPI_Init(void);
static void __W25Qxx_GPIO_Init(void);
static void __W25Qxx_DMA_Init(void);
static void __W25Qxx_WriteCmd(uint8_t byte);
static uint8_t __W25Qxx_ExchangeByte(uint8_t byte);
static void __W25Qxx_WaitBusy(void);
static void __W25Qxx_WriteEnable(uint8_t write_enable);
static void __W25Qxx_WriteStatusEnable(void);

/* Interruption functions ----------------------------------------------------*/

#ifdef __cplusplus
    extern "C"{
#endif

void W25Qxx_DMA_Tx_IRQHandler(){
HAL_DMA_IRQHandler(&W25Qxx_dma_tx_handler);
}

void W25Qxx_DMA_Rx_IRQHandler(){
    HAL_DMA_IRQHandler(&W25Qxx_dma_rx_handler);
}

void W25Qxx_SPI_IRQHandler(){
    HAL_SPI_IRQHandler(&W25Qxx_spi_handler);
}

#ifdef __cplusplus
    }
#endif // 

/* Private functions ---------------------------------------------------------*/

/// @brief Init SPI for W25Qxx 
static void __W25Qxx_SPI_Init(void){
    W25Qxx_SPI_CLK_ENABLE();
    W25Qxx_spi_handler.Instance = W25Qxx_SPI;
    W25Qxx_spi_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
    W25Qxx_spi_handler.Init.Direction = SPI_DIRECTION_2LINES;
    W25Qxx_spi_handler.Init.DataSize  = SPI_DATASIZE_8BIT;
    W25Qxx_spi_handler.Init.Mode     = SPI_MODE_MASTER;
    W25Qxx_spi_handler.Init.CLKPhase = SPI_PHASE_1EDGE;
    W25Qxx_spi_handler.Init.CLKPolarity = SPI_POLARITY_LOW;
    W25Qxx_spi_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;
    W25Qxx_spi_handler.Init.NSS = SPI_NSS_SOFT;
    W25Qxx_spi_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    W25Qxx_spi_handler.Init.CRCPolynomial = 7;
    W25Qxx_spi_handler.Init.TIMode = SPI_TIMODE_DISABLE;
    
    HAL_SPI_DeInit(&W25Qxx_spi_handler);
    
    if (HAL_SPI_Init(&W25Qxx_spi_handler) != HAL_OK){
        W25Qxx_Error_Handler();
    }
}

static void __W25Qxx_GPIO_Init(void){
    W25Qxx_MOSI_GPIO_CLK_ENABLE();
    W25Qxx_MISO_GPIO_CLK_ENABLE();
    W25Qxx_CS_GPIO_CLK_ENABLE();
    W25Qxx_SCK_GPIO_CLK_ENABLE();
    
    W25Qxx_gpio_initstruct.Pin  = W25Qxx_SCK_PIN;
    W25Qxx_gpio_initstruct.Mode = GPIO_MODE_AF_PP;
    W25Qxx_gpio_initstruct.Pull = GPIO_PULLUP;
    W25Qxx_gpio_initstruct.Speed = GPIO_SPEED_FAST;
    W25Qxx_gpio_initstruct.Alternate = W25Qxx_SCK_AF;
    HAL_GPIO_Init(W25Qxx_SCK_GPIO, &W25Qxx_gpio_initstruct);

    W25Qxx_gpio_initstruct.Pin = W25Qxx_MOSI_PIN;
    W25Qxx_gpio_initstruct.Alternate = W25Qxx_MOSI_AF;
    HAL_GPIO_Init(W25Qxx_MOSI_GPIO, &W25Qxx_gpio_initstruct);

    W25Qxx_gpio_initstruct.Pin = W25Qxx_MISO_PIN;
    W25Qxx_gpio_initstruct.Alternate = W25Qxx_MISO_AF;
    HAL_GPIO_Init(W25Qxx_MISO_GPIO, &W25Qxx_gpio_initstruct);

    W25Qxx_gpio_initstruct.Pin = W25Qxx_CS_PIN;
    W25Qxx_gpio_initstruct.Mode = GPIO_MODE_OUTPUT_PP;
    W25Qxx_gpio_initstruct.Alternate = 0x00;
    
    HAL_GPIO_Init(W25Qxx_CS_GPIO, &W25Qxx_gpio_initstruct);

    W25Qxx_CS_HIGH();      // not selected as default 
}

/* ---------- Init DMA1  Channel6 -------- */
static void __W25Qxx_DMA_Init(void){
    
    W25Qxx_DMA_Tx_CLK_ENABLE();
    W25Qxx_DMA_Rx_CLK_ENABLE();
    
    __HAL_LINKDMA(&W25Qxx_spi_handler, hdmatx, W25Qxx_dma_tx_handler);
    __HAL_LINKDMA(&W25Qxx_spi_handler, hdmarx, W25Qxx_dma_rx_handler);

    W25Qxx_dma_tx_handler.Instance = W25Qxx_DMA_Tx_Stream; // for example, DMA1_Stream0
    W25Qxx_dma_tx_handler.Init.Channel = W25Qxx_DMA_Tx_Channel;
    W25Qxx_dma_tx_handler.Init.Direction = DMA_MEMORY_TO_PERIPH;
    W25Qxx_dma_tx_handler.Init.Mode = DMA_NORMAL;
    W25Qxx_dma_tx_handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    W25Qxx_dma_tx_handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    W25Qxx_dma_tx_handler.Init.MemInc = DMA_MINC_ENABLE;
    W25Qxx_dma_tx_handler.Init.PeriphInc = DMA_PINC_DISABLE;
    W25Qxx_dma_tx_handler.Init.Priority = DMA_PRIORITY_LOW;
    W25Qxx_dma_tx_handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    W25Qxx_dma_tx_handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    W25Qxx_dma_tx_handler.Init.MemBurst = DMA_MBURST_SINGLE;
    W25Qxx_dma_tx_handler.Init.PeriphBurst = DMA_PBURST_SINGLE;
    
    W25Qxx_dma_rx_handler.Instance       = W25Qxx_DMA_Rx_Stream; // for example, DMA1_Stream0
    W25Qxx_dma_rx_handler.Init.Channel   = W25Qxx_DMA_Rx_Channel;
    W25Qxx_dma_rx_handler.Init.Direction = DMA_PERIPH_TO_MEMORY;
    W25Qxx_dma_rx_handler.Init.Mode      = DMA_NORMAL;
    W25Qxx_dma_rx_handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    W25Qxx_dma_rx_handler.Init.MemInc = DMA_MINC_ENABLE;
    W25Qxx_dma_rx_handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    W25Qxx_dma_rx_handler.Init.PeriphInc = DMA_PINC_DISABLE;
    W25Qxx_dma_rx_handler.Init.Priority = DMA_PRIORITY_HIGH;
    W25Qxx_dma_rx_handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    W25Qxx_dma_rx_handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    W25Qxx_dma_rx_handler.Init.MemBurst = DMA_MBURST_SINGLE;
    W25Qxx_dma_rx_handler.Init.PeriphBurst = DMA_PBURST_SINGLE; 
    
    HAL_DMA_DeInit(&W25Qxx_dma_rx_handler);
    HAL_DMA_DeInit(&W25Qxx_dma_tx_handler);
    if (HAL_DMA_Init(&W25Qxx_dma_tx_handler)!=HAL_OK || HAL_DMA_Init(&W25Qxx_dma_rx_handler) != HAL_OK){
        W25Qxx_Error_Handler();
    }

    /*##---- Configure the NVIC for DMA #########################################*/ 
    /* NVIC configuration for DMA transfer complete interrupt (Tx) */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
    HAL_NVIC_SetPriority(W25Qxx_DMA_Tx_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(W25Qxx_DMA_Tx_IRQn);
    
    /* NVIC configuration for DMA transfer complete interrupt (Rx) */
    HAL_NVIC_SetPriority(W25Qxx_DMA_Rx_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(W25Qxx_DMA_Rx_IRQn);
  
    /*##-5- Configure the NVIC for SPI #########################################*/
    HAL_NVIC_SetPriority(W25Qxx_SPI_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(W25Qxx_SPI_IRQn);
}

/// @brief Write 1-byte Command to W25Qxx 
/// @param byte 
static void __W25Qxx_WriteCmd(uint8_t byte){
    if (HAL_SPI_Transmit_DMA(&W25Qxx_spi_handler, (uint8_t*)(&byte), 1))
    {
        W25Qxx_Error_Handler();
    }
    while (HAL_SPI_GetState(&W25Qxx_spi_handler) != HAL_SPI_STATE_READY){
    };
}

static uint8_t __W25Qxx_ExchangeByte(uint8_t byte){
    uint8_t ch;
    if (HAL_SPI_TransmitReceive_DMA(&W25Qxx_spi_handler, (uint8_t*)&byte, &ch, 1))
    {
        W25Qxx_Error_Handler();
    }
    while (HAL_SPI_GetState(&W25Qxx_spi_handler) != HAL_SPI_STATE_READY);
    return ch;
}

/// @brief Wait for Busy Status
static void __W25Qxx_WaitBusy(void){
    W25Qxx_CS_LOW();
    __W25Qxx_WriteCmd(W25QxxCmd_ReadStatusReg);          // only read register 1
    while ((__W25Qxx_ExchangeByte(0xFF) & 0x01) != 0);   // while not BUSY
    W25Qxx_CS_HIGH();
}

/// @brief Enable Write Function
/// @param write_enable use true
static void __W25Qxx_WriteEnable(uint8_t write_enable){
    W25Qxx_CS_LOW();
    if (write_enable) __W25Qxx_WriteCmd(W25QxxCmd_WriteEnable);
    else __W25Qxx_WriteCmd(W25QxxCmd_WriteDisable);
    W25Qxx_CS_HIGH();
}

/// @brief Write Enable for Volatile Status Register 
static void __W25Qxx_WriteStatusEnable(void){
    W25Qxx_CS_LOW();
    __W25Qxx_WriteCmd(W25QxxCmd_WriteStatusEnable);
    W25Qxx_CS_HIGH();
}

/* Public Functions ***********************************************************/

/// @brief Init W25Qxx Series Flash 
uint8_t W25Qxx_Init(void){
    __W25Qxx_GPIO_Init();
    __W25Qxx_DMA_Init();  
    __W25Qxx_SPI_Init();  //
    return W25Qxx_ReadID(NULL, NULL);
}

/// @brief read Manufacture ID
/// @param mid addr of MID
/// @param did addr of DID
/// @return 
uint8_t W25Qxx_ReadID(uint8_t* mid, uint16_t* did){
    uint8_t tx[3] = {0xff, 0xff, 0xff};
    uint8_t rx[3] = {0xff, 0xff, 0xff};
    W25Qxx_CS_LOW();
    __W25Qxx_WriteCmd(W25QxxCmd_JedecDeviceID);
    if (HAL_SPI_TransmitReceive_DMA(&W25Qxx_spi_handler, (uint8_t*) tx, (uint8_t*)rx, 3)){
        W25Qxx_Error_Handler();
    }
    while (HAL_SPI_GetState(&W25Qxx_spi_handler) != HAL_SPI_STATE_READY);

    uint8_t MID = rx[0];
    uint16_t DID = (rx[1] << 8 | rx[2]);
    W25Qxx_CS_HIGH();
    if (mid!= NULL && did!= NULL)
    {
        (*mid) = MID;
        (*did) = DID;
    }
    if (MID == 0xFF) return 1;
    return 0;
}

/// @brief Erase the sector the byte address exists
/// @param SectorAddr address of the sector to erase
void W25Qxx_SectorErase(uint32_t SectorAddr){
    __W25Qxx_WaitBusy();
    __W25Qxx_WriteEnable(1);
    W25Qxx_CS_LOW();
    __W25Qxx_WriteCmd(W25QxxCmd_SectorErase);
    __W25Qxx_WriteCmd((SectorAddr & 0xFF0000) >> 16);
    __W25Qxx_WriteCmd((SectorAddr & 0x00FF00) >> 8);
    __W25Qxx_WriteCmd((SectorAddr & 0x0000FF));
    W25Qxx_CS_HIGH();
    __W25Qxx_WaitBusy();
}

/// @brief erase the sector of addr and then write message to it.
/// @note don't Write more than 256 bytes at once or exceed page 
void W25Qxx_WriteData(const uint32_t addr, uint8_t* buff, uint16_t size){
    W25Qxx_SectorErase(addr); // erase the sector
    __W25Qxx_WriteEnable(1);
    W25Qxx_CS_LOW();
    __W25Qxx_WriteCmd(W25QxxCmd_PageProgram);
    __W25Qxx_WriteCmd((addr & 0xFF0000)>> 16);
    __W25Qxx_WriteCmd((addr & 0x00FF00)>> 8);
    __W25Qxx_WriteCmd((addr & 0x0000FF));
    HAL_SPI_Transmit_DMA(&W25Qxx_spi_handler, buff, size);
    while (HAL_SPI_GetState(&W25Qxx_spi_handler) != HAL_SPI_STATE_READY);

    W25Qxx_CS_HIGH();
    __W25Qxx_WaitBusy();
}


/// @brief Read Data from address
/// @param addr Data Address to be read from 
/// @param buff 
/// @param size no limit 
/// @return 0 read succeed, 1 read failed 
uint8_t W25Qxx_ReadData(uint32_t addr, uint8_t *buff, uint16_t size){
    uint8_t result;
    __W25Qxx_WaitBusy();
    W25Qxx_CS_LOW();
    __W25Qxx_WriteCmd(W25QxxCmd_ReadData);
    /*  construct 24-bit address    */
    __W25Qxx_WriteCmd((addr & 0xFF0000)>>16);
    __W25Qxx_WriteCmd((addr & 0x00FF00)>>8);
    __W25Qxx_WriteCmd((addr & 0x0000FF));

    result = HAL_SPI_Receive_DMA(&W25Qxx_spi_handler, buff, size);
    while (HAL_SPI_GetState(&W25Qxx_spi_handler) != HAL_SPI_STATE_READY);
    W25Qxx_CS_HIGH();
    return result;
}

/// @brief Get status register (status 1 at low 8 digits and 2 at high 8 digits)
uint16_t W25Qxx_GetStatus(void){
    W25Qxx_CS_LOW();
    __W25Qxx_WriteCmd(W25QxxCmd_ReadStatus2Reg);
    uint8_t res = __W25Qxx_ExchangeByte(0xFF);
    W25Qxx_CS_HIGH();
    W25Qxx_CS_LOW();
    __W25Qxx_WriteCmd(W25QxxCmd_ReadStatusReg);
    uint8_t res2 = __W25Qxx_ExchangeByte(0xFF);
    W25Qxx_CS_HIGH();
    return (((uint16_t)res << 8) | res2);
}

/// @brief set status bits
/// @param SR_bit the status bit to set
/// @note can only set non-violate bits (must use false 0)
void W25Qxx_SetStatus(uint16_t SR_bit, uint8_t violateBit){
    uint16_t _curState_ = W25Qxx_GetStatus();
    uint16_t status = (_curState_ | SR_bit); // calculate the current bit;

    if  (violateBit) __W25Qxx_WriteStatusEnable(); 
    else __W25Qxx_WriteEnable(1);
    
    W25Qxx_CS_LOW();
    __W25Qxx_WriteCmd(W25QxxCmd_WriteStatusReg);
    __W25Qxx_WriteCmd((status & 0x00FF));       // low bit first
    __W25Qxx_WriteCmd((status & 0xFF00)>>8);
    W25Qxx_CS_HIGH();
    __W25Qxx_WaitBusy(); // wait for Write Register Cycle BUSY
}

/// @brief Reset status bits 
/// @param SR_bit the register bit to set 
/// @note can only set non-violate bits (must use false 0)
void W25Qxx_ResetStatus(uint16_t SR_bit, uint8_t violateBit){
    uint16_t _curState_ = W25Qxx_GetStatus();
    uint16_t status = (_curState_ & (~SR_bit)); // calculate the current bit;

    if  (violateBit) __W25Qxx_WriteStatusEnable();
    else __W25Qxx_WriteEnable(1);
    
    W25Qxx_CS_LOW();
    __W25Qxx_WriteCmd(W25QxxCmd_WriteStatusReg);
    __W25Qxx_WriteCmd((status & 0x00FF));       // low 
    __W25Qxx_WriteCmd((status & 0xFF00)>>8);
    W25Qxx_CS_HIGH();
    __W25Qxx_WaitBusy(); // wait for Write Register Cycle BUSY
}

__weak void W25Qxx_Error_Handler(void){
    while (1)
    {
        /* Go To Infinite Loop */
    }
}
