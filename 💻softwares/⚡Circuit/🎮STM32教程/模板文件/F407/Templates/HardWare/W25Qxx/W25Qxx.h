#ifndef __W25Qxx_H
#define __W25Qxx_H

#ifdef __cplusplus
    extern "C"{
#endif // DEBUG

#include "sys.h"    /** system config file */

#define W25Qxx_TYPE                 128     /** W25Q128 Driver functions */

#define W25Qxx_ROOTDIRECTORY        "0:"

#define W25Qxx_Sector_Space         512     /** 512 bytes per sector (for FATFS) */
#define W25Qxx_Sector_Size          2       /** 2   pages per sector (for FATFS) */
#define W25Qxx_Sector_EraseGrSize   8       /** erase at least 4kb per time      */

#define W25Qxx_Page_Space           256     /** 256 bytes per page   */

#define W25Qxx_TIMEOUT          (0xffffffffU)

#define W25Qxx_ASSERT(msg)      printf("error: %s at %s : %d\r\n", msg, __FILE__, __LINE__)

/************ Malloc  Configurations **************/
#if (SYS_USER_MALLOC == 0) && (SYS_SUPPORT_OS == 0)
    #include "stdlib.h"
    #define W25Qxx_malloc   malloc
    #define W25Qxx_free     free
#elif (SYS_USER_MALLOC == 1) && (SYS_SUPPORT_OS == 0)
    #include "../MALLOC/mymalloc.h"
    #define W25Qxx_malloc(x)   mymalloc(SRAMIN, x)
    #define W25Qxx_free(x)     myfree(SRAMIN, x)
#elif (SYS_SUPPORT_OS == 1) // FREERTOS SUPPORT
    #include "FreeRTOS.h"
    #include "task.h"
#else
    #error "Please select a memory management method!"
#endif

/************ W25Qxx Driver Configurations **************/
#if (W25Qxx_TYPE == 128) 
    #define W25Qxx_Sector_Count 32768
    #define W25Qxx_MAX_ADDRESS 0x00ffffff
#elif (W25Qxx_TYPE == 64)
    #define W25Qxx_Sector_Count 16384
    #define W25Qxx_MAX_ADDRESS 0x007fffff
#elif (W25Qxx_TYPE == 32)
    #define W25Qxx_Sector_Count 8192
    #define W25Qxx_MAX_ADDRESS 0x003fffff
#elif (W25Qxx_TYPE == 16)
    #define W25Qxx_Sector_Count 4096
    #define W25Qxx_MAX_ADDRESS 0x001fffff
#else
    #error "Plase #define the correct Flash Type!"
#endif

/**
 * @defgroup W25Qxx_Pin_Config  
 * @brief Pin Configurations and CLK Enable for W25Qxx Driver
 */
#define W25Qxx_SPI_CLK_ENABLE       __HAL_RCC_SPI3_CLK_ENABLE
#define W25Qxx_SPI                  SPI3
#define W25Qxx_SPI_IRQn             SPI3_IRQn
#define W25Qxx_SPI_IRQHandler       SPI3_IRQHandler

#define W25Qxx_DMA_Tx_CLK_ENABLE    __HAL_RCC_DMA1_CLK_ENABLE
#define W25Qxx_DMA_Tx_Stream        DMA1_Stream7
#define W25Qxx_DMA_Tx_Channel       DMA_CHANNEL_0
#define W25Qxx_DMA_Tx_IRQn          DMA1_Stream7_IRQn
#define W25Qxx_DMA_Tx_IRQHandler    DMA1_Stream7_IRQHandler

#define W25Qxx_DMA_Rx_CLK_ENABLE    __HAL_RCC_DMA1_CLK_ENABLE
#define W25Qxx_DMA_Rx_Stream        DMA1_Stream0
#define W25Qxx_DMA_Rx_Channel       DMA_CHANNEL_0
#define W25Qxx_DMA_Rx_IRQn          DMA1_Stream0_IRQn
#define W25Qxx_DMA_Rx_IRQHandler    DMA1_Stream0_IRQHandler

#define W25Qxx_SCK_GPIO_CLK_ENABLE  __HAL_RCC_GPIOB_CLK_ENABLE
#define W25Qxx_SCK_GPIO             GPIOB
#define W25Qxx_SCK_PIN              GPIO_PIN_3
#define W25Qxx_SCK_AF               GPIO_AF6_SPI3

#define W25Qxx_MOSI_GPIO_CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE
#define W25Qxx_MOSI_GPIO            GPIOB
#define W25Qxx_MOSI_PIN             GPIO_PIN_5
#define W25Qxx_MOSI_AF              GPIO_AF6_SPI3

#define W25Qxx_MISO_GPIO_CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE
#define W25Qxx_MISO_GPIO            GPIOB
#define W25Qxx_MISO_PIN             GPIO_PIN_4
#define W25Qxx_MISO_AF              GPIO_AF6_SPI3

#define W25Qxx_CS_GPIO_CLK_ENABLE   __HAL_RCC_GPIOB_CLK_ENABLE
#define W25Qxx_CS_GPIO              GPIOB
#define W25Qxx_CS_PIN               GPIO_PIN_14

#define W25Qxx_CS_HIGH()            HAL_GPIO_WritePin(W25Qxx_CS_GPIO, W25Qxx_CS_PIN, GPIO_PIN_SET)
#define W25Qxx_CS_LOW()             HAL_GPIO_WritePin(W25Qxx_CS_GPIO, W25Qxx_CS_PIN, GPIO_PIN_RESET)

/** @defgroup W25QxxCmd  
 *  @brief W25Qxx Command List 
 *  @{
*/
#define W25QxxCmd_WriteEnable		    0x06 
#define W25QxxCmd_WriteDisable		    0x04 
#define W25QxxCmd_WriteStatusEnable     0x50
#define W25QxxCmd_ReadStatusReg		    0x05 
#define W25QxxCmd_ReadStatus2Reg        0x35
#define W25QxxCmd_WriteStatusReg		0x01 
#define W25QxxCmd_ReadData			    0x03 
#define W25QxxCmd_FastReadData		    0x0B 
#define W25QxxCmd_FastReadDual		    0x3B 
#define W25QxxCmd_PageProgram		    0x02 
#define W25QxxCmd_BlockErase			0xD8 
#define W25QxxCmd_SectorErase		    0x20 
#define W25QxxCmd_ChipErase			    0xC7 
#define W25QxxCmd_PowerDown			    0xB9 
#define W25QxxCmd_ReleasePowerDown	    0xAB 
#define W25QxxCmd_DeviceID			    0xAB 
#define W25QxxCmd_ManufactDeviceID   	0x90 
#define W25QxxCmd_JedecDeviceID		    0x9F
#define W25QxxCmd_EnableReset           0x66
#define W25QxxCmd_Reset                 0x99
/**
 * @}
 */

/* Public functions --------------------------------------------------------- */

uint8_t W25Qxx_WaitBusy(void);
uint8_t W25Qxx_Init(void);

uint8_t W25Qxx_ReadID(uint8_t* mid, uint16_t* did);
void    W25Qxx_SectorErase(uint32_t SectorAddr);


uint8_t W25Qxx_WriteData(const uint32_t addr, const uint8_t* buff, uint16_t size);
uint8_t W25Qxx_ReadData(const uint32_t addr, uint8_t *buff, uint16_t size);

uint8_t W25Qxx_WriteBlock(uint32_t start_addr, const uint8_t* buff);
uint8_t W25Qxx_WriteSector(const uint8_t *buff, uint32_t saddr, uint32_t sec_cnt);
uint8_t W25Qxx_ReadSector(uint8_t *buff, uint32_t saddr, uint32_t sec_cnt);


uint16_t W25Qxx_GetStatus(void);
void W25Qxx_SetStatus(uint16_t SR_bit, uint8_t violateBit);
void W25Qxx_ResetStatus(uint16_t SR_bit, uint8_t violateBit);

/**  File System relevant functions ------------------------------------------ */ 

uint8_t W25Qxx_InitFS(void);

void W25Qxx_Error_Handler(void);

#ifdef __cplusplus
    }
#endif

#endif // !__W25Qxx_H