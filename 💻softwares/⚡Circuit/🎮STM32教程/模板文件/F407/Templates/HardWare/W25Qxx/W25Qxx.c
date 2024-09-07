/************************ Declaration **************************
 * 
 * 
 * 
 * 
 * 
 * 
 */

#include "stm32f4xx_hal.h"
#include "W25Qxx.h"
#include "sdcard_driver.h"

#include "stdio.h"   // remove it if not debug (use "ASSERT")

/* Private variables ---------------------------------------------------------*/

static DMA_HandleTypeDef W25Qxx_dma_tx_handler;
static DMA_HandleTypeDef W25Qxx_dma_rx_handler;
static GPIO_InitTypeDef  W25Qxx_gpio_initstruct;
static SPI_HandleTypeDef W25Qxx_spi_handler;

static uint8_t fs_buffer[W25Qxx_Sector_Space];
static FATFS fs;

/* Private function prototypes -----------------------------------------------*/

static void __W25Qxx_SPI_Init(void);
static void __W25Qxx_GPIO_Init(void);
static void __W25Qxx_DMA_Init(void);
static void __W25Qxx_WriteCmd(uint8_t byte);
static uint8_t __W25Qxx_ExchangeByte(uint8_t byte);
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
    W25Qxx_spi_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
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

/// @brief Wait for Busy Status
uint8_t W25Qxx_WaitBusy(void){
    W25Qxx_CS_LOW();
    __W25Qxx_WriteCmd(W25QxxCmd_ReadStatusReg);          // only read register 1
    
    uint32_t t = W25Qxx_TIMEOUT;
    while ((__W25Qxx_ExchangeByte(0xFF) & 0x01) != 0){
        if (t-- == 0) {
            W25Qxx_ASSERT("Exceed The Maximum Timeout");
            W25Qxx_CS_HIGH();
            return 1;
        }
    };   // while not BUSY
    W25Qxx_CS_HIGH();
    return 0;
}

/// @brief Init W25Qxx Series Flash 
uint8_t W25Qxx_Init(void){
    __W25Qxx_GPIO_Init();
    __W25Qxx_DMA_Init();  
    __W25Qxx_SPI_Init();  //
    return W25Qxx_ReadID(NULL, NULL);
}

