/* Includes ------------------------------------------------------------------*/
#include  "sdcard_driver.h"
#include  "stm32f4xx_hal_sd.h"

#include  "string.h"
#include  "stdio.h"
#include  "stdlib.h"

/* Private define ------------------------------------------------------------*/
#define MAX_ABSOLUTE_PATH_LENGTH    255

/* Private macro -------------------------------------------------------------*/
#define SD_TOTAL_SIZE_BYTE(__Handle__)  (((uint64_t)((__Handle__)->SdCard.LogBlockNbr) * ((__Handle__)->SdCard.LogBlockSize)) >> 0)
#define SD_TOTAL_SIZE_KB(__Handle__)    (((uint64_t)((__Handle__)->SdCard.LogBlockNbr) * ((__Handle__)->SdCard.LogBlockSize)) >> 10)
#define SD_TOTAL_SIZE_MB(__Handle__)    (((uint64_t)((__Handle__)->SdCard.LogBlockNbr) * ((__Handle__)->SdCard.LogBlockSize)) >> 20)
#define SD_TOTAL_SIZE_GB(__Handle__)    (((uint64_t)((__Handle__)->SdCard.LogBlockNbr) * ((__Handle__)->SdCard.LogBlockSize)) >> 30)

/* Private variables ---------------------------------------------------------*/
static GPIO_InitTypeDef GPIO_InitStructure;

static SD_HandleTypeDef        sd_handler;
static HAL_SD_CardInfoTypeDef  sd_info_Structure;
static HAL_SD_CardCSDTypeDef   sd_csd_structure;
static HAL_SD_CardCIDTypeDef   sd_cid_Structure;

static FATFS fs;
static char fs_buffer[512];     /* global variable for fs buffer */
static uint16_t sdcard_file_counter = 0;

/* this is used when transfer error interrupt occurs, it will be set to 1 */ 
volatile uint8_t SDcard_errState = 0; // 0: no error, 1: error 


/* Private function prototypes -----------------------------------------------*/
static uint8_t _sd_file_available_cb(FILINFO fno);
static uint8_t _sd_count_filenumber_cb(FILINFO fno);

uint8_t _sd_combine_dir(const char* root_dir, const char* dir, char* result_dir);
uint8_t _sd_get_parent_path(const char* filePath, char* dst_str);
uint8_t _sd_get_filename(const char* filePath, char* dst_str);

/* Private functions ---------------------------------------------------------*/

void HAL_SD_MspInit(SD_HandleTypeDef *hsd){
    /**  Init GPIO -------------- */ 
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    SDCARD_CD_GPIO_CLK_ENABLE(); 
    
    /** Data GPIO Ports --------- */
    GPIO_InitStructure.Pin  = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Alternate = GPIO_AF12_SDIO;   // use AF12 SDIO 
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    /** CLK GPIO Init ---------- */
    GPIO_InitStructure.Pin = GPIO_PIN_12;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    /** Command GPIO Init ------ */
    GPIO_InitStructure.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /** CD GPIO CLK ------------ */
    GPIO_InitStructure.Pin = SDCARD_CD_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;   // floating (must be pull up with resistance)
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Alternate = 0;
    HAL_GPIO_Init(SDCARD_CD_GPIO, &GPIO_InitStructure);
    
    /** Init Clock of SDIO ----- */
    __HAL_RCC_SDIO_CLK_ENABLE();
}

/// @brief retrieve judge if file is available 
/// @param fno 
/// @note The file/sub-directory must not have read-only attribute (AM_RDO), or the function will be rejected with FR_DENIED.
///       The sub-directory must be empty and must not be current directory, or the function will be rejected with FR_DENIED.
///       The file/sub-directory must not be opened, or the FAT volume can be collapsed. It will be rejected safely when file lock function is enabled.
/// @return 
static uint8_t _sd_file_available_cb(FILINFO fno){
    if ((fno.fattrib & AM_HID) || (fno.fattrib & AM_RDO)) return 1;
    return 0;
}

// 2. The sub-directory must be empty and must not be current director

/// @brief call back function for counting file number 
/// @param fno 
/// @note set sdcard_file_counter to 0 before counting 
/// @return 
static uint8_t _sd_count_filenumber_cb(FILINFO fno){
    if (!(fno.fattrib & AM_DIR)) {
        if (fno.fattrib & AM_HID) return 0; 
        sdcard_file_counter++;
    }
    return 0;
}

