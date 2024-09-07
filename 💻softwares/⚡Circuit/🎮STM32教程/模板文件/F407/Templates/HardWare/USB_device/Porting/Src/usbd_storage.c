/**
  ******************************************************************************
  * @file    USB_Device/MSC_Standalone/Src/usbd_storage.c
  * @author  MCD Application Team
  * @brief   Memory management layer
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "usbd_storage.h"
#include "sdcard_driver.h"
#include "W25Qxx.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
__IO uint32_t writestatus, readstatus = 0;

/* USB Mass storage Standard Inquiry Data */
int8_t STORAGE_Inquirydata[] = { /* 36 */
  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer: 8 bytes  */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product     : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0','1',                      /* Version     : 4 Bytes  */

  /* LUN 1 */
  0x00,           // peripheral type bit 7:5
  0x80,           // reserved bit 7: removable media bit (set 1 to indicate removable media)
  0x02,           // ANSI Version 
  0x02,           // Response Data format - reserved
  (STANDARD_INQUIRY_DATA_LEN - 5),   //  additional length, specify the length in bytes of the parameters
  0x00,
  0x00,
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ',  /* Manufacturer : 8bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product : 16bytes     */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0','1',                      /* Version : 4bytes      */
};

/* Private function prototypes -----------------------------------------------*/
int8_t STORAGE_Init(uint8_t lun);
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
int8_t STORAGE_IsReady(uint8_t lun);
int8_t STORAGE_IsWriteProtected(uint8_t lun);
int8_t STORAGE_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
int8_t STORAGE_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
int8_t STORAGE_GetMaxLun(void);

USBD_StorageTypeDef USBD_DISK_fops = {
  STORAGE_Init,
  STORAGE_GetCapacity,
  STORAGE_IsReady,
  STORAGE_IsWriteProtected,
  STORAGE_Read,
  STORAGE_Write,
  STORAGE_GetMaxLun,
  STORAGE_Inquirydata,
};
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the storage unit (medium)
  * @param  lun: Logical unit number
  * @retval Status (0 : Ok / -1 : Error)
  */
int8_t STORAGE_Init(uint8_t lun)
{
  int8_t ret = -1;
  switch (lun){
    case 0:
      if (W25Qxx_InitFS()) ret = -1;
      else ret = 0;
      break;
    case 1:
      if (SDCard_initFS()) ret = -1;
      else ret = 0;
      break;
    default:
      break;
  }
  return ret;
}

/**
  * @brief  Returns the medium capacity.
  * @param  lun: Logical unit number
  * @param  block_num: Number of total block number
  * @param  block_size: Block size
  * @retval Status (0: Ok / -1: Error)
  */
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  int8_t ret = -1;
  switch (lun){
    case 0:
      (*block_num)  = W25Qxx_Sector_Count - 1;    /** begin from 0 */
      (*block_size) = W25Qxx_Sector_Space;
      ret = 0 ;
      break;
    case 1:
      (*block_num) = SDCard_GetSectorCount() - 1;    /** begin from 0  */
      (*block_size) = SDCard_GetSectorSize();    /**  */
      ret = 0;
      break;
    default:
      break;
  }
  return ret;
}

/**
  * @brief  Checks whether the medium is ready.
  * @param  lun: Logical unit number
  * @retval Status (0: Ok / -1: Error)
  */
int8_t STORAGE_IsReady(uint8_t lun) 
{
    static int8_t prev_status = 0;
    int8_t ret = -1;
    switch (lun){
        case 0:
            if (W25Qxx_WaitBusy() == 0)
            {
                if (prev_status < 0 && !W25Qxx_InitFS())
                {
                  prev_status = 0;
                }
                ret = 0;  // just wait busy 
            }
            else if (prev_status == 0)
            {
                prev_status = -1;
            }
            break;
        case 1: 
            if(SDCard_WaitBusy() == 0)
            {
                if(prev_status < 0 && !SDCard_initFS())   // may try multiple times 
                {
                  prev_status = 0;
                }
                ret = 0;  // just wait busy 
            }
            else if(prev_status == 0)
            {
                prev_status = -1;
            }
            break;
    default:
        break;
    }
    return ret;
}

/**
  * @brief  Checks whether the medium is write protected.
  * @param  lun: Logical unit number
  * @retval Status (0: write enabled / -1: otherwise)
  */
int8_t STORAGE_IsWriteProtected(uint8_t lun)
{   
    switch (lun){
        case 0:
            return 0; // if (W25Qxx_IsWriteProtected()) return -1
        case 1:
            return 0;  // if (SDCard_IsWriteProtected() == NOERR)
        default:
            break;
    }
    return -1;
}

/**
  * @brief  Reads data from the medium.
  * @param  lun: Logical unit number
  * @param  blk_addr: Logical block address
  * @param  blk_len: Blocks number
  * @retval Status (0: Ok / -1: Error)
  */
int8_t STORAGE_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    int8_t ret = -1;

    switch (lun){
        case 0:
            W25Qxx_WaitBusy();
            if (W25Qxx_ReadSector(buf, blk_addr, blk_len)) ret = -1;
            else ret = 0;
            break;
        case 1:
            SDCard_WaitBusy();
            if (SDCard_ReadBlock(buf, blk_addr, blk_len)!= NOERR) ret = -1;
            else ret = 0;
            break;
        default:
            break;
    }
    return ret;
}

/**
  * @brief  Writes data into the medium.
  * @param  lun: Logical unit number
  * @param  blk_addr: Logical block address
  * @param  blk_len: Blocks number
  * @retval Status (0 : Ok / -1 : Error)
  */
int8_t STORAGE_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    int8_t ret = -1;
    switch (lun){
        case 0:
            W25Qxx_WaitBusy();
            if (W25Qxx_WriteSector(buf,blk_addr,blk_len)) ret = -1;
            else ret = 0;
            break;
        case 1:
            SDCard_WaitBusy();
            if (SDCard_WriteBlock(buf, blk_addr, blk_len)!=NOERR) ret = -1;
            else ret = 0;
            break;
        default:
            break;
    }
    return ret;
}

/**
  * @brief  Returns the Max Supported LUNs.
  * @param  None
  * @retval Lun(s) number
  */
int8_t STORAGE_GetMaxLun(void)
{
    if (SDCard_initFS() == NOERR) {
        return STORAGE_LUN_NBR - 1;
    }else{
        /* error in sd card -> only W25Qxx is supported */
        return (STORAGE_LUN_NBR - 2);
    }
}

/**
  * @brief BSP Tx Transfer completed callbacks
  * @param None
  * @retval None
  */
void BSP_SD_WriteCpltCallback(void)
{
  writestatus = 1;
}

/**
  * @brief BSP Rx Transfer completed callbacks
  * @param None
  * @retval None
  */
void BSP_SD_ReadCpltCallback(void)
{
  readstatus = 1;
}
