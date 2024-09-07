#ifndef __SDCARD_DRIVER__
#define __SDCARD_DRIVER__

#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "ff.h"

#define SDCARD_SECTOR_SZ            512
#define SDCARD_ROOTDIRECTORY        "1:"

#define SDCARD_CD_GPIO_CLK_ENABLE   __HAL_RCC_GPIOD_CLK_ENABLE
#define SDCARD_CD_GPIO              GPIOD
#define SDCARD_CD_GPIO_PIN          GPIO_PIN_1

#define SDCARD_TRANSFER_DIV_USB     1U

typedef enum{
    NOERR          = (0x00),
    CARDPROTECTEDERR   = (0x01),
    PARAERR,
    INITERR,   
    TIMEOUTERR,
    NoInfo,
    FileCreateERR,   // can't create file for invalid name
    StorgeFull,
    FileOpenERR,
    FileInfoERR,
    FileCloseERR,
    FileReadERR,
    FileWriteERR,
    DirOpenERR,
    DirCreateERR,
    DirCloseERR,
    DirDeleteERR
}SDCard_Err;

SDCard_Err SDCard_init(void);
SDCard_Err SDCard_initFS(void);
SDCard_Err SDCard_WriteInfoFile(void);
SDCard_Err SDCard_ReadBlock(uint8_t *buff, uint32_t saddr, uint32_t sec_cnt);
SDCard_Err SDCard_WriteBlock(uint8_t *buff, uint32_t saddr, uint32_t sec_cnt);
void SDCard_getinfo(void);

SDCard_Err SDCard_WaitBusy(void);   /** used more frequent than getstate */
SDCard_Err SDCard_GetState(void);
SDCard_Err SDCard_IsWriteProtected(void);

uint32_t SDCard_GetSectorSize(void);
uint32_t SDCard_GetSectorCount(void);
uint32_t SDCard_GetBlockSize(void);
void SDCard_GetType(uint8_t *type, uint8_t *version);

SDCard_Err SDCard_GetSpace(uint32_t* totalspace, uint32_t* freespace);





/** File System Operation Functions --> call it when use W25Qxx */

SDCard_Err SDCard_CreateFile(const char* path, bool override);
SDCard_Err SDCard_WriteFile(const char* path, const char* write_buf, bool append);
SDCard_Err SDCard_CreateFolder(const char* path);
SDCard_Err SDCard_DeleteFolder(const char* path, bool retrieve);

SDCard_Err SDCard_ListPath(const char* path, uint8_t (*info_func_cb)(FILINFO fno));
SDCard_Err SDCard_ListPath_retrieve(const char* path, uint8_t (*info_func_cb)(FILINFO fno));
SDCard_Err SDCard_ReadFile(const char* path, uint8_t (*read_file_func_cb)(const char* buff));
SDCard_Err SDCard_CopyFile(const char* src_dir, const char* dst_dir, bool force_cpy, void (*process_func)(uint8_t prop));
SDCard_Err SDCard_CopyFolder(const char* src_dir, const char* dst_fold_dir, bool force_cp, void (*process_func)(uint8_t prop));

SDCard_Err SDCard_RenameFolder(const char* path, const char* path_new);
SDCard_Err SDCard_MoveFolder(const char* src_dir, const char* dst_fold_dir);

#ifdef __cplusplus
    }
#endif // __cplusplus

#endif // !__SDCARD_DRIVER__