/// ========  several string processing functions =========

/// @brief combine the root directory and directory using "/"
/// @param root_dir     if use NULL, will not combine it.
/// @param dir          root directory;
/// @param result_dir   use char* dir = NULL and use it address for it.
/// @warning  dst_str must have enough space (MAX_ABSOLUTE_PATH_LENGTH) to receive data;
/// @return 0: combine succeed;   1: failed;
static uint8_t _sd_combine_dir(const char* root_dir, const char* dir, char* result_dir){
    if (strlen(root_dir) + strlen(dir) + 2 > MAX_ABSOLUTE_PATH_LENGTH) return 1;
    memset(result_dir, 0, strlen(result_dir));
    sprintf(result_dir, "%s/%s",root_dir, dir);
    return 0;
}

/// @brief get the absolute path of the parent path 
/// @param filePath 
/// @param dst_str 
/// @return 0: succeed;   1: failed;
/// @warning  dst_str must have enough space
static uint8_t _sd_get_parent_path(const char* filePath, char* dst_str){
    if (strlen(filePath) > MAX_ABSOLUTE_PATH_LENGTH) return 1;
    strcpy(dst_str, filePath);
    char* lastSlash = strrchr(dst_str, '/');
    if (lastSlash != NULL) {
        (*lastSlash) = '\0';    // set it to end character 
        return 0;
    }
    return 1;  // can't find the parent dir 
}

/// @brief get the file or folder name from the path 
/// @param filePath 
/// @param dst_str 
/// @return 0: succeed;   1: failed;
/// @warning the dst_str must have enough space for filePath or HardFault will happen;
static uint8_t _sd_get_filename(const char* filePath, char* dst_str){
    if (strlen(filePath) > MAX_ABSOLUTE_PATH_LENGTH) return 1;
    const char* lastSlash = strrchr(filePath,'/');
    if (lastSlash !=NULL){
        strcpy(dst_str,lastSlash+1);
        return 0;
    }
    return 1;
}

/* Public functions -----------------------------------------------------------*/

/// @brief Init SD Card
/// @return 
SDCard_Err SDCard_init(void){
    sd_handler.Instance = SDIO;
    sd_handler.Init.BusWide         = SDIO_BUS_WIDE_1B;   // use DAT_0 for initialization
    sd_handler.Init.ClockBypass     = SDIO_CLOCK_BYPASS_DISABLE;
    /** In the Init Function,  the CLK_DIV will be set to SDIO_INIT_CLK_DIV automatically */ 
    sd_handler.Init.ClockDiv        = SDCARD_TRANSFER_DIV_USB;   // use <= 25Mhz for sdio data transfer;
    sd_handler.Init.ClockEdge       = SDIO_CLOCK_EDGE_RISING;
    sd_handler.Init.ClockPowerSave  = SDIO_CLOCK_POWER_SAVE_DISABLE;
    sd_handler.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;   // disable the hardware flow control
    if (HAL_SD_Init(&sd_handler) != HAL_OK){
        return INITERR;
    }
    HAL_SD_GetCardInfo(&sd_handler, &sd_info_Structure);
    HAL_SD_GetCardCID(&sd_handler,&sd_cid_Structure);
    HAL_SD_GetCardCSD(&sd_handler,&sd_csd_structure);

    // change transfer type to 4 line transfer mode;
    if (HAL_SD_ConfigWideBusOperation(&sd_handler, SDIO_BUS_WIDE_4B)!=HAL_OK){
        return INITERR;
    }
    return NOERR;
}

/// @brief Init SD Card with File System 
/// @param  
/// @return 
SDCard_Err SDCard_initFS(void){
    FRESULT res =  f_mount(&fs, SDCARD_ROOTDIRECTORY, 1);
    if (res == FR_OK){
        return NOERR;
    }
    else if (res == FR_NO_FILESYSTEM){
        /** begin format SD card */
        res = f_mkfs(SDCARD_ROOTDIRECTORY, 0, fs_buffer, SDCARD_SECTOR_SZ);
        if (res == FR_OK) {
            res = f_mount(&fs, SDCARD_ROOTDIRECTORY, 1);
            if (res == FR_OK){
                return NOERR;
            }
        }
    }
    return INITERR;
}