/// @brief read Manufacture ID, also used for tet chip
/// @param mid addr of MID
/// @param did addr of DID
/// @return 
uint8_t W25Qxx_ReadID(uint8_t* mid, uint16_t* did){
    uint8_t tx[3] = {0xff, 0xff, 0xff};
    uint8_t rx[3] = {0xff, 0xff, 0xff};
    
    W25Qxx_WaitBusy();
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
/// @note  at least erase 1 sector (4kb) per time
/// @param SectorAddr address of the sector to erase
void W25Qxx_SectorErase(uint32_t SectorAddr){
    W25Qxx_WaitBusy();
    __W25Qxx_WriteEnable(1);
    W25Qxx_CS_LOW();
    __W25Qxx_WriteCmd(W25QxxCmd_SectorErase);
    __W25Qxx_WriteCmd((SectorAddr & 0xFF0000) >> 16);
    __W25Qxx_WriteCmd((SectorAddr & 0x00FF00) >> 8);
    __W25Qxx_WriteCmd((SectorAddr & 0x0000FF));
    W25Qxx_CS_HIGH();
}

/// @brief Read Data from address
/// @param addr Data Address to be read from 
/// @param buff buffer of the 
/// @param size maximum read 65536 (64kb) of data from W25Qxx
/// @return 0 read succeed, 1 read failed 
uint8_t W25Qxx_ReadData(uint32_t addr, uint8_t *buff, uint16_t size){
    if (W25Qxx_MAX_ADDRESS - size + 1 < addr) return 1;
    uint8_t result;
    W25Qxx_WaitBusy();
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

/// @brief erase the sector of addr and then write message to it.
/// @warning this write wouldn't call sector erase; 
/// @note don't Write more than 256 bytes at once or exceed page 
uint8_t W25Qxx_WriteData(const uint32_t addr, const uint8_t* buff, uint16_t size){
    if (size >  W25Qxx_Page_Space){
        W25Qxx_ASSERT("Write Exceed Size");
        return 1;
    }
    if (W25Qxx_MAX_ADDRESS - size + 1 < addr) return 1;
    W25Qxx_WaitBusy();
    __W25Qxx_WriteEnable(1);
    W25Qxx_CS_LOW();
    __W25Qxx_WriteCmd(W25QxxCmd_PageProgram);
    __W25Qxx_WriteCmd((addr & 0xFF0000)>> 16);
    __W25Qxx_WriteCmd((addr & 0x00FF00)>> 8);
    __W25Qxx_WriteCmd((addr & 0x0000FF));
    
    if (HAL_SPI_Transmit_DMA(&W25Qxx_spi_handler, (uint8_t*)buff, size)){
        W25Qxx_ASSERT("READ BUFF ERR");
        return 1;
    }
    while (HAL_SPI_GetState(&W25Qxx_spi_handler) != HAL_SPI_STATE_READY);

    W25Qxx_CS_HIGH();
    return 0;
}


/// @brief Read multiple specified continuous sectors from the chip
/// @param buff buffer for data
/// @param saddr address of sector (512bytes per sector)
/// @param sec_cnt count of sector 
/// @return 
uint8_t W25Qxx_ReadSector(uint8_t *buff, uint32_t saddr, uint32_t sec_cnt){
    uint32_t tot_read_size = sec_cnt * W25Qxx_Sector_Space;
    if (tot_read_size >  W25Qxx_MAX_ADDRESS + 1 || W25Qxx_MAX_ADDRESS + 1 - tot_read_size < saddr * W25Qxx_Sector_Space) {
        W25Qxx_ASSERT(" No Space to read ");
        return 1;
    }

    /** read a sector per time */
    for (int i = 0; i < sec_cnt; i++){
        uint32_t offset = i * W25Qxx_Sector_Space;
        W25Qxx_ReadData(W25Qxx_Sector_Space * saddr + offset, buff + offset, W25Qxx_Sector_Space);
    }
    return 0;
}

/// @brief Write a block of data (4kb) to Chip 
/// @param start_addr  
/// @param buff  size to be write of this buff must be 4kb (a whole )
/// @return 1: failed;  2: succeed; 
uint8_t W25Qxx_WriteBlock(uint32_t start_addr, const uint8_t* buff){
    if (start_addr >  W25Qxx_MAX_ADDRESS || W25Qxx_MAX_ADDRESS + 1 - start_addr < W25Qxx_Sector_EraseGrSize * W25Qxx_Sector_Size ){
        W25Qxx_ASSERT("write blcok failed, full storge");
        return 1;
    }
    uint32_t page_cnt = W25Qxx_Sector_EraseGrSize * W25Qxx_Sector_Size;
    for (int i = 0; i < page_cnt; i++){
        W25Qxx_WriteData(start_addr + i * W25Qxx_Page_Space, buff + i* W25Qxx_Page_Space, W25Qxx_Page_Space);
    }
    return 0;
}

/// @brief Write multiple specified continuous sectors to the chip
/// @note  This function is used for port in FATFS file system, sector size is 512 bytes
/// @param buff     buffer to write
/// @param saddr    sector address (from 1)
/// @param sec_cnt  count of sectors 
/// @return 
uint8_t W25Qxx_WriteSector(const uint8_t *buff, uint32_t saddr, uint32_t sec_cnt){
    uint32_t tot_write_size = sec_cnt * W25Qxx_Sector_Space;
    if (tot_write_size >  W25Qxx_MAX_ADDRESS + 1 || W25Qxx_MAX_ADDRESS + 1 - tot_write_size < saddr * W25Qxx_Sector_Space){
        W25Qxx_ASSERT(" No Space to write ");
        return 1;
    }
    /** the sector leave to be read -> front overflow and back overflow must be considered */
    uint32_t  sector_left = sec_cnt;
    uint32_t  saddr_new   = saddr;
    
    /** front overflow -> read out the un-aligned sector first, erase the whole block(4kb) and re-write block */
    if (saddr % W25Qxx_Sector_EraseGrSize != 0) {
        /** W25Qxx_ASSERT("Write front overflow!"); */

        saddr_new = saddr - saddr % W25Qxx_Sector_EraseGrSize;                             /** start address to be read */
        uint32_t    tmp = W25Qxx_Sector_EraseGrSize - saddr % W25Qxx_Sector_EraseGrSize;        /** left sector to read in the block */
        uint32_t    write_sec_cnt = sec_cnt <= tmp ? sec_cnt : tmp;
        
        uint8_t     *buff_tmp = (uint8_t*)W25Qxx_malloc(W25Qxx_Sector_Space * W25Qxx_Sector_EraseGrSize); /** store the block */
        W25Qxx_ReadSector(buff_tmp, saddr_new, W25Qxx_Sector_EraseGrSize);               /** read a whole block */
        W25Qxx_SectorErase(saddr_new * W25Qxx_Sector_Space);                             /** erase the current sector */
        
        /** offset of the first sector in buff_tmp */
        uint32_t write_offset = (saddr - saddr_new) * W25Qxx_Sector_Space;
        for (int i = 0; i < write_sec_cnt * W25Qxx_Sector_Space; i++){
            buff_tmp[i + write_offset] = buff[i];
        }
        
        /** re-write the block to storge */
        W25Qxx_WriteBlock(saddr_new * W25Qxx_Sector_Space, buff_tmp);
        W25Qxx_free(buff_tmp);

        /** refresh the sector leave to be read */
        saddr_new   = saddr_new + W25Qxx_Sector_EraseGrSize;
        sector_left-= write_sec_cnt;
    }
    
    /** each time erase 1 block (4kb) and write 1 block */
    while (sector_left >= W25Qxx_Sector_EraseGrSize){
        uint32_t write_offset = saddr_new * W25Qxx_Sector_Space;
        W25Qxx_SectorErase(saddr_new * W25Qxx_Sector_Space);

        W25Qxx_WriteBlock(write_offset,  buff + (sec_cnt - sector_left) * W25Qxx_Sector_Space);
        sector_left -= W25Qxx_Sector_EraseGrSize;
        saddr_new   += W25Qxx_Sector_EraseGrSize;
    }
    
    /** back overflow -> read out the un-aligned sector first, erase the whole block(4kb) and re-write block */
    if (sector_left > 0){ 
        /** W25Qxx_ASSERT("Write back overflow!"); */
        uint8_t     *buff_tmp = (uint8_t*)W25Qxx_malloc(W25Qxx_Sector_Space * W25Qxx_Sector_EraseGrSize); /** store the block */
        W25Qxx_ReadSector(buff_tmp, saddr_new, W25Qxx_Sector_EraseGrSize);

        /** offset of the last sector in buff */
        uint32_t write_offset = (sec_cnt - sector_left) * W25Qxx_Sector_Space;
        for (int i = 0; i < sector_left * W25Qxx_Sector_Space; i++){
            buff_tmp[i] = buff[write_offset + i];
        }
        W25Qxx_SectorErase(saddr_new * W25Qxx_Sector_Space);
        
        W25Qxx_WriteBlock(saddr_new * W25Qxx_Sector_Space, buff_tmp);
        W25Qxx_free(buff_tmp);
        sector_left = 0;
    }
    return 0;
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
    W25Qxx_WaitBusy(); // wait for Write Register Cycle BUSY
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
    W25Qxx_WaitBusy(); // wait for Write Register Cycle BUSY
}

uint8_t W25Qxx_InitFS(void){
    FRESULT res = f_mount(&fs,W25Qxx_ROOTDIRECTORY, 1);
    if (f_mount(&fs, W25Qxx_ROOTDIRECTORY, 1) == FR_OK){
        return 0;
    }
    else if (res == FR_NO_FILESYSTEM){
        /** begin format W25Qxx */
        res = f_mkfs(W25Qxx_ROOTDIRECTORY, 0, fs_buffer, W25Qxx_Sector_Space);
        if (res == FR_OK) {
            printf("format W25Qxx suc: %d\r\n", res);
            res = f_mount(&fs, W25Qxx_ROOTDIRECTORY, 1);
            if (res == FR_OK){
                return 0;
            }
        }
    }
    return 1;
}



__weak void W25Qxx_Error_Handler(void){
    while (1)
    {
        /* Go To Infinite Loop */
    }
}
