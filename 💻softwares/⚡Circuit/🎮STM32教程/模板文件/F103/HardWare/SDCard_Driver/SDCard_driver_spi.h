#ifndef __SDCARD_DRIVER_SPI_H
#define __SDCARD_DRIVER_SPI_H

#ifdef __cplusplus
    extern "C"{
#endif

#define SDCARD_ROOTDIRECTORY "1:"
#define MAX_ABSOLUTE_PATH_LENGTH 256
#define NULL 0

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stm32f10x.h"
#include "stm32_spi_sd.h"
#include "ff.h"

typedef enum{
    NOERR          = (0x00),
    PARAERR,
    INITERR,   
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

typedef struct { 
    uint16_t task_num;
    uint8_t current_task_prop;
    // uint8_t total_task_prop;
}sdcard_task_pro_t;


SDCard_Err SDCard_Init(void);
SDCard_Err SDCard_GetSpace(uint32_t *totalspace, uint32_t *freespace);
SDCard_Err SDCard_WriteInfoFile(void);

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
#endif // 

#endif // !__SDCARD_DRIVER_SPI_H