/// @brief Write a info file for SD card and log its type, serial number, etc. 
/// @param 
/// @return 
SDCard_Err SDCard_WriteInfoFile(void){
    FIL fp; FRESULT res; SDCard_Err result = NOERR;
    uint32_t tot_space, free_space;
    
    char result_dir[MAX_ABSOLUTE_PATH_LENGTH];
    _sd_combine_dir(SDCARD_ROOTDIRECTORY, "cardinfo.txt", result_dir);  // create cardinfo.txt at root directory
    res = f_open(&fp, result_dir, FA_CREATE_ALWAYS|FA_WRITE);
    
    if (res != FR_OK) { f_close(&fp);  return FileOpenERR;} 
    // try write the SD Card Multipule times and just check the last res
    char* msg[10];
    msg[0] = "=============== SD card Infomation Logs ===============\r\n";
    
    uint8_t card_type, card_version;
    
    SDCard_GetType(&card_type, &card_version);
    if (SDCard_GetSpace(&tot_space, &free_space)!=NOERR) return NoInfo;
    
    switch (card_version)
    {
    case CARD_V1_X:
        msg[1] = "Type : SD Card V1.0 Card\r\n"; break;
    case CARD_V2_X:
        if (card_type == CARD_SDSC){
            msg[1] = "Type : SD Standard Capacity Card (SDSC) V2.0\r\n";
        }
        else if (card_type == CARD_SDHC_SDXC){
            msg[1] = "Type : SD High Capacity Card (SDHC) V2.0\r\n";
        }
        else{
            msg[1] = "Type : Secured SD Card V2.0\r\n"; 
        }
        break;
    }
    msg[2] = (char*)malloc(45);
    sprintf(msg[2], "total sector count: %d\r\n",SDCard_GetSectorCount());
    
    msg[3] = (char*)malloc(45);
    sprintf(msg[3], "card space total: %.2f MB\r\n", tot_space/1024.0);
    
    msg[4] = (char*)malloc(35);
    sprintf(msg[4], "card space free: %.2f MB\r\n", free_space/1024.0);
    
    msg[5] = (char*)malloc(200);
    uint16_t year  = ((sd_cid_Structure.ManufactDate & 0xFF0)>> 4) + 2000;
    uint16_t month = ((sd_cid_Structure.ManufactDate)&0xF);
    /** lu : unsigned long number */
    sprintf(msg[5], "manufacture in :%d\\%d,product serial number: %lu\r\n",year, month, (uint32_t)sd_cid_Structure.ProdSN);
    msg[6] = "Start Enjoy it!\r\n";
    for (int i = 0; i<7; i++){
       if (f_puts(msg[i], &fp)<0) result = FileWriteERR;
    }
    free(msg[2]); free(msg[3]); free(msg[4]); free(msg[5]);  // free the space after construct
    f_close(&fp);
    return result;
}

/// @brief Read sector Block data from sd card 
/// @param hsd  sd card handler typedef 
/// @param buff buffer (size = 512 * N)
/// @param saddr addr of first sector to read 
/// @param sec_cnt count of sector 
/// @return 
SDCard_Err SDCard_ReadBlock(uint8_t *buff, uint32_t saddr, uint32_t sec_cnt){
    __disable_irq();
    HAL_StatusTypeDef res = HAL_SD_ReadBlocks(&sd_handler, buff, saddr, sec_cnt, SDMMC_DATATIMEOUT);
    SDCard_WaitBusy();
    __enable_irq();

    if (res!= HAL_OK) return FileReadERR;
    return NOERR;
}

/// @brief Write Block 
/// @param buff 
/// @param saddr
/// @param sec_cnt 
/// @return 
SDCard_Err SDCard_WriteBlock(uint8_t *buff, uint32_t saddr, uint32_t sec_cnt)
{
    __disable_irq();
    HAL_StatusTypeDef res = HAL_SD_WriteBlocks(&sd_handler, buff,  saddr, sec_cnt, SDMMC_DATATIMEOUT);
    SDCard_WaitBusy();
    __enable_irq();
    if  (res!= HAL_OK) return FileWriteERR;
    return NOERR;
}

/// @brief called when re-get infomation 
/// @param 
void SDCard_getinfo(void){
    HAL_SD_GetCardInfo(&sd_handler, &sd_info_Structure);
}

/// @brief wait busy for sd card function 
/// @param  
/// @return 
SDCard_Err SDCard_WaitBusy(void){
    uint32_t timeout = SDMMC_DATATIMEOUT;
    // note: this function is called for check if can transfer data
    while (HAL_SD_GetCardState(&sd_handler) != HAL_SD_CARD_TRANSFER){
        if (timeout-- == 0) return TIMEOUTERR;
    }
    return NOERR;
}

/// @brief get sector size
/// @param  
/// @return 
uint32_t SDCard_GetSectorSize(void){
    return SDCARD_SECTOR_SZ;
}

/// @brief 
/// @param  
/// @return 0: not write protected, 1: write protected, 2: other errors
SDCard_Err SDCard_IsWriteProtected(void){
    FRESULT res = f_mount(&fs, SDCARD_ROOTDIRECTORY, 1); 
    if (res == FR_OK) return NOERR;
    else if (res == FR_WRITE_PROTECTED) return CARDPROTECTEDERR;
    return INITERR;
}

/// @brief get sector count 
/// @param 
/// @return 
uint32_t SDCard_GetSectorCount(void){
    return sd_info_Structure.BlockNbr * (sd_info_Structure.BlockSize/SDCARD_SECTOR_SZ);
}

/// @brief get erase block size in unit of sector 
/// @param  
/// @return 
uint32_t SDCard_GetBlockSize(void){
    return sd_csd_structure.EraseGrSize;
}

/// @brief 
/// @param  
/// @return CARD_V1_X (0U) or  CARD_V2_X (1U)
void SDCard_GetType(uint8_t *type, uint8_t *version){
    (*type) = sd_info_Structure.CardType;
    (*version) = sd_info_Structure.CardVersion;
}

/// @brief Get the space of SD Card 
/// @param totalspace 
/// @param freespace 
/// @warning only works correctly when clust 512kb 
/// @return 
SDCard_Err SDCard_GetSpace(uint32_t* totalspace, uint32_t* freespace){
    DWORD freeclst;
    FATFS *fs;
    if (f_getfree(SDCARD_ROOTDIRECTORY, &freeclst, &fs)) return NoInfo;
	
    // for clust is always 512 kb (csize : sector per cluster)
    *totalspace = (fs->n_fatent - 2) * fs->csize /2; // (fs.n_fatent - 2) * 512/1024 
    *freespace  = freeclst * fs->csize /2;

    return NOERR;
}

/// @brief get current state of sd card
/// @return 0: normal ,  other: busy / error
SDCard_Err SDCard_GetState(void){
    if (HAL_SD_GetCardState(&sd_handler) == HAL_SD_CARD_TRANSFER) return NOERR;
    return INITERR;
}




/// @brief Create File in the specified directory
/// @param path_to_create  create at root use "root_directory", or use "current_directory"
/// @param dir      directory to be created (abs shouldn't exceed MAX_ABSOLUTE_PATH_LENGTH characters)
/// @param override if set to false return error when file exists.
/// @return 
SDCard_Err SDCard_CreateFile(const char* path, bool override){
    FIL file; FRESULT res;
    SDCard_Err result = NOERR;
    if (!override){
        res = f_open(&file, path, FA_CREATE_NEW);
    }
    else{
        res = f_open(&file, path, FA_CREATE_ALWAYS);
    }
    if (res!=FR_OK) result = FileCreateERR;
    f_close(&file);
    return result;
}

/// @brief write to specified file (if file not exists, it would create).
/// @param path_to_create create at root use "root_directory", or use "current_directory"
/// @param dir       directory of the file 
/// @param write_buf the string to write to the file 
/// @param append    if set to false, open mode "w", true -> "a"
/// @note  this function can only write file < 512bytes, but can switch to append mode;
/// @return 
SDCard_Err SDCard_WriteFile(const char* path, const char* write_buf, bool append){
    // try create a file in SD card;
    FIL file;  FRESULT res; SDCard_Err result = NOERR;
    UINT write_size;                              // note: this just receives the number of bytes written 
    BYTE write_mode = FA_CREATE_ALWAYS|FA_WRITE;       // "w" for not append, "a" for append
    if (append) write_mode = FA_OPEN_APPEND|FA_WRITE; 
    
    res = f_open(&file, path, write_mode);  // must use result dir for it. 
    if ((res&FR_DENIED) == FR_DENIED) return StorgeFull;
    else if(res != FR_OK) return FileCreateERR;
    
    // write the string (note: use strlen() not use sizeof())
    res = f_write(&file, write_buf, strlen(write_buf), &write_size); if (res != FR_OK) result = FileWriteERR;
    f_close(&file);
    return result; 
}

/// @brief Create Directory on SD card Disk, if it exits, it not create it. 
/// @param path_to_create if create at root use "root_directory", or use "current directory"
/// @param dir folder directory (depend on "path_to_create" specified) 
/// @return 
SDCard_Err SDCard_CreateFolder(const char* path){
    FRESULT res;
    DIR dir;

    res = f_opendir(&dir, path);
    if (res!=FR_OK){
        res = f_mkdir(path);  // try to create 
    }
    f_closedir(&dir);  // try close dir;
    if (res!=FR_OK) return  FileCreateERR;
    return NOERR;
}

/// @brief Delete the file or folder (use retrieve option to delete all in the folder)
/// @param path 
/// @param retrieve  if set to false, it would failed if the folder is not empty.
/// @note  all files under the folder  must be deleteable (not Hidden and not read-only);
/// @return 
SDCard_Err SDCard_DeleteFolder(const char* path, bool retrieve){
    SDCard_Err result = NOERR;
    FRESULT res;
    FILINFO fno;
    
    if (f_stat(path, &fno) || (fno.fattrib & AM_HID) || (fno.fattrib & AM_RDO)) return FileInfoERR;
    if(!retrieve) {
        res = f_unlink(path);
        if (res!= FR_OK) result = DirDeleteERR;
    }else{
        if (fno.fattrib & AM_DIR){
            DIR dir;
            if (f_opendir(&dir,path)!=FR_OK) return DirOpenERR;
            while (1){
                char res_dir[MAX_ABSOLUTE_PATH_LENGTH];
                res = f_readdir(&dir, &fno);
                if (res!= FR_OK || fno.fname[0] == 0) break;   // read end, break normally.
                if (_sd_combine_dir(path, fno.fname, res_dir)){ result = DirCreateERR; break;}
                if (SDCard_DeleteFolder(res_dir,true)!= NOERR) { result = DirDeleteERR; break;}
                // every sub-folder and file should be deleteable 
            }
            f_closedir(&dir); 
            res = f_unlink(path);
            if (res!= FR_OK) result = DirDeleteERR;
        }else {
            return SDCard_DeleteFolder(path, false);
        }
    }
    return result;
}

#ifdef __cplusplus
extern "C"{
#endif

/// @brief read the path, and call info_func_cb with the FILINFO type infomation
/// @param path 
/// @param info_func_cb return 0 is execute succeed;
/// @return 
SDCard_Err SDCard_ListPath(const char* path, uint8_t (*info_func_cb)(FILINFO fno)){
    DIR dir;
    if (f_opendir(&dir , path)!= FR_OK) return DirOpenERR;
    FILINFO fno;
    while (1){
        FRESULT res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) break;
        if (info_func_cb != NULL && info_func_cb(fno)) { f_closedir(&dir); return FileInfoERR;}
    }
    f_closedir(&dir);
    return NOERR;
}

/// @brief retrive all the folder and file of the "path" specified 
/// @param path 
/// @param info_func_cb 
/// @return 
SDCard_Err SDCard_ListPath_retrieve(const char* path, uint8_t (*info_func_cb)(FILINFO fno)){
    DIR dir;
    if (f_opendir(&dir, path)!=FR_OK) return DirOpenERR;
    FILINFO fno;
    while (1){
        FRESULT res = f_readdir(&dir, &fno);
        if (res!= FR_OK || fno.fname[0] == 0) break;
        if(info_func_cb!=NULL && info_func_cb(fno)) { f_closedir(&dir); return FileInfoERR;}
        if (fno.fattrib & AM_DIR){
            char str[MAX_ABSOLUTE_PATH_LENGTH];
            if (_sd_combine_dir(path, fno.fname, str)) continue;  // when the length is too long 
            SDCard_ListPath_retrieve(str, info_func_cb);
        }
    }
    f_closedir(&dir);
    return NOERR;
}


/// @brief Read the File sector by sector and each read action would call read_file func 
/// @param path 
/// @param read_file_func_cb since the funtion read by sector, it call this function every sector it read.
/// @return 
SDCard_Err SDCard_ReadFile(const char* path, uint8_t (*read_file_func_cb)(const char* buff)){
    FIL file; FRESULT res;
    UINT read_size;
    res = f_open(&file, path, FA_READ);  if (res!= FR_OK) return FileOpenERR;
    
    while (1){
        // **** important -> the btr parameter must be sizeof (read_buf) - 1, or first bit will error****
        memset(fs_buffer , 0, strlen(fs_buffer));
        res = f_read(&file, fs_buffer, sizeof(fs_buffer), &read_size);
        if (res!=FR_OK) { // if the SD card plug out, it returns.
            f_close(&file);  return FileReadERR;
        }
        if (read_file_func_cb!=NULL && read_file_func_cb(fs_buffer)){
            f_close(&file); return FileReadERR;
        }
        if (read_size == 0) break;  // the file read ended;
    }
    f_close(&file);
    return NOERR;
}

/// @brief Copy file from  the src_dir to dst_dir (will rename it.)
/// @param dst_dir     need to specify name!!!! 
/// @param force_cp    if set to false, if dst_dir exists, the function will return err, true to replace the dst file.
/// @param process_func it receive the process proportion (from 0 to 100), n is number of task remained;
/// @return 
SDCard_Err SDCard_CopyFile(const char* src_dir, const char* dst_dir, bool force_cp, void (*process_func)(uint8_t prop)){
    FIL fp1, fp2;
    char BUF[MAX_ABSOLUTE_PATH_LENGTH];  // give 512byte space for buffer for faster copy and move; -> 
    FRESULT res;  FILINFO fno;
    UINT read_size, write_size;
    SDCard_Err result = NOERR;
    if (strcmp(src_dir, dst_dir) == 0) return FileCreateERR;
    
    res = f_open(&fp1, src_dir, FA_READ);
    if (res != FR_OK) return FileOpenERR;
    res = f_stat(src_dir, &fno); if (res != FR_OK || (fno.fattrib & AM_HID)) {
        f_close(&fp1); return FileOpenERR;
    }  // can't copy hidden files
    
    if (force_cp){
        res = f_open(&fp2 , dst_dir, FA_WRITE|FA_CREATE_ALWAYS);
    }else{
        res = f_open(&fp2 , dst_dir, FA_CREATE_NEW|FA_WRITE);
    }
    if (res != FR_OK) { f_close(&fp1); return FileOpenERR;}

    uint32_t space_tot = f_size(&fp1)/1024 * 2;   // the total space in sector (1 sector = 512byte )
    uint32_t space_cp  = 0;                       // the sector that copied
    
    while (1){
        res = f_read(&fp1, BUF, sizeof(BUF), &read_size);
        if (res!=FR_OK) {
            result = FileReadERR; break;  // if the SD card plug out, it occurs 
        }
        if (read_size == 0) break;        // eof error or break interval;
        res = f_write(&fp2, BUF, read_size, &write_size);
        if (res!=FR_OK) {
            result = FileWriteERR; break; // the destination can't be written.
        }
        space_cp += 1;    // 512 byte copied;
        if (process_func!=NULL) process_func((int)((float)space_cp/space_tot * 100));
        if (write_size < read_size) break;    //  disk full error
    }
    
    f_close(&fp1); f_close(&fp2);
    return result;
}



/// @brief Copy Folder or file to the "dest_fold_dir" location. (if the dst_fold_dir not exist, will try create it)
/// @param src_dir      
/// @param dst_fold_dir directory for destination folder (no need to specify name!)
/// @param force_cp     if force to copy files if file already exist.
/// @param process_func  receive the process proportion (from 0 to 100), n is number of task remained 
///            (use sdcard_file_counter for remain file to copy);
/// @note  call SDCard_ListPath_retrieve first to get files to cp in user_copy function 
/// @return 
SDCard_Err SDCard_CopyFolder(const char* src_dir, const char* dst_fold_dir, bool force_cp, void (*process_func)(uint8_t prop)){
    FRESULT res;
    FILINFO fno, fno_child;   // fno : info of parent folder;  fno_child: info of child foldder
    DIR dir;                  // firstly, will create directory under the dst_fold_dir 
    char path1[MAX_ABSOLUTE_PATH_LENGTH];
    _sd_get_parent_path(src_dir, path1);

    // avoid to copy to the same place or hidden file 
    if (strcmp(path1, dst_fold_dir) == 0) return DirCreateERR;  
    if (f_stat(src_dir, &fno)  ||  (fno.fattrib & AM_HID)) return FileInfoERR;
    
    // in this loop, each file directory is listed as result_dir
    if (fno.fattrib & AM_DIR){
        char path2[MAX_ABSOLUTE_PATH_LENGTH];
        /* if not exist, try create this directory.(include the dst_folder and under dst_dir)*/
        if (SDCard_CreateFolder(dst_fold_dir) || 
            _sd_combine_dir(dst_fold_dir, fno.fname, path2) || 
                (SDCard_CreateFolder(path2)))
            return DirCreateERR;

        if (f_opendir(&dir,src_dir)!= FR_OK) return DirOpenERR;
        // use path2 as the dst folder name; path 1 as fully name of files in src folder;
        while (1){    // List Path and copy every file to the destination.            
            res = f_readdir(&dir, &fno_child);      // read the directory and get the name 
            if (_sd_combine_dir(src_dir, fno_child.fname, path1)) {
                f_closedir(&dir); return DirCreateERR;
            }
            if (res!= FR_OK || fno_child.fname[0] == 0) break;     // read end
            if (SDCard_CopyFolder(path1, path2, force_cp, NULL)!=FR_OK){
                // copy folder retrieve. also tip when
                /* SDCard_File_Error Handeler ............ */  
            }
        }
        f_closedir(&dir);
    }
    else  // if src_dir is a file, copy the file to destination
    {
        SDCard_Err result = SDCard_CreateFolder(dst_fold_dir);
        if (result) return DirCreateERR;
        if (_sd_combine_dir(dst_fold_dir, fno.fname, path1)) return DirCreateERR;
        if (SDCard_CopyFile(src_dir, path1, force_cp, process_func)!=FR_OK){
            // jump the invalid file copy. 
            /* SDCard_File_Error Handeler ............ */
        }
        if (sdcard_file_counter) sdcard_file_counter--;
    }
    SDCard_WaitBusy();
    return NOERR;
}

/// @brief rename file or folder
/// @param path      total path of file 
/// @param path_new  total path of folder
/// @return 
SDCard_Err SDCard_RenameFolder(const char* path, const char* path_new){
    FILINFO fno;
    if (strcmp(path,path_new) == 0) return FileCreateERR;
    f_stat(path, &fno);  // firstly, judge whether path is folder or file
    if ((fno.fattrib & AM_HID) || (fno.fattrib & AM_RDO)) return FileInfoERR;
    if (f_rename(path, path_new)!=FR_OK) return DirCreateERR;
    return NOERR;
}

/// @brief Move file or folder to dst_fold_dir (if dst_folder_dir not exist, will try create it)
/// @param src_dir directory of file/folder to be moved 
/// @param dst_fold_dir directory to move to 
/// @note if same name folder already exists in dst_fold_dir, it will not move and return error
/// @return 
SDCard_Err SDCard_MoveFolder(const char* src_dir, const char* dst_fold_dir){
    FILINFO fno;
    if (f_stat(src_dir, &fno)!=FR_OK) return FileOpenERR;
    if ((fno.fattrib & AM_HID) || (fno.fattrib & AM_RDO)) return FileInfoERR;
    
    if ((fno.fattrib & AM_DIR) && SDCard_ListPath_retrieve(src_dir, _sd_file_available_cb)) return FileInfoERR;
    
    char res_dir[MAX_ABSOLUTE_PATH_LENGTH], res_dir2[MAX_ABSOLUTE_PATH_LENGTH];
    if (_sd_get_parent_path(src_dir, res_dir) || 
        (strcmp(res_dir, dst_fold_dir) == 0)  ||   // dst dir shouldn't same the dir of src_dir
        _sd_get_filename(src_dir, res_dir2)   ||
        _sd_combine_dir(dst_fold_dir, res_dir2, res_dir))
        return DirCreateERR;
    
    // first, try create dst_fold_dir
    if (SDCard_CreateFolder(dst_fold_dir)) return DirCreateERR;
    
    // res_dir is the path to create folder (if alerday exists, not do it)
    if (f_rename(src_dir, res_dir)!= FR_OK) return FileWriteERR;
    return NOERR;
}

#ifdef __cplusplus
    }
#endif // __cplusplus
